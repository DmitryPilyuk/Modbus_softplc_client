
all : libmodbus client

LIBMODBUS_VER = 3.1.11

ROOT_BASE = $(PWD)

BUILD_BASE = $(ROOT_BASE)/build

LD_FLAGS_LIBMODBUS = -L$(BUILD_BASE)/libmodbus/lib -lmodbus -lm

CFLAGS_LIBMODBUS_INCLUDES = -I$(BUILD_BASE)/libmodbus/include/modbus

CFLAGS = -Wall -g3 -O0 $(CFLAGS_LIBMODBUS_INCLUDES)

v$(LIBMODBUS_VER).tar.gz:
	wget https://github.com/stephane/libmodbus/archive/refs/tags/v$(LIBMODBUS_VER).tar.gz

$(BUILD_BASE)/libmodbus/lib/pkgconfig/libmodbus.pc : v$(LIBMODBUS_VER).tar.gz
	tar -xvf v$(LIBMODBUS_VER).tar.gz
	cd libmodbus-$(LIBMODBUS_VER); \
	./autogen.sh; \
	./configure --prefix=$(BUILD_BASE)/libmodbus --enable-static --disable-shared; \
	make install; cd ..;

libmodbus: $(BUILD_BASE)/libmodbus/lib/pkgconfig/libmodbus.pc
	
client: libmodbus client.c
	gcc $(CFLAGS) client.c -o $@ $(LD_FLAGS_LIBMODBUS)

clean:
	rm -rf $(BUILD_BASE)
	rm client
