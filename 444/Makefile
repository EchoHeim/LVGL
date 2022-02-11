
# 强制用bash，有的系统/bin/sh默认是用dash的，用dash会导致echo -e显示时多出一个-e
SHELL = /bin/bash

# 这里先根据用户输入的编译目标选择好相应的编译器，加入-include $(DEPENDS)的目的是为了能够自动识别出头文件的修改并进行跟随编译
ifeq ($(MAKECMDGOALS),pc)
	COMPILE	= gcc
	-include $(DEPENDS)
else ifeq ($(MAKECMDGOALS),imx6)
	COMPILE = /opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a7
	-include $(DEPENDS)
else ifeq ($(MAKECMDGOALS),2440)
	COMPILE	= arm-linux-gcc
	-include $(DEPENDS)
endif

# 这里定义出所编译的目标架构及最终的可执行文件名
TARGET_ARCH		 = $(MAKECMDGOALS)
TARGET_OBJT		:= test_$(MAKECMDGOALS)

# 这里定义出编译的选项及配置
CFLAGS			+= $(INCLUDES) -rdynamic -Wall -O2 -Wno-uninitialized
LDFLAGS			+= -lpthread

# 这里指定编译过程需要包含的头文件路径，以及要编译的源文件（下面的inc目录和src目录分别存放头文件和源文件，可自定义修改和添加）
INCLUDES		+= -Iinc/h
SOURCES			+= ${wildcard *.c}
SOURCES			+= ${wildcard src/*.c}

# 这里设置好编译过程中的临时目录名称
BUILDDIR		?= _build
OBJCTDIR		?= $(BUILDDIR)/$(TARGET_ARCH)

# 这里定义好编译的规则：即根据.c文件编译出同名的.d和.o文件
OBJECTS			:= $(patsubst %.c, $(OBJCTDIR)/%.o, $(filter %.c, $(SOURCES)))
DEPENDS			:= $(patsubst %.c, $(OBJCTDIR)/%.d, $(filter %.c, $(SOURCES)))

# 这里定义出合法的编译对象
.PHONY: pc imx6 2440 useage help clean

pc imx6 2440: mngdir $(TARGET_OBJT)

mngdir:
	@echo "-------------------------------------------------- Begin to bulid $(TARGET_OBJT)"
	@mkdir -pv $(OBJCTDIR)
	@mkdir -pv $(BUILDDIR)

$(OBJCTDIR)/%.o: %.c
	@mkdir -pv $(@D)
	$(COMPILE) $(CFLAGS) -c -o $@ $<

$(OBJCTDIR)/%.d: %.c
	@echo "$@: $<:$(notdir $*)"
	@mkdir -pv $(@D)
	@set -e; rm -f $@; \
	$(COMPILE) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,$(notdir $*)\.o[ :]*,$(OBJCTDIR)/$*\.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(TARGET_OBJT): $(OBJECTS) $(DEPENDS)
	$(COMPILE) $(CFLAGS) -o $(BUILDDIR)/$@ $(OBJECTS) $(LDFLAGS)
	@echo "-------------------------------------------------- Bulid $(TARGET_OBJT) complete!"

useage help:
	@echo "How to build"
	@echo "    make pc		-- build target for pc"
	@echo "    make imx6		-- build target for imx6"
	@echo "    make 2440		-- build target for mini2440"

clean:
	@echo "-------------------------------------------------- Begin to clean bulid files"
	-@rm -rfv $(BUILDDIR)
	@echo "-------------------------------------------------- clean all files complete!"

