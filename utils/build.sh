#!/bin/bash

cd ../ext/logger

phpize7.2
./configure --with-php-config=/usr/bin/php-config7.2

make clean
make
sudo make install
