#!/bin/bash

sudo apt install nasm
sudo apt install gcc

cd src
make
sudo make install
