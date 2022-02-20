#!/bin/bash

target_file=demo

# server_IP=192.168.0.84
# server_username=lodge

server_IP=192.168.2.108
server_username=pi

#make clean

make arm
scp ./$target_file $server_username@$server_IP:/home/$server_username/

