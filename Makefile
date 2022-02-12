#
# Makefile
#

#工程目录
LVGL_DIR ?= .

# 根据输入的编译目标选择相应的编译器，加入-include $(DEPENDS)的目的是为了能够自动识别出头文件的修改并进行跟随编译
ifeq ($(MAKECMDGOALS),pc)
#-------------------------------------------------------------------------- pc
CC = gcc
-include $(DEPENDS)
# 编译参数：
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/

# 链接库的文件路径、头文件路径与要链接的库
# LDFLAGS := -I /home/lgb/piLib/piInclude/WiringPi/wiringPi -L /home/lgb/piLib -lwiringPi -lSDL2 -lm

else ifeq ($(MAKECMDGOALS),mp157)
#-------------------------------------------------------------------------- mp157
# define ON_Embedded
CC = arm-none-linux-gnueabihf-gcc
-include $(DEPENDS)
CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/ -D ON_Embedded

else
#--------------------------------------------------------------------------	default (mp157)
CC = arm-none-linux-gnueabihf-gcc
-include $(DEPENDS)

CFLAGS ?= -Wall -Wshadow -Wundef -O3 -g0 -I$(LVGL_DIR)/ -D ON_Embedded

endif
#--------------------------------------------------------------------------END-----

#目标文件名
BIN = demo

# 定义编译的目标架构及最终的可执行文件名
TARGET_ARCH		 = $(MAKECMDGOALS)
TARGET_OBJT		:= $(BIN)_$(MAKECMDGOALS)

# 设置编译过程中的临时文件保存路径
BUILD_DIR		?= _build
OBJ_DIR			?= $(BUILD_DIR)/$(TARGET_ARCH)

include $(LVGL_DIR)/source.mk

OBJECTS = $(AOBJS) $(COBJS) $(MAINOBJ)
INCLUDES = $(ASRCS) $(CSRCS) $(SOURCES)

# 根据.c文件编译出同名的.d和.o文件
AOBJS			:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(filter %.S, $(ASRCS)))
COBJS			:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(filter %.c, $(CSRCS)))
MAINOBJ			:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(filter %.c, $(SOURCES)))
DEPENDS			:= $(patsubst %.c, $(OBJ_DIR)/%.d, $(filter %.c, $(INCLUDES)))

# 定义合法的编译对象, PHONY 强制让makefile执行指令操作;
.PHONY: pc mp157 useage help clean

# 定义生成目标的名称;
pc mp157 : mngdir $(TARGET_OBJT)

mngdir:
	@echo "-------------------------------------------------- Begin to bulid $(TARGET_OBJT)"
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
	@echo "-------------------------------------------------- Bulid $(TARGET_OBJT) complete!"
	@cp $(BUILD_DIR)/$@ $(LVGL_DIR)/$(BIN)

useage help:
	@echo "\n           **** How to build? ****\n"
	@echo "    make pc		-- build target for pc"
	@echo "    make mp157		-- build target for mp157"
	@echo "    make 		-- build target for default (mp157)"

clean:
	@echo "-------------------------------------------------- Begin to clean bulid files"
	@rm -rfv $(BUILD_DIR) $(LVGL_DIR)/$(BIN)
	@echo "-------------------------------------------------- clean all files complete!"

