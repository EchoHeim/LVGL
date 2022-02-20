#
# Makefile
#

#工程目录
LVGL_DIR ?= .

#目标文件名
BIN = demo

# 根据输入的编译目标选择相应的编译器，加入-include $(DEPENDS)的目的是为了能够自动识别出头文件的修改并进行跟随编译
ifeq ($(MAKECMDGOALS),)
# 如果make没有指定平台架构，默认是 arm 嵌入式平台
DEFAULT_ARCH=arm
# 定义编译的目标架构及最终的可执行文件名
TARGET_ARCH		 = $(DEFAULT_ARCH)
TARGET_OBJT		:= $(BIN)_$(DEFAULT_ARCH)
else
TARGET_ARCH		 = $(MAKECMDGOALS)
TARGET_OBJT		:= $(BIN)_$(MAKECMDGOALS)
endif

ifneq ($(findstring $(MAKECMDGOALS)$(DEFAULT_ARCH),pc pc),)
#-------------------------------------------------------------------------- ARCH-pc
CC = gcc
-include $(DEPENDS)

# 宏定义
DEFINES ?= -D PC_MONITOR
# 编译参数:
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/ $(DEFINES)

# 链接库的文件路径、头文件路径与要链接的库
LDFLAGS := -lSDL2 -lm

else ifneq ($(findstring $(MAKECMDGOALS)$(DEFAULT_ARCH),arm arm),)
#-------------------------------------------------------------------------- ARCH-arm
# define ON_Embedded
CC = arm-none-linux-gnueabihf-gcc
# CC = arm-none-eabi-gcc

-include $(DEPENDS)

DEFINES ?= -D ON_Embedded
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/ $(DEFINES)

# 链接库的文件路径、头文件路径与要链接的库
# LDFLAGS := -I /home/lgb/piLib/piInclude/WiringPi/wiringPi -L /home/lgb/piLib -lwiringPi -lSDL2 -lm

endif
#--------------------------------------------------------------------------END-------------

# 设置编译过程中的临时文件保存路径
BUILD_DIR		?= _build
OBJ_DIR			?= $(BUILD_DIR)/$(TARGET_ARCH)

#Collect the files to compile
include $(LVGL_DIR)/source.mk

OBJECTS = $(AOBJS) $(COBJS) $(MAINOBJ)
INCLUDES = $(ASRCS) $(CSRCS) $(SOURCES)

# 根据.c文件编译出同名的.d和.o文件
AOBJS			:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(filter %.S, $(ASRCS)))
COBJS			:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(filter %.c, $(CSRCS)))
MAINOBJ			:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(filter %.c, $(SOURCES)))
DEPENDS			:= $(patsubst %.c, $(OBJ_DIR)/%.d, $(filter %.c, $(INCLUDES)))

# 定义合法的编译对象, PHONY 强制让makefile执行指令操作;
.PHONY: pc arm useage help clean

# 定义生成目标的名称;
pc arm : mngdir $(TARGET_OBJT)

mngdir:
	@echo "\n-------------------------- Begin to bulid $(TARGET_OBJT) --------------------------\n"
	@mkdir -pv $(OBJ_DIR)
	@mkdir -pv $(BUILD_DIR)

#将生成的 .o .d 文件放到O_PATH目录下
$(OBJ_DIR)/%.o: %.c
	@mkdir -pv $(@D)
	@$(CC) -c $(CFLAGS) $< -o $@ 
	@echo "CC $<"

$(OBJ_DIR)/%.d: %.c
	@echo "$@: $<:$(notdir $*)"
	@mkdir -pv $(@D)
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,$(notdir $*)\.o[ :]*,$(OBJ_DIR)/$*\.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# 生成目标文件
$(TARGET_OBJT): $(OBJECTS) $(DEPENDS)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $(OBJECTS) $(LDFLAGS)
	@echo "\n-------------------------- Bulid $(TARGET_OBJT) complete! --------------------------\n"
	@cp $(BUILD_DIR)/$@ $(LVGL_DIR)/$(BIN)

useage help:
	@echo "\n           **** How to build? ****\n"
	@echo "    make pc		-- build target for pc"
	@echo "    make arm		-- build target for arm"
	@echo "    make 		-- build target for default (arm)"

clean:
	@echo "\n-------------------------- Begin to clean bulid files --------------------------\n"
	@rm -rfv $(BUILD_DIR) $(LVGL_DIR)/$(BIN)
	@echo "\n-------------------------- clean all files complete! --------------------------\n"

