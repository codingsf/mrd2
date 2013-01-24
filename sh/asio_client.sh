#!/bin/sh
path=$(pwd)/${1} 
paras="127.0.0.1 4001 2 2048 100 30"
echo "run asio_ppong_c cmd line = $paras"
$path../cmake/bin/asio_ppong_c $paras
