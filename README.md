# PHP扩展开发介绍

## 目录

- 场景介绍
- 配置开发环境
- Hello world
- 基本功能
- 利用PHP语言的能力
- 使用外部库
- 使用Go语言实现
  

### 场景介绍

- 获得更加底层的控制权
- 提升性能

	PHP扩展作为PHP语言和操作系统环境交互的中间件，可以增强现有的或者延展PHP未有的功能。
	比如，为PHP添加Go语言的协程能力，像swoole扩展。
	比如，在不修改项目代码的前提下，为产品添加一些额外的功能。在java里面，一般是通过java agent进行字节注入。对于php来说，可以开发扩展来实现更为底层的控制，而不必入侵到应用代码。

### 配置开发环境
本文以ubuntu 18.04、PHP 7.0为例。

运行utils/install.sh，安装PHP7.0开发环境：
```bash
sudo add-apt-repository ppa:ondrej/php
sudo apt-get update

sudo apt-get install php7.0 php7.0-dev
```

### Hello world
1. 获取项目源码
`git clone https://github.com/panxl6/php-logger.git`

2. 项目目录说明
- utils: 构建和安装工具脚本
- ext: 扩展的实现逻辑
- demo: 演示扩展提供的功能

3. 使用PHP源码中(php-src/ext目录下)提供的工具创建初始化目录：
```
./ext/ext_skel --extname=logger
```

4. 将ext/logger/config.m4中的编译选项取消注释
```
PHP_ARG_ENABLE(logger, whether to enable logger support,
dnl Make sure that the comment is aligned:
[  --enable-logger           Enable logger support])
```

5. 运行utils/build.sh,构建扩展。并在php.ini(/etc/php/7.0/cli/php.ini以及/etc/php/7.0/fpm/php.ini)中添加`extension=logger.so`

运行demo目录下的示例`php demo/1.hello_world.php`，即可看到输出。
至此，你已经完成了PHP扩展的Hello world开发。


### 基本功能
- 定义函数
- 定义类
- 定义类方法
- 定义类变量
- 定义全局变量
- 使用php.ini配置文件

### 利用PHP的能力
- 使用PHP语言的特性
- 使用PHP内核提供的API(php_stream)
- 实现持久连接


### 使用Go语言实现
- 实现Hello world
- 基本思路


### 参考文献

[1]: [https://phpinternals.net/](https://phpinternals.net/)
[2]: [http://www.phpinternalsbook.com/](http://www.phpinternalsbook.com/)
[3]: [https://github.com/huqinlou0123/php-internals-extended-development-course](https://github.com/huqinlou0123/php-internals-extended-development-course)
