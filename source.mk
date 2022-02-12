#
# 源码路径
#

LVGL_DIR_NAME = lvgl
LV_DRIVERS_DIR_NAME ?= lv_drivers

# 这里指定编译过程需要包含的头文件路径，以及要编译的源文件（下面的inc目录和src目录分别存放头文件和源文件，可自定义修改和添加）
# INCLUDES		+= -Iinc/h
# SOURCES			+= ${wildcard *.c}
# SOURCES			+= ${wildcard src/*.c}

include $(LVGL_DIR)/$(LVGL_DIR_NAME)/lvgl.mk
include $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/lv_drivers.mk

#Collect the files to compile
# SOURCES = ../main.c ../chinese.c ../fontadpt.c ../ui.c
SOURCES += ./main.c ./chinese.c ./fontadpt.c

