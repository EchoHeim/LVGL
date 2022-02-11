# all:
# 	make -C object

# pc:
# 	make pc -C object

# mp157:
# 	make mp157 -C object

# clean:
# 	make clean -C object

#
# Makefile
#
#编译工具链
# GCC = gcc
# CC = arm-none-linux-gnueabihf-gcc

ifeq ($(MAKECMDGOALS),pc)

CC = gcc
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/

else ifeq ($(MAKECMDGOALS),mp157)

# define ON_Embedded
CC = arm-none-linux-gnueabihf-gcc
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/ -D ON_Embedded
# CFLAGS ?= -Wall -Wshadow -Wundef -g -I$(LVGL_DIR)/ -D ON_Embedded

else 	# default
CC = arm-none-linux-gnueabihf-gcc
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/ -D ON_Embedded

endif

#目标文件名
BIN = demo
# 这里定义出所编译的目标架构及最终的可执行文件名
TARGET_ARCH		 = $(MAKECMDGOALS)
TARGET_OBJT		:= demo_$(MAKECMDGOALS)


# 这里设置好编译过程中的临时目录名称
BUILDDIR		?= _build
OBJCTDIR		?= $(BUILDDIR)/$(TARGET_ARCH)

LVGL_DIR_NAME = lvgl
LV_DRIVERS_DIR_NAME ?= lv_drivers

#源文件路径
LVGL_DIR ?= ${shell pwd}/

#链接库的文件路径、头文件路径与要链接的库
# PI_LIB_PATH := -I /home/lgb/piLib/piInclude/WiringPi/wiringPi -L /home/lgb/piLib -lwiringPi


#LDFLAGS ?= -lSDL2 -lm


#.o文件存放路径
O_PATH = $(LVGL_DIR)/object

include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/lv_drivers.mk

#Collect the files to compile
# MAINSRC = ../main.c ../chinese.c ../fontadpt.c ../ui.c
MAINSRC = ../main.c ../chinese.c ../fontadpt.c

# CSRCS +=lvgl-new/demos/benchmark/lv_demo_benchmark.c

AOBJS = $(ASRCS:.S=.o)
COBJS = $(CSRCS:.c=.o)
MAINOBJ = $(MAINSRC:.c=.o)

# AOBJS := $(addprefix $(O_PATH)/,$(AOBJS))		# 连接路径;
# COBJS := $(addprefix $(O_PATH)/,$(COBJS))
# MAINOBJ := $(addprefix $(O_PATH)/,$(MAINOBJ))


SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS) $(MAINOBJ)

#将生成的.o文件放到O_PATH目录下
%.o: %.c
	@$(CC) -c $(CFLAGS) $(PI_LIB_PATH) $< -o $@
	@echo "CC $<"


# 这里定义出合法的编译对象
.PHONY: pc mp157 useage help clean

pc mp157 : mngdir $(TARGET_OBJT)

mngdir:
	@echo "-------------------------------------------------- Begin to bulid $(TARGET_OBJT)"
	@mkdir -pv $(OBJCTDIR)
	@mkdir -pv $(BUILDDIR)
	
## MAINOBJ -> OBJFILES
all: default

# 生成目标文件    

$(TARGET_OBJT): $(OBJECTS) $(DEPENDS)
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $(OBJECTS) $(LDFLAGS)
	@echo "-------------------------------------------------- Bulid $(TARGET_OBJT) complete!"


default: $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(PI_LIB_PATH) 

mp157: $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(PI_LIB_PATH) 

pc: $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(PI_LIB_PATH)

test: benchmark.o
	$(CC) -o benchmark $(AOBJS) $(COBJS) $(LDFLAGS)

clean: 
	rm -f $(BIN) $(OBJS) 

# #获取源文件路径下的所有.o文件名
# GCC_SOURCE_PATH = $(foreach dir,$(SUBDIR),$(wildcard $(dir)/*.c))
# GCC_OBJ_PATH = $(patsubst %.c,$(O_PATH)/%.o, $(notdir $(GCC_SOURCE_PATH)))

