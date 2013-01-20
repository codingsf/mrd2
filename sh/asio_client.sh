#!/bin/sh
path=$(pwd)/${1} 
$path../cmake/bin/asio_ppong_c 127.0.0.1 4001 2 2048 100 30
