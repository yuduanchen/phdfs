/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: yuduan chen <yumin1985@php.net>                             |
  +----------------------------------------------------------------------+
*/ 

#ifndef PHP_PHDFS_H
#define PHP_PHDFS_H

#define phdfs_hadoop_tsize                  tSize
#define phdfs_hadoop_hdfs_connect           hdfsConnect
#define phdfs_hadoop_hdfs_file              hdfsFile
#define phdfs_hadoop_hdfs_open_file         hdfsOpenFile
#define phdfs_hadoop_hdfs_write             hdfsWrite
#define phdfs_hadoop_hdfs_close_file        hdfsCloseFile
#define phdfs_hadoop_hdfs_disconnect        hdfsDisconnect
#define phdfs_hadoop_hdfs_exists            hdfsExists
#define phdfs_hadoop_hdfs_create_directory  hdfsCreateDirectory
#define phdfs_hadoop_hdfs_delete            hdfsDelete 
#define phdfs_hadoop_hdfs_rename            hdfsRename
#define phdfs_hadoop_hdfs_read              hdfsRead
#define phdfs_hadoop_hdfs_get_path_info     hdfsGetPathInfo
#define phdfs_hadoop_hdfs_file_info         hdfsFileInfo
#define phdfs_hadoop_hdfs_free_file_info    hdfsFreeFileInfo
#define phdfs_hadoop_hdfs_flush             hdfsFlush
#define phdfs_hadoop_hdfs_copy              hdfsCopy
#define phdfs_hadoop_hdfs_list_directory    hdfsListDirectory
#define phdfs_hadoop_hdfs_tell              hdfsTell
#define phdfs_hadoop_toffset                tOffset
#define phdfs_hadoop_hdfs                   hdfsFS

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#ifdef HAVE_PHDFS
#define PHP_PHDFS_VERSION "0.1.2"


#include <php_ini.h>
#include <SAPI.h>
#include <ext/standard/info.h>
#include <Zend/zend_extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdfs.h"
#ifdef  __cplusplus
} 
#endif
#ifdef  __cplusplus
extern "C" {
#endif

extern zend_module_entry phdfs_module_entry;
#define phpext_phdfs_ptr &phdfs_module_entry

#ifdef PHP_WIN32
#define PHP_PHDFS_API __declspec(dllexport)
#else
#define PHP_PHDFS_API
#endif

PHP_MINIT_FUNCTION(phdfs);
PHP_MSHUTDOWN_FUNCTION(phdfs);
PHP_RINIT_FUNCTION(phdfs);
PHP_RSHUTDOWN_FUNCTION(phdfs);
PHP_MINFO_FUNCTION(phdfs);

PHP_METHOD(phdfs, __construct);                                      
PHP_METHOD(phdfs, __destruct);

#ifdef ZTS
#include "TSRM.h"
#endif

#define FREE_RESOURCE(resource) zend_list_delete(Z_LVAL_P(resource))

#define PROP_GET_LONG(name)    Z_LVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_LONG(name, l) zend_update_property_long(_this_ce, _this_zval, #name, strlen(#name), l TSRMLS_CC)
#define PROP_GET_DOUBLE(name)    Z_DVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_DOUBLE(name, d) zend_update_property_double(_this_ce, _this_zval, #name, strlen(#name), d TSRMLS_CC)
#define PROP_GET_STRING(name)    Z_STRVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_GET_STRLEN(name)    Z_STRLEN_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_STRING(name, s) zend_update_property_string(_this_ce, _this_zval, #name, strlen(#name), s TSRMLS_CC)
#define PROP_SET_STRINGL(name, s, l) zend_update_property_stringl(_this_ce, _this_zval, #name, strlen(#name), s, l TSRMLS_CC)

PHP_METHOD(phdfs, __construct);
PHP_METHOD(phdfs, __destruct);
typedef struct _ze_phdfs_object {
	zend_object zo;
	phdfs_hadoop_hdfs ptr;
	char *hdfs_host;
	char *hdfs_port;
} ze_phdfs_object;

ZEND_BEGIN_MODULE_GLOBALS(phdfs)
    phdfs_hadoop_hdfs fs;
ZEND_END_MODULE_GLOBALS(phdfs)
#ifdef ZTS
#define PHDFS_G(v) TSRMG(phdfs_globals_id,zend_phdfs_globals *, v)
#else
#define PHDFS_G(v) (phdfs_globals.v)
#endif
ZEND_DECLARE_MODULE_GLOBALS(phdfs)
        
PHP_METHOD(phdfs, __construct);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO(phdfs__construct_args,2)
  ZEND_ARG_INFO(0,host)
  ZEND_ARG_INFO(0,port)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__construct_args NULL
#endif
        
PHP_METHOD(phdfs, connect);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__connect_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE,0)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__connect_args NULL
#endif
   
PHP_METHOD(phdfs, rename);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__rename_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE,2)
  ZEND_ARG_INFO(0,old_path)
  ZEND_ARG_INFO(0,new_path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__rename_args NULL
#endif

PHP_METHOD(phdfs, read);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__read_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE,1)
  ZEND_ARG_INFO(0,path)
  ZEND_ARG_INFO(0,length)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__read_args NULL
#endif
        
PHP_METHOD(phdfs, file_info);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__file_info_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__file_info_args NULL
#endif
                
PHP_METHOD(phdfs,create_directory);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__create_directory_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__create_directory_args NULL
#endif

PHP_METHOD(phdfs, exists);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__exists_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__exists_args NULL
#endif
        
PHP_METHOD(phdfs, disconnect);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__disconnect_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE,0) 
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__disconnect_args NULL
#endif
        
        
PHP_METHOD(phdfs, write);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__write_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 2)
  ZEND_ARG_INFO(0,path)
  ZEND_ARG_INFO(0,buffer)
  ZEND_ARG_INFO(0,mode)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__write_args NULL
#endif

PHP_METHOD(phdfs, delete);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__delete_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__delete_args NULL
#endif
        
PHP_METHOD(phdfs, copy);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__copy_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE,2)
  ZEND_ARG_INFO(0, source_file)
  ZEND_ARG_INFO(0, destination_file)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__copy_args NULL
#endif
        
PHP_METHOD(phdfs,list_directory);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__list_directory_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__list_directory_args NULL
#endif
        
PHP_METHOD(phdfs,tell);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__tell_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__tell_args NULL
#endif 

PHP_METHOD(phdfs, __destruct);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(phdfs__destruct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define phdfs__destruct_args NULL
#endif 

#ifdef  __cplusplus
}
#endif

#endif /* PHP_HAVE_PHDFS */

#endif /* PHP_PHDFS_H */

