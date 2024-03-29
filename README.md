# LVGL

官网：<https://lvgl.io/features>

开发手册：<https://docs.lvgl.io/master/get-started/quick-overview.html>

汉化版：<http://lvgl.100ask.org/master/>

***

## 编译架构选择

可以在项目主目录下通过 make help 命令查看

![示例 make help](https://user-images.githubusercontent.com/26021085/154875133-7a493a11-b212-4eae-bc08-1b8075ac9ebd.png)


pc端模拟运行需要安装sdl库

    sudo apt update && sudo apt install -y build-essential libsdl2-dev

参考：

<https://github.com/lvgl/lv_sim_vscode_sdl>

## git 子模块

### 添加子模块
命令如下
    
    git submodule add <url> <path>

本次移植的是 lvgl8.2，在git主目录下输入以下命令，添加 lvgl、lv_drivers 作为子模块;

    git submodule add -b release/v8.2 https://github.com/lvgl/lvgl.git lvgl
    git submodule add -b release/v8.2 https://github.com/lvgl/lv_drivers.git lv_drivers

### 子模块使用

克隆项目后，默认子模块目录下无任何内容。需要在项目根目录执行以下命令完成子模块的下载

    git submodule update --init --recursive

或者使用 recursive 参数在克隆项目的时候循环克隆git子项目 

    git clone --recursive <git_url>

本仓库可使用 

    git clone --recursive git@github.com:EchoHeim/LVGL.git


参考:

<https://www.likecs.com/show-125655.html>

<https://blog.csdn.net/guotianqing/article/details/82391665>

## gcc 编译参数详解

<https://blog.csdn.net/qq_30011277/article/details/103711155>

## Makefile语法参考

逻辑或、逻辑与的变通实现

<https://blog.csdn.net/u010312436/article/details/52459609/>

***

## klipperscreen 界面移植

