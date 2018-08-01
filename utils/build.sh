#!/bin/bash

cd ../ext/logger

phpize7.0
./configure --with-php-config=/usr/bin/php-config7.0

make clean
make
sudo make install
