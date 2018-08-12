
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_logger.h"

/* If you declare any globals in php_logger.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(logger)
*/

/* True global resources - no need for thread safety here */
static int le_logger;

// 全局的类入口
zend_class_entry* logger_ce;

PHP_INI_BEGIN()
	PHP_INI_ENTRY("logger.dir","/var/log/",PHP_INI_ALL,NULL)
PHP_INI_END()


/* Uncomment this function if you have INI entries
static void php_logger_init_globals(zend_logger_globals *logger_globals)
{
	logger_globals->global_value = 0;
	logger_globals->global_string = NULL;
}
*/

/*------------------定义Logger类的成员方法BEG---------------------*/
// 构造函数
ZEND_METHOD(Logger, __construct)
{
	php_printf("__construct");
}

// 析构函数
ZEND_METHOD(Logger, __destruct)
{
	php_printf("__destruct");
}

// 调试信息
ZEND_METHOD(Logger, info)
{
	zend_string *data;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &data) == FAILURE)
		RETURN_FALSE;

	php_printf("debug");
	php_printf("%s\n", ZSTR_VAL(data));
	
	// 日志文件的完整路径
	char *path = "/home/panxl/log/20180812.log";

	FILE *fp;
	fp = fopen(path, "a+");

	fwrite(ZSTR_VAL(data), sizeof(char),data->len, fp);
	fclose(fp);
}

// 告警信息
ZEND_METHOD(Logger, warn)
{
	php_printf("warning");
}

// 业务错误信息
ZEND_METHOD(Logger, err)
{
	php_printf("warning");
}

// 语法错误，系统不可用 
ZEND_METHOD(Logger, fatal)
{
	php_printf("fatal");
}

// 获取调试信息(保护方法)
ZEND_METHOD(Logger, get_context)
{
	// 日志的级别
	// 时间戳
	// 主机名称(ip)
	// 进程id
	// HTTP:url,请求方法
	// 文件名,行号
	// 当前内存使用量
	
	php_printf("get_context");
}

// 将日志信息写入到文件(保护方法)
ZEND_METHOD(Logger, write)
{
	php_printf("write");
}

// 将日志信息格式化(保护方法) 
ZEND_METHOD(Logger, format)
{
	php_printf("format");
}


/*------------------定义Logger类的成员方法END---------------------*/



const zend_function_entry logger_functions[] = {
	PHP_ME(Logger, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Logger, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)

	// 外部接口
	PHP_ME(Logger, info, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Logger, warn, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Logger, err, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Logger, fatal, NULL, ZEND_ACC_PUBLIC)

	// 内部实现
	PHP_ME(Logger, write, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Logger, get_context, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Logger, format, NULL, ZEND_ACC_PROTECTED)

	PHP_FE_END	/* Must be the last line in logger_functions[] */
};


PHP_MINIT_FUNCTION(logger)
{
	// 注册Logger类
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Logger", logger_functions);
	logger_ce = zend_register_internal_class(&ce);

	// 声明类常量:日志的影响级别
	zend_declare_class_constant_long(logger_ce, "INFO", strlen("INFO"), 0);
	zend_declare_class_constant_long(logger_ce, "WARN", strlen("WARN"), 1);
	zend_declare_class_constant_long(logger_ce, "ERR", strlen("ERR"), 2);
	zend_declare_class_constant_long(logger_ce, "FATAL", strlen("FATAL"), 3);

	// 声明类变量
	zend_declare_property_string(logger_ce, "fileName", strlen("fileName"),"temp.log",ZEND_ACC_PROTECTED);

	REGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(logger)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(logger)
{
	// Request信息
	zend_is_auto_global_str("_SERVER", strlen("_SERVER"));
	zval *server = zend_hash_str_find(&EG(symbol_table), "_SERVER", strlen("_SERVER"));

	zval func_name;
	ZVAL_STR(&func_name, zend_string_init("var_dump", strlen("var_dump"),0));
	
	zval retval;
	if (call_user_function(EG(function_table), NULL, &func_name, &retval,1,server) == SUCCESS)
		php_printf("调用成功");
	else
		php_printf("var_dump调用失败");

	php_printf("初始化请求信息\n");

	zval_ptr_dtor(&func_name);
	zval_ptr_dtor(&retval);

#if defined(COMPILE_DL_LOGGER) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(logger)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(logger)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "logger support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}


zend_module_entry logger_module_entry = {
	STANDARD_MODULE_HEADER,
	"logger",
	logger_functions,
	PHP_MINIT(logger),
	PHP_MSHUTDOWN(logger),
	PHP_RINIT(logger),
	PHP_RSHUTDOWN(logger),
	PHP_MINFO(logger),
	PHP_LOGGER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LOGGER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(logger)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
