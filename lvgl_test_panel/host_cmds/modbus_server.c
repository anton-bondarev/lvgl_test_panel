/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    06.11.2014
 */

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include <modbus.h>

#define MOTION_SENSORS_QUANTITY         (60)

#define INPUT_REGISTERS_QUANTITY       (MOTION_SENSORS_QUANTITY * 2 + 2 + 1)

#define BITS_QUANTITY                  (10)

struct motion_sensor_struct {
	uint16_t value;
	uint16_t flags;
};

struct input_registers {
	struct motion_sensor_struct motions[MOTION_SENSORS_QUANTITY];
	uint16_t pressures[2];
	uint16_t temp;
};
	
static struct input_registers input_registers = {0};
static uint8_t io_bits[BITS_QUANTITY] = {0};

static modbus_mapping_t *mb_mapping_wrapper_new(void) {
	int i;
	modbus_mapping_t *mb_mapping;
	mb_mapping = modbus_mapping_new(BITS_QUANTITY, 0, 0, INPUT_REGISTERS_QUANTITY);

	/* Initialize input values that's can be only done server side. */
	modbus_set_bits_from_bytes(mb_mapping->tab_bits, 0, 1, io_bits);

       for (i = 0; i < MOTION_SENSORS_QUANTITY; i++) {
	       input_registers.motions[i].value = i;
	       input_registers.motions[i].flags = 0;
	       
		//mb_mapping->tab_input_registers[i * 2] = htons(input_registers.motions[i].value);
		//mb_mapping->tab_input_registers[i * 2 + 1] = htons(input_registers.motions[i].flags);
	}
	
	return mb_mapping;
}

static void mb_mapping_wrapper_free(modbus_mapping_t *mb_mapping) {

	modbus_mapping_free(mb_mapping);
}

static void mb_mapping_getstates(modbus_mapping_t *mb_mapping) {
	int i;

	for (i = 0; i < mb_mapping->nb_bits; i++) {
		mb_mapping->tab_bits[i] = io_bits[i] ? ON : OFF;
	}

	for (i = 0; i < MOTION_SENSORS_QUANTITY; i++) {
		mb_mapping->tab_input_registers[i * 2] = input_registers.motions[i].value;
		mb_mapping->tab_input_registers[i * 2 + 1] = input_registers.motions[i].flags;
	}
	for (i = MOTION_SENSORS_QUANTITY * 2; i < 2; i ++) {
		mb_mapping->tab_input_registers[MOTION_SENSORS_QUANTITY * 2 + i] = input_registers.pressures[i];
	}
	mb_mapping->tab_input_registers[MOTION_SENSORS_QUANTITY * 2 + 2 ] = input_registers.temp;
}

static void mb_mapping_setstates(modbus_mapping_t *mb_mapping) {
	io_bits[0] = mb_mapping->tab_bits[0];
}

int main(int argc, char*argv[]) {
	int listen_socket, client_socket;
	modbus_t *ctx;
	modbus_mapping_t *mb_mapping;
	uint8_t *query;
	const char *ip = "0.0.0.0";
	unsigned short port = 1502;
	int header_len;
	int opt;

	while (-1 != (opt = getopt(argc, argv, "a:p:"))) {
		switch (opt) {
		case 'a':
			ip = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		}
	}
	
	ctx = modbus_new_tcp(ip, port);
	header_len = modbus_get_header_length(ctx);
	query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);

	modbus_set_debug(ctx, TRUE);

	mb_mapping = mb_mapping_wrapper_new();
	if (mb_mapping == NULL) {
		fprintf(stderr, "Failed to allocate the mapping: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	listen_socket = modbus_tcp_listen(ctx, 1);
	for (;;) {
		client_socket = modbus_tcp_accept(ctx, &listen_socket);
		if (-1 == client_socket) {
			break;
		}

		for (;;) {
			int query_len;

			query_len = modbus_receive(ctx, query);
			if (-1 == query_len) {
				/* Connection closed by the client or error */
				break;
			}

			if (query[header_len - 1] != MODBUS_TCP_SLAVE) {
				continue;
			}

			mb_mapping_getstates(mb_mapping);

			if (-1 == modbus_reply(ctx, query, query_len, mb_mapping)) {
				break;
			}
	
			mb_mapping_setstates(mb_mapping);

		}

		close(client_socket);
	}
	printf("exiting: %s\n", modbus_strerror(errno));

	close(listen_socket);
	mb_mapping_wrapper_free(mb_mapping);
	free(query);
	modbus_free(ctx);

	return 0;
}
