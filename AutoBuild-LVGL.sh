#!/bin/bash

target_file=demo
server_IP=192.168.0.84
server_username=lodge

#make clean

make mp157
scp ./$target_file $server_username@$server_IP:/home/$server_username/

