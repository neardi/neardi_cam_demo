# Neardi AHD Camera Demo

目录主要包含如下内容:

``` shell
neardi_cam_demo$ tree
.
├── build.sh
├── main.cpp
├── queue.hpp
└── README.md
``` 

- build.sh: 编译脚本
- main.cpp: Demo源码
- queue.hpp: 线程安全队列

## 1. 适用平台

- `Neardi`平台`RK3399Pro`或`RK3588`带`AHD摄像头`的机型。
- `USB摄像头`或`MIPI摄像头`请自行修改源码适配。

## 2. 编译环境

``` shell
sudo apt update
sudo apt install build-essential libopencv-dev   
``` 

## 3. 下载Demo

``` shell
git clone https://gitee.com/ytlans/neardi_cam_demo
``` 

## 4. 编译Demo

``` shell
./build.sh
``` 

## 5. 编译结果

``` shell
neardi@LPA3588:~/neardi_cam_demo$ ls
build.sh  main.cpp  n4camera  queue.hpp
``` 

其中`n4camera`为编译生成的可执行文件。

## 6. 运行

``` shell
./n4camera 11
```

`11`为`AHD摄像头`对应的设备节点，例如`/dev/video11`。