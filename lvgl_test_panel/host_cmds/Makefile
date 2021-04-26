
all : libmodbus led-server led-client

LIBMODBUS_VER = 3.1.6

ROOT_BASE = $(PWD)/..

BUILD_BASE = $(ROOT_BASE)/host_cmds/build

EMULATE_BASE = $(ROOT_BASE)/emulate

CMDS_BASE = $(ROOT_BASE)/cmds

LD_FLAGS_LIBMODBUS = -L$(BUILD_BASE)/libmodbus/lib -lmodbus

CFLAGS_LIBMODBUS_INCLUDES = -I$(BUILD_BASE)/libmodbus/include/modbus

libmodbus-$(LIBMODBUS_VER).tar.gz:
	wget http://libmodbus.org/releases/libmodbus-$(LIBMODBUS_VER).tar.gz

$(BUILD_BASE)/libmodbus/lib/pkgconfig/libmodbus.pc : libmodbus-$(LIBMODBUS_VER).tar.gz
	tar -xf libmodbus-$(LIBMODBUS_VER).tar.gz
	cd libmodbus-$(LIBMODBUS_VER); \
	./configure --prefix=$(BUILD_BASE)/libmodbus --enable-static --disable-shared; \
	make install; cd ..;

libmodbus: $(BUILD_BASE)/libmodbus/lib/pkgconfig/libmodbus.pc

led-server led-client : CFLAGS += -Wall -g3 -O0 $(CFLAGS_LIBMODBUS_INCLUDES)

$(EMULATE_BASE)/conf/leds.txt:
	mkdir -p $(EMULATE_BASE)/conf
	cp leds.txt $(EMULATE_BASE)/conf/leds.txt
	
led-client : libmodbus
	gcc $(CFLAGS) led-client.c -o $(EMULATE_BASE)/$@ $(LD_FLAGS_LIBMODBUS)

led-server : libmodbus $(EMULATE_BASE)/conf/leds.txt
led-server : modbus_server.c
	$(CC) $(CFLAGS) modbus_server.c -o $(EMULATE_BASE)/$@ $(LD_FLAGS_LIBMODBUS)

clean :
	rm -rf $(BUILD_BASE) $(EMULATE_BASE)

