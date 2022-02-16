#
# 源码路径
#

LVGL_DIR_NAME = lvgl
LV_DRIVERS_DIR_NAME ?= lv_drivers

include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/lv_drivers.mk


INCLUDES	+= -Iinc/h

# SOURCES		+= ${wildcard *.c}
# SOURCES		+= ${wildcard src/*.c}

CSRCS		+= ./lv_drivers/sdl/sdl.c

SOURCES 	+= ./main.c
# SOURCES		+= ./resource/chinese.c
# SOURCES		+= ./resource/fontadpt.c
SOURCES		+= ./resource/mouse_cursor_icon.c

########################### klipper ###########################
# 指定编译过程中需要包含的头文件路径，以及要编译的源文件（下面的 klipper 目录和 klipper 目录分别存放头文件和源文件，可自定义修改和添加）
INCLUDES	+= -Iklipper/h
SOURCES		+= ${wildcard klipper/*.c}

