# PHP扩展开发介绍

PHP日志扩展已经有成熟的[sealog](https://github.com/SeasX/SeasLog)了，这个项目只是提供一个尝试的思路。

## 目录

- [阅读前提](#阅读前提)
- [场景介绍](#场景介绍)
- [日志组件的功能蓝图](#日志组件的功能蓝图)
- [配置开发环境](#配置开发环境)
- [编译安装](#编译安装)
- [基本工序](#基本工序)
- [利用PHP语言的能力](#利用PHP语言的能力)
- 使用外部库
- [使用Go语言实现](#使用Go语言实现)
- [参考文献](#参考文献)
  

### 阅读前提
我们假定读者有一定的C语言基础，并且对于PHP内核的实现有基础的了解。如果你想了解PHP底层实现的知识，[参考文献](#参考文献)部分提供了一些阅读资料。

### 场景介绍

作为动态语言，PHP在快速开发业务功能方面具有优势。底层的C扩展可以解决PHP的性能问题，并为PHP赋能。PHP扩展是PHP语言和操作系统环境交互的中间件，可以增强现有的PHP特性或者延展PHP未有的功能。
- 开发新的能力。比如，为PHP添加Go语言的协程能力，像swoole扩展。
- 添加中间层。比如，在不修改项目代码的前提下，为产品添加一些额外的功能。在java里面，一般是通过java agent进行字节注入。对于php来说，可以开发扩展来实现更为底层的控制（在请求的生命周期中做劫持和注入），而不必入侵到应用代码。
- 性能提升。在php-fpm模式下，每个请求进来PHP都会重新初始化一次文件句柄。如果我们通过PHP扩展将文件句柄持久化，一次性初始化，多次使用。这样复用了文件句柄也会带来一定的性能提升。

![PHP扩展示意图](https://raw.githubusercontent.com/panxl6/blog/master/Images/php-logger/php-archteture.jpg)


### 日志组件的功能蓝图
我们打日志，主要是留下软件运行过程中的一些快照信息，尤其是一些可能出错和关键路径的地方，以便定位问题的时候回溯。根据这个需求，我们规划一下这个扩展应有的一些功能：
- 版本1：将日志信息(字符串)写入文本；
- 版本2：自动获取软件运行的上下文信息；
- 版本3：提供更加丰富的日志写入方式，如写入数据库、发给消息队列、发送邮件等；
- 版本4：提供日志辅助处理工具；

版本1和版本2的功能类似于原始的[PHP Logger](https://github.com/katzgrau/KLogger)工具类的实现。我们把这一类的功能由PHP实现翻译为C语言实现。版本3和版本4的功能，偏向于Monolog这一类的重型的框架。通过配置化，聚合一些其他平台的能力。为了保持文章说明性的定位和缩减文章篇幅，我们只实现版本1和版本2的功能。


### 编译安装
本文以ubuntu 18.04、PHP 7.2为例。

1. 获取项目源码
`git clone https://github.com/panxl6/php-logger.git`

2. 项目目录说明
- utils: 构建和安装工具脚本
- ext: 扩展的实现逻辑
- demo: 演示扩展提供的功能

3. 运行utils/install.sh，安装PHP7.2开发环境。

4. 运行utils/build.sh,构建扩展。并在php.ini(/etc/php/7.2/cli/php.ini以及/etc/php/7.2/fpm/php.ini)中手动添加`extension=logger.so`
```bash
cd utils/
./build.sh
```

运行demo目录下的示例`php demo/1.hello_world.php`，即可看到输出。
至此，你已经完成了PHP扩展的Hello world开发。


### 基本工序

1. 实现一个logger函数;
2. 实现一个logger函数，向指定的文件写入日志信息(字符串);
3. 实现一个logger函数，通过ini配置文件指定日志文件路径，并向其中写入日志信息;
4. 实现一个Logger类;
5. 实现一个Logger类，并提供info、debug、warn、error四个接口;
6. 实现一个Logger类，并提供info、debug、warn、error四个接口。获取当前程序运行的上下文;

php-fpm启动过程中，会加载扩展。此过程提供了模块初始化和模块关闭两个钩子函数。除非重启php-fpm，否者这两个函数只会被调用一次。php-fpm运行期间，当接受到请求时，会执行请求初始化和请求关闭两个钩子函数。PHP通过将一个脚本的生命周期分为多个阶段，然后在不同阶段去劫持php脚本(请求)。拦截了脚本的执行权之后，php执行不同的回调函数，从而调用扩展的功能。Nginx的模块也是类似的思路。PHP的扩展实现逻辑框架提供了各类固定的钩子函数，我们只需要在钩子函数(扩展功能的入口)中调用我们的自定义逻辑即可。

![PHP脚本生命周期示意图](https://raw.githubusercontent.com/panxl6/blog/master/Images/php-logger/php-lifecycly.png)
> 图片引用来源：https://www.kancloud.cn/nickbai/php7/363265

**注意点**
1. 使用PHP源码中(php-src/ext目录下)提供的工具创建初始化目录：
```
./ext/ext_skel --extname=logger
```

2. 将ext/logger/config.m4中的编译选项取消注释
```
PHP_ARG_ENABLE(logger, whether to enable logger support,
dnl Make sure that the comment is aligned:
[  --enable-logger           Enable logger support])
```

**调试信息的要素**
- 当前时间;
- 文件信息;
- debugtrace信息(调用栈);
- $_SERVER信息(我们只讨论fpm模式，cli模式没有这个信息);

### 利用PHP语言的能力
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

#### Go语言实现PHP扩展的Hello world
在这里，我们提供两个开源项目以供参考:[kitech/php-go](https://github.com/kitech/php-go)、[write php extension in go](https://github.com/do-aki/gophp_sample)。

#### Benchmark
- [ ] 比较PHP扩展的方式实现，跟现有的主流日志框架之间的性能比较

### 参考文献

1: [phpinternals](https://phpinternals.net/)

2: [phpinternalsbook](http://www.phpinternalsbook.com/)

3: [php-internals-extended-development-course](https://github.com/huqinlou0123/php-internals-extended-development-course)
