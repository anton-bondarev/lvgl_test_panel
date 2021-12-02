/**
 * @file
 * @brief LVGL demo
 *
 * @date 15.02.2021
 * @author Alexander Kalmuk
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>

#include <kernel/time/timer.h>

#include "lvgl.h"

#include "lvgl_port.h"
#include <modbus_server_conf.h>

#include <modbus.h>

#define USE_HTTP_CONTROL 0

#define TIMER_TICK_PERIOD 50



#if USE_HTTP_CONTROL
static int sock_fd;
#else
static modbus_t *ctx;
#endif

static char *fb_path;
static char *input_dev_path;
static int input_is_mouse = 0;

static lv_color_t *buf1_1;
#if LVGL_VERSION_MAJOR == 7
static lv_disp_buf_t disp_buf1;
#elif LVGL_VERSION_MAJOR == 8
static lv_disp_draw_buf_t disp_buf1;
#else
#error "wrong LVGL version"
#endif

#if LVGL_VERSION_MAJOR == 7
static void input_dev_mouse_handler(lv_task_t *t) {
	lvgl_port_mouse_handle();
}

static void input_dev_touchscreen_handler(lv_task_t *t) {
	lvgl_port_touchscreen_handle();
}
#else
static void input_dev_mouse_handler(lv_timer_t *t) {
	lvgl_port_mouse_handle();
}

static void input_dev_touchscreen_handler(lv_timer_t *t) {
	lvgl_port_touchscreen_handle();
}
#endif

static int hal_init(void) {
	static lv_disp_drv_t disp_drv;
	static lv_indev_drv_t indev_drv;
	lv_indev_t *mouse_indev;
	lv_obj_t *cursor_obj;

	if (lvgl_port_fbdev_init(fb_path) < 0) {
		fprintf(stderr, "Failed to init framebuffer %s\n", fb_path);
		return -1;
	}

	buf1_1 = malloc(LV_HOR_RES_MAX * LV_VER_RES_MAX * 4);
	if (!buf1_1) {
		fprintf(stderr, "Error allocation buffer for LVGL display\n");
		return -1;
	}
#if LVGL_VERSION_MAJOR == 7
	lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
#else
	lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
#endif
	/*Create a display*/
	lv_disp_drv_init(&disp_drv);
#if LVGL_VERSION_MAJOR == 7
	disp_drv.buffer = &disp_buf1;
#else
    /*Set the resolution of the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
	disp_drv.draw_buf = &disp_buf1;
#endif
	disp_drv.flush_cb = lvgl_port_fbdev_flush;
	lv_disp_drv_register(&disp_drv);

	if (lvgl_port_input_dev_init(input_dev_path) < 0) {
		fprintf(stderr, "Error open input device %s\n", input_dev_path);
		goto err_free;
	}

	/* Add the mouse as input device */
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = lvgl_port_input_dev_read;
	mouse_indev = lv_indev_drv_register(&indev_drv);

	if (input_is_mouse) {
		/*Set a cursor for the mouse*/
		LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
#if LVGL_VERSION_MAJOR == 7
		cursor_obj = lv_img_create(lv_scr_act(), NULL);
#else
		cursor_obj = lv_img_create(lv_scr_act());
#endif
		lv_img_set_src(cursor_obj, &mouse_cursor_icon);
		lv_indev_set_cursor(mouse_indev, cursor_obj);
	}
#if LVGL_VERSION_MAJOR == 7
	if (input_is_mouse) {
		lv_task_create(input_dev_mouse_handler, 10, LV_TASK_PRIO_HIGH, NULL);

	} else {
		lv_task_create(input_dev_touchscreen_handler, 10, LV_TASK_PRIO_HIGH, NULL);
	}
#else
	if (input_is_mouse) {
		lv_timer_create(input_dev_mouse_handler, 10, NULL);

	} else {
		lv_timer_create(input_dev_touchscreen_handler, 10, NULL);
	}
#endif
	return 0;

err_free:
	free(buf1_1);
	return -1;
}

static void update_sersors(void) {
	extern void test_panel_set_sensor(int value[5]);
	static int sensor_value[5]= {0};

#if 0
	sensor_value[0]++;
	sensor_value[0] = sensor_value[0] % 100;

	sensor_value[1] += 2;
	sensor_value[1] = sensor_value[1] % 100;

	sensor_value[2] += 3;
	sensor_value[2] = sensor_value[2] % 100;

	sensor_value[3] += 4;
	sensor_value[3] = sensor_value[3] % 100;

	sensor_value[4] += 5;
	sensor_value[4] = sensor_value[4] % 100;
#else
	static uint16_t tab_input_registers[123];
	int rc;

	rc = modbus_read_input_registers(ctx, 0, 123, tab_input_registers);
	if (rc < 0) {
		printf("modbus_read_input_registers error %d ", rc);
	}
	sensor_value[0] = (int16_t)tab_input_registers[0];

	sensor_value[1] = (int16_t)tab_input_registers[2];

	sensor_value[2] = (int16_t)tab_input_registers[4];

	sensor_value[3] = (int16_t)tab_input_registers[6];

	sensor_value[4] = (int16_t)tab_input_registers[8];
#endif
	test_panel_set_sensor(sensor_value);
}

#define REQUEST_CLR "GET /cgi-bin/led_driver?cmd=clr&led=0 HTTP/1.1\r\n\r\n"
#define REQUEST_SET "GET /cgi-bin/led_driver?cmd=set&led=0 HTTP/1.1\r\n\r\n"
void test_panel_update_button(int state) {
	printf("State: %s\n", state ? "On" : "Off");
#if USE_HTTP_CONTROL
	if (sock_fd) {
		if (state) {
			send(sock_fd, REQUEST_SET, sizeof(REQUEST_SET) - 1, 0);
		} else {
			send(sock_fd, REQUEST_CLR, sizeof(REQUEST_CLR) - 1, 0);
		 }
	}
#else
	if (1 == modbus_write_bit(ctx, 0,  state ? ON : OFF)) {
		printf("OK\n");
	} else {
		printf("FAILED\n");
	}
#endif
}

static void lvgl_timer_handler(struct sys_timer *timer, void *param) {
	(void) timer;
	(void) param;

	lv_tick_inc(TIMER_TICK_PERIOD);
}

static void print_usage(void) {
	printf("Usage: lvgl_demo [-t] fb input_dev\n"
	       "\t -t: Ifspecified, this means input device is touchscreen.\n"
	       "\t     Othervice, it's mouse.\n"
	);
}

static int socket_init(void) {
#if USE_HTTP_CONTROL
	struct sockaddr_in cliaddr;

	memset(&cliaddr, 0, sizeof cliaddr);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(NEIGHBOUR_PORT);
	if (inet_aton(NEIGHBOUR_ADDR, &cliaddr.sin_addr) == 0) {
		printf("Fail\n");
		return 0;
	}
	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_fd == -1) {
		perror("socket");
		sock_fd = 0;
		return -errno;
	}

	if (-1 == connect(sock_fd, (struct sockaddr *)&cliaddr, sizeof(cliaddr))) {
		perror("connect");
		close(sock_fd);
		sock_fd = 0;
		return -errno;
	}

#else
	const char *ip = NEIGHBOUR_ADDR;
	unsigned short port = NEIGHBOUR_PORT;

	ctx = modbus_new_tcp(ip, port);
	if (ctx == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		return -1;
	}

	modbus_set_debug(ctx, TRUE);
	modbus_set_error_recovery(ctx,
			MODBUS_ERROR_RECOVERY_LINK |
			MODBUS_ERROR_RECOVERY_PROTOCOL);

	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}
#endif
	return 0;
}
extern void lv_demo_widgets(void);
int main(int argc, char **argv) {
	int opt;
	struct sys_timer *timer;

	input_is_mouse = 1;

	if (argc < 3) {
		print_usage();
		return 0;
	}

	while (-1 != (opt = getopt(argc, argv, "ht"))) {
		switch (opt) {
		case 't':
			input_is_mouse = 0;
			break;
		case 'h':
			print_usage();
			/* fallthrough */
		default:
			return 0;
		}
	}

	fb_path = argv[argc - 2];
	input_dev_path = argv[argc - 1];

	/*Initialize LVGL*/
	lv_init();

	/*Initialize the HAL (display, input devices, tick) for LVGL*/
	if (0 != hal_init()) {
		fprintf(stderr, "lvgl hal_init failed\n");
		return -1;
	}

	socket_init();

	lv_demo_widgets();

	timer_set(&timer, TIMER_PERIODIC, TIMER_TICK_PERIOD, lvgl_timer_handler, NULL);

	while (1) {
		static int cnt;
		/* Periodically call the lv_task handler.
		 * It could be done in a timer interrupt or an OS task too.*/
#if LVGL_VERSION_MAJOR == 7
		lv_task_handler();
#else
		lv_timer_handler();
#endif
		usleep(5 * 1000);
		cnt ++;
		if ((cnt % 100) == 0) {
			cnt = 0;
			update_sersors();
		}
	}


	/* Close the connection */
#if USE_HTTP_CONTROL
	close(sock_fd);
#else
	modbus_close(ctx);
	modbus_free(ctx);
#endif
	return 0;
}
