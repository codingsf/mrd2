#!/bin/sh
path=$(pwd)/${1} 
echo "run echo_srv_mt"
$path../cmake/bin/echo_srv_mt
