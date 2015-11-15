LIB_DIR = ../STM32Cube_FW_F4_V1.9.0

DRIVER = $(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Src

LWIP = $(LIB_DIR)/Middlewares/Third_Party/LwIP/src

BUILD_DIR = build


#Add any source files that you use here.  Remember to end them with .o
OBJECTS = Src/main.o \
		 Src/stm32f4xx_it.o \
		#Src/system_stm32f4xx.o \
		Src/app_ethernet.o \
		Src/ethernetif.o \
		Src/udp_echoserver.o




OBJECTS += \
       $(DRIVER)/stm32f4xx_hal.o \
       $(DRIVER)/stm32f4xx_hal_crc.o \
       $(DRIVER)/stm32f4xx_hal_can.o \
       $(DRIVER)/stm32f4xx_hal_uart.o \
       $(DRIVER)/stm32f4xx_hal_usart.o \
       $(DRIVER)/stm32f4xx_hal_cortex.o \
       $(DRIVER)/stm32f4xx_hal_gpio.o \
       $(DRIVER)/stm32f4xx_hal_rcc.o \
       $(DRIVER)/stm32f4xx_hal_dma.o \
       $(DRIVER)/stm32f4xx_hal_spi.o \
       $(DRIVER)/stm32f4xx_hal_i2c.o \
       $(DRIVER)/stm32f4xx_hal_i2c_ex.o \
       $(DRIVER)/stm32f4xx_hal_i2s.o \
       $(DRIVER)/stm32f4xx_hal_i2s_ex.o \
        $(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o \
        $(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.o \
        $(LWIP)/netif/etharp.o \
        $(LWIP)/netif/slipif.o \
        $(LWIP)/netif/ppp/auth.o \
        $(LWIP)/netif/ppp/chap.o \
        $(LWIP)/netif/ppp/chpms.o \
        $(LWIP)/netif/ppp/fsm.o \
        $(LWIP)/netif/ppp/ipcp.o \
	$(LWIP)/netif/ppp/lcp.o \
	$(LWIP)/netif/ppp/magic.o \
	$(LWIP)/netif/ppp/md5.o \
	$(LWIP)/netif/ppp/pap.o \
	$(LWIP)/netif/ppp/ppp.o \
	$(LWIP)/netif/ppp/ppp_oe.o \
	$(LWIP)/netif/ppp/randm.o \
	$(LWIP)/netif/ppp/vj.o \
	$(LWIP)/api/api_lib.o \
	$(LWIP)/api/api_msg.o \
	$(LWIP)/api/err.o \
	$(LWIP)/api/netbuf.o \
	$(LWIP)/api/netdb.o \
	$(LWIP)/api/netifapi.o \
	$(LWIP)/api/sockets.o \
	$(LWIP)/api/tcpip.o \
	$(LWIP)/core/def.o \
	$(LWIP)/core/dhcp.o \
	$(LWIP)/core/dns.o \
	$(LWIP)/core/init.o \
	$(LWIP)/core/lwip_timers.o \
	$(LWIP)/core/mem.o \
	$(LWIP)/core/memp.o \
	$(LWIP)/core/netif.o \
	$(LWIP)/core/pbuf.o \
	$(LWIP)/core/raw.o \
	$(LWIP)/core/stats.o \
	$(LWIP)/core/sys.o \
	$(LWIP)/core/tcp.o \
	$(LWIP)/core/tcp_in.o \
	$(LWIP)/core/tcp_out.o \
	$(LWIP)/core/udp.o \
	$(LWIP)/core/snmp/asn1_dec.o \
	$(LWIP)/core/snmp/asn1_enc.o \
	$(LWIP)/core/snmp/mib2.o \
	$(LWIP)/core/snmp/mib_structs.o \
	$(LWIP)/core/snmp/msg_in.o \
	$(LWIP)/core/snmp/msg_out.o \
	$(LWIP)/core/ipv4/autoip.o \
	$(LWIP)/core/ipv4/icmp.o \
	$(LWIP)/core/ipv4/igmp.o \
	$(LWIP)/core/ipv4/inet.o \
	$(LWIP)/core/ipv4/inet_chksum.o \
	$(LWIP)/core/ipv4/ip.o \
	$(LWIP)/core/ipv4/ip_addr.o \
	$(LWIP)/core/ipv4/ip_frag.o \
	$(LWIP)/netif/etharp.o \
	$(LWIP)/netif/slipif.o \
	$(LWIP)/netif/ppp/auth.o \
	$(LWIP)/netif/ppp/chap.o \
	$(LWIP)/netif/ppp/chpms.o \
	$(LWIP)/netif/ppp/fsm.o \
	$(LWIP)/netif/ppp/ipcp.o \
	$(LWIP)/netif/ppp/lcp.o \
	$(LWIP)/netif/ppp/magic.o \
	$(LWIP)/netif/ppp/md5.o \
	$(LWIP)/netif/ppp/pap.o \
	$(LWIP)/netif/ppp/ppp.o \
	$(LWIP)/netif/ppp/ppp_oe.o \
	$(LWIP)/netif/ppp/randm.o \
	$(LWIP)/netif/ppp/vj.o \
	$(LWIP)/api/api_lib.o \
	$(LWIP)/api/api_msg.o \
	$(LWIP)/api/err.o \
	$(LWIP)/api/netbuf.o \
	$(LWIP)/api/netdb.o \
	$(LWIP)/api/netifapi.o \
	$(LWIP)/api/sockets.o \
	$(LWIP)/api/tcpip.o \
    	$(LWIP)/core/def.o \
    	$(LWIP)/core/dhcp.o \
     	$(LWIP)/core/dns.o \
      	$(LWIP)/core/init.o \
       	$(LWIP)/core/lwip_timers.o \
       	$(LWIP)/core/mem.o \
       	$(LWIP)/core/memp.o \
       	$(LWIP)/core/netif.o \
        $(LWIP)/core/pbuf.o \
        $(LWIP)/core/raw.o \
        $(LWIP)/core/stats.o \
        $(LWIP)/core/sys.o \
        $(LWIP)/core/tcp.o \
        $(LWIP)/core/tcp_in.o \
        $(LWIP)/core/tcp_out.o \
        $(LWIP)/core/udp.o \
        $(LWIP)/core/snmp/asn1_dec.o \
        $(LWIP)/core/snmp/asn1_enc.o \
        $(LWIP)/core/snmp/mib2.o \
        $(LWIP)/core/snmp/mib_structs.o \
        $(LWIP)/core/snmp/msg_in.o \
        $(LWIP)/core/snmp/msg_out.o \
        $(LWIP)/core/ipv4/autoip.o \
        $(LWIP)/core/ipv4/icmp.o \
        $(LWIP)/core/ipv4/igmp.o \
        $(LWIP)/core/ipv4/inet.o \
        $(LWIP)/core/ipv4/inet_chksum.o \
        $(LWIP)/core/ipv4/ip.o \
        $(LWIP)/core/ipv4/ip_addr.o \
        $(LWIP)/core/ipv4/ip_frag.o \
        $(LIB_DIR)/Drivers/BSP/STM32F4-Discovery/stm32f4_discovery.o \
        /Users/stewartb2/ROV/X8-MicroController/STM32Cube_FW_F4_V1.9.0/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.o 






INCLUDE_PATHS = \
	-I./inc \
	-I./Inc \
    -I./$(LIB_DIR)/Inc \
    -I./$(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc \
    -I./$(LIB_DIR)/Drivers/CMSIS/Include \
    -I./$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
    -I./$(LWIP)/include \
    -I./$(LWIP)/include/lwip \
    -I./$(LWIP)/include/ipv4 \
    -I./$(LWIP)/include/ipv6 \
    -I./$(LIB_DIR)/Middlewares/Third_party/LwIP/test/unit \
    -I./$(LIB_DIR)/Middlewares/Third_party/LwIP/system \
    -I./$(LIB_DIR)/Middlewares/Third_Party/LwIP/system \
    -I./$(LWIP)/include \
    -I./$(LWIP)/include/ipv4 \
    -I./$(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc \
    -I./$(LIB_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
    -I./$(LIB_DIR)/Middlewares/Third_Party/LwIP/system/arch \
    -I./$(LWIP)/include/ipv4/lwip \
    -I./$(LWIP)/include/lwip \
    -I./$(LWIP)/include/netif \
    -I./$(LWIP)/include/posix \
    -I./$(LWIP)/include/posix/sys \
    -I./$(LWIP)/netif/ppp \
    -I./Src \
    -I./src \
    -I./$(LIB_DIR)/Drivers/BSP/STM32F4-Discovery


LINKER_SCRIPT = ./$(LIB_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/STM32F407XG.ld


#directory where the binary files go
BUILD_DIR = build

#name of the project. This name gets used to generate the .o, .bin, and .hex files
PROJECT = main

#these are the variables that will store the .o and .cpp files of the libraries that you create

SOURCE = source

LIB_SRC = $(wildcard $(SOURCE)/*.cpp)

LIB_OBJ = $(LIB_SRC:$(SOURCE)/%.cpp=$(BUILD_DIR)/%.o)


###############################################################################
AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
OBJDUMP = $(GCC_BIN)arm-none-eabi-objdump
SIZE    = $(GCC_BIN)arm-none-eabi-size

#compiling flags needed by the boards
CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -Wall -Wextra -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP -fdiagnostics-color=auto
CC_SYMBOLS = \
        -DSTM32F407xx           \
        -DSTM32F4               \
        -DSTM32F407VG

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs -u _printf_float -u _scanf_float -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

CC_FLAGS += -DNDEBUG -Os

#the actual portion of the make file

.PHONY: all compile clean burn dfu

all: compile

compile: $(PROJECT).bin $(PROJECT).hex
	@echo " ### Compiled $(PROJECT).bin & $(PROJECT).hex"

.c.o:
	@$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	@$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<


$(LIB_OBJ): $(LIB_SRC)
	@$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS)  $(LIB_SRC)
	@mv *.o ./$(BUILD_DIR)

$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	@$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT)  -o $(BUILD_DIR)/$@ $^ $(LIB_OBJ) $(LD_SYS_LIBS) $(LD_SYS_LIBS)
	@rm *.o

$(PROJECT).bin: $(PROJECT).elf
	@rm *.d
	@$(OBJCOPY) -O binary $(BUILD_DIR)/$< $(BUILD_DIR)/$@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -g -O ihex $(BUILD_DIR)/$< $(BUILD_DIR)/$@


clean:
	@rm $(BUILD_DIR)/*
	@echo " ### Cleaned build directory"

burn:
	@echo " ### Flashing $(BUILD_DIR)/main.bin to device with st-flash...\n"
	@st-flash --reset write $(BUILD_DIR)/main.bin 0x8000000

dfu:
	@echo " ### Flashing $(BUILD_DIR)/$(PROJECT).bin to device with dfu-util...\n"
	@dfu-util -a 0 --dfuse-address 0x08000000 -D $(BUILD_DIR)/$(PROJECT).bin
