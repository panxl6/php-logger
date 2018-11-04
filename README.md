# PHP扩展开发介绍

## 目录

- 阅读前提
- 场景介绍
- 日志组件的功能蓝图
- 配置开发环境
- Hello world
- 基本工序
- 利用PHP语言的能力
- 使用外部库
- 使用Go语言实现
  

### 阅读前提
我们假定读者有一定的C语言基础，并且对于PHP内核的实现有基础的了解。

### 场景介绍

- 获得更加底层的控制权
- 提升性能

PHP扩展作为PHP语言和操作系统环境交互的中间件，可以增强现有的或者延展PHP未有的功能。

比如，为PHP添加Go语言的协程能力，像swoole扩展。

比如，在不修改项目代码的前提下，为产品添加一些额外的功能。在java里面，一般是通过java agent进行字节注入。对于php来说，可以开发扩展来实现更为底层的控制，而不必入侵到应用代码。

### 日志组件的功能蓝图
我们打日志，主要是留下软件运行过程中的一些快照信息，尤其是一些可能出错和关键路径的地方，以便定位问题的时候回溯。
- 版本1：将日志信息(字符串)写入文本；
- 版本2：自动获取软件运行的上下文信息；
- 版本3：提供更加丰富的日志写入方式，如写入数据库、发给消息队列、发送邮件等。
- 版本4：提供日志辅助处理工具。

版本1和版本2的功能类似于原始的[PHP Logger](https://github.com/katzgrau/KLogger)工具类的实现。我们把这一类的功能由PHP实现翻译为C语言实现。

版本3和版本4的功能，偏向于Monolog这一类的重型的框架。

为了保持文章说明性的定位和缩减文章篇幅，我们只实现版本1和版本2的功能。


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


### 基本工序
- 定义函数
- 定义类
- 定义类方法
- 定义类变量
- 定义全局变量
- 使用php.ini配置文件


1. 实现一个logger函数;
2. 实现一个logger函数，向指定的文件写入日志信息(字符串);
3. 实现一个logger函数，通过ini配置文件指定日志文件路径，并向其中写入日志信息;
4. 实现一个Logger类;
5. 实现一个Logger类，并提供info、debug、warn、error四个接口;
6. 实现一个Logger类，并提供info、debug、warn、error四个接口。获取当前程序运行的上下文;


**调试信息的要素**
- 当前时间;
- 文件信息;
- debugtrace信息(调用栈);
- $_SERVER信息(我们只讨论Request环境，cli模式没有这个信息);

### 利用PHP的能力
- 使用PHP语言的特性
- 使用PHP内核提供的API(php_stream)
- 实现持久连接


### 使用Go语言实现
- 基本思路
- 实现Hello world

#### 基本思路
整个流程走下来，我们发现使用C语言调用原始的zend API开发效率比较低。

对于一些定制化程度要求不太高的项目，我们可以使用有一些php扩展开发框架来加速开发。例如,[swoole衍生项目phpx](https://github.com/swoole/phpx)。但是这一类的项目对于底层的API支持程度并不丰富，目前都不太适合用于复杂的生产环境。

另一个思路是使用go语言来开发php扩展。go语言可以使用原生的c语言代码。go语言可以打包成`.so`共享文件，我们的PHP扩展正是`.so`文件，这样以来，go语言的能力和php的能力就可以联系在一起了。具体思路可以参考这篇文章[building python modules with go](https://blog.filippo.io/building-python-modules-with-go-1-5/)

同样的，go语言开发php扩展的社区支持也并不丰富，而且绕一个大弯来使用PHP，还不如直接使用go语言开发。

#### 实现Hello world
在这里，我们提供两个开源项目以供参考:[kitech/php-go](https://github.com/kitech/php-go)、[write php extension in go](https://github.com/do-aki/gophp_sample)。

### 参考文献

1: [phpinternals](https://phpinternals.net/)

2: [phpinternalsbook](http://www.phpinternalsbook.com/)

3: [php-internals-extended-development-course](https://github.com/huqinlou0123/php-internals-extended-development-course)
