#!/bin/sh
path=$(pwd)/${1} 
paras="127.0.0.1 4001 2 2048"
echo "run asio_ppong_s cmd line = $paras"
$path../cmake/bin/asio_ppong_s $paras
