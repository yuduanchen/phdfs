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
  | Authors: yuduan chen <thisduan@gmail.com>                          |
  +----------------------------------------------------------------------+
*/ 

#include "php_phdfs.h"

#if HAVE_PHDFS

/* {{{ Class definitions */

static zend_class_entry * phdfs_ce_ptr = NULL; 
struct _php_hdfs_hanele {
    phdfs_hadoop_hdfs fs;
} php_hdfs_hanele;
/* {{{ Methods */

/* {{{ proto boolean connect()
 */
PHP_METHOD(phdfs, connect) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    zval *hdfs_ip;
    zval *hdfs_port;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, _this_zval) == FAILURE) {
        return;
    }
    /*
     * To obtain hadhoop  ip
     */
    hdfs_ip = zend_read_property(_this_ce, getThis(), "ip", strlen("ip"), 0 TSRMLS_CC); 
    /*
     * To obtain hadhoop  port
     */
    hdfs_port = zend_read_property(_this_ce, getThis(), "port", strlen("port"), 0 TSRMLS_CC); 
    php_hdfs_hanele.fs  = phdfs_hadoop_hdfs_connect(Z_STRLEN_P(hdfs_ip) ? Z_STRVAL_P(hdfs_ip) : 0, atoi(Z_STRLEN_P(hdfs_port) ? Z_STRVAL_P(hdfs_port) : 0));
    if (!php_hdfs_hanele.fs) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
        ZVAL_FALSE(return_value);
        return;
    }
    ZVAL_TRUE(return_value);
    return;
}
/* }}} connect */

/* {{{ proto mixed write(char*  path,char*  buffer [, int mode ])
 */
PHP_METHOD(phdfs, write) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    const char *buffer = NULL;
    int buffer_len = 0;
    int mode=O_WRONLY|O_CREAT;
    int mode_len=0;
    phdfs_hadoop_tsize buffer_size;
    phdfs_hadoop_hdfs_file hdfs_file;
    phdfs_hadoop_tsize num_written_bytes;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss|l", &_this_zval, _this_zval, &path, &path_len, &buffer, &buffer_len,&mode,&mode_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
        ZVAL_FALSE(return_value);
        return;
    }
    buffer_size = strtoul(buffer, NULL, 10);
    hdfs_file = phdfs_hadoop_hdfs_open_file(php_hdfs_hanele.fs, path,mode,buffer_size, 0, 0);
    if (!hdfs_file) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, "Failed to  hdfs file open ");
        ZVAL_FALSE(return_value);
        return;
    }
    num_written_bytes = phdfs_hadoop_hdfs_write(php_hdfs_hanele.fs, hdfs_file, (void*) buffer, strlen(buffer) + 1);
    if (phdfs_hadoop_hdfs_flush(php_hdfs_hanele.fs, hdfs_file)) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs flush");
        ZVAL_FALSE(return_value);
        return;
    }
    phdfs_hadoop_hdfs_close_file(php_hdfs_hanele.fs, hdfs_file);
    ZVAL_TRUE(return_value);
    return;
}

/* }}} write */

/* {{{ proto boolean disconnect()
 */
PHP_METHOD(phdfs, disconnect) {
    if(phdfs_hadoop_hdfs_disconnect(php_hdfs_hanele.fs)==0){
        ZVAL_TRUE(return_value);
        return;  
    }else{
        ZVAL_FALSE(return_value);
        return;
    }
}
/* }}} disconnect */

/* {{{ proto boolean exists(char*  path)
 */
PHP_METHOD(phdfs, exists) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * filename = NULL;
    int filename_len = 0; 
    int state;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &filename, &filename_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
        ZVAL_FALSE(return_value);
        return;
    }
    state = phdfs_hadoop_hdfs_exists(php_hdfs_hanele.fs,filename);
    if (state==0) {
        ZVAL_TRUE(return_value);
        return;
    }
    ZVAL_FALSE(return_value);
    return;
}
 
/* }}} exists */

/* {{{ proto boolean create_directory(const char*  path)
 */
PHP_METHOD(phdfs, create_directory) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * filename = NULL;
    int filename_len = 0; 
    int state;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &filename, &filename_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
        ZVAL_FALSE(return_value);
        return;
    } 
    state = phdfs_hadoop_hdfs_create_directory(php_hdfs_hanele.fs, filename);
    if (state==0) {
        ZVAL_TRUE(return_value);
        return;
    }
    ZVAL_FALSE(return_value);
    return;
}
/* }}} create_directory */

/* {{{ proto boolean delete(char*  path)
 */
PHP_METHOD(phdfs, delete) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int state;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &path, &path_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
        zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
        ZVAL_FALSE(return_value);
        return;
    }
    state = phdfs_hadoop_hdfs_delete(php_hdfs_hanele.fs, path);
    if (state==0) {
        ZVAL_TRUE(return_value);
        return;
    }
    ZVAL_FALSE(return_value);
    return;
}
/* }}} delete */
 

/* {{{ proto boolean rename(const char* old_path , const char* new_path)
 */
PHP_METHOD(phdfs, rename) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * old_path = NULL;
    const char * new_path = NULL;
    int old_path_len = 0;
    int new_path_len = 0; 
    int state;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss", &_this_zval, _this_zval, &old_path, &old_path_len,&new_path,&new_path_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
       zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
       ZVAL_FALSE(return_value);
       return;
    }
    state = phdfs_hadoop_hdfs_rename(php_hdfs_hanele.fs, old_path,new_path);
    if (state==0) {
        ZVAL_TRUE(return_value);
        return; 
    }
    ZVAL_FALSE(return_value);
    return;
}
/* }}} rename */

/* {{{ proto boolean read(const char* path)
 */
PHP_METHOD(phdfs, read) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * path = NULL; 
    int path_len = 0; 
    int read_length=1024;
    phdfs_hadoop_hdfs_file hdfs_file;
    char* buffer;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, _this_zval, &path, &path_len,&read_length) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
       zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
       ZVAL_FALSE(return_value);
       return;
    }
    hdfs_file = phdfs_hadoop_hdfs_open_file(php_hdfs_hanele.fs,path,O_RDONLY, read_length, 0, 0);
    buffer = emalloc(sizeof(char) * read_length); 
    phdfs_hadoop_hdfs_read(php_hdfs_hanele.fs, hdfs_file, (void*)buffer,read_length);
    ZVAL_STRINGL(return_value,buffer,read_length, 1);
    efree(buffer);
    return;
}
/* }}} read */


/* {{{ proto array file_info(const char* path)
 */
PHP_METHOD(phdfs, file_info) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * path = NULL; 
    int path_len = 0;  
    phdfs_hadoop_hdfs_file_info *hdfs_file_info = NULL;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &path, &path_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
       zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
       ZVAL_FALSE(return_value);
       return;
    }
    if((hdfs_file_info = phdfs_hadoop_hdfs_get_path_info(php_hdfs_hanele.fs, path)) != NULL) { 
            array_init(return_value);
            add_assoc_string(return_value,"name",hdfs_file_info->mName, 1);
            add_assoc_double(return_value,"replication",hdfs_file_info->mReplication);
            add_assoc_double(return_value,"blockSize",hdfs_file_info->mBlockSize);
            add_assoc_double(return_value,"size",hdfs_file_info->mSize);
            add_assoc_string(return_value,"lastMod",ctime(&hdfs_file_info->mLastMod), 1);
            add_assoc_string(return_value,"owner",hdfs_file_info->mOwner, 1);
            add_assoc_string(return_value,"group",hdfs_file_info->mGroup, 1);
            phdfs_hadoop_hdfs_free_file_info(hdfs_file_info, 1);
    }else{
        ZVAL_FALSE(return_value);
       return;
    }
    return;
}
/* }}} file_info */


/* {{{ proto array list_directory(const char* path)
 */
PHP_METHOD(phdfs, list_directory) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * path = NULL; 
    int path_len = 0;
    int level=0;
    zval *subarray;
    int i=0;
    phdfs_hadoop_hdfs_file_info *hdfs_file_info = NULL;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, _this_zval, &path, &path_len,&level) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
       zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
       ZVAL_FALSE(return_value);
       return;
    }
    if((hdfs_file_info = phdfs_hadoop_hdfs_list_directory(php_hdfs_hanele.fs, path,&level)) != NULL) { 
        array_init(return_value);
        for(i=0; i < level; ++i) {
                MAKE_STD_ZVAL(subarray);
                array_init(subarray);
                add_assoc_string(subarray,"name",hdfs_file_info[i].mName, 1);
                add_assoc_double(subarray,"replication",hdfs_file_info[i].mReplication);
                add_assoc_double(subarray,"blockSize",hdfs_file_info[i].mBlockSize);
                add_assoc_double(subarray,"size",hdfs_file_info[i].mSize);
                add_assoc_string(subarray,"lastMod",ctime(&hdfs_file_info[i].mLastMod), 1);
                add_assoc_string(subarray,"owner",hdfs_file_info[i].mOwner, 1);
                add_assoc_string(subarray,"group",hdfs_file_info[i].mGroup, 1); 
                add_index_zval(return_value,i,subarray);
        }
        phdfs_hadoop_hdfs_free_file_info(hdfs_file_info,level);
    }else{
        ZVAL_FALSE(return_value);
       return;
    }
    return;
}
/* }}} list_directory */

/* {{{ proto int tell(const char* path)
 */
PHP_METHOD(phdfs,tell) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char * path = NULL; 
    int path_len = 0; 
    int read_length=1024;
    phdfs_hadoop_hdfs_file hdfs_file;
    phdfs_hadoop_toffset current_pos = -1;
    phdfs_hadoop_toffset seek_pos = 1;
    char* buffer;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, _this_zval, &path, &path_len,&read_length) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
       zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
       ZVAL_FALSE(return_value);
       return;
    }
    hdfs_file = phdfs_hadoop_hdfs_open_file(php_hdfs_hanele.fs,path,O_RDONLY, read_length, 0, 0);
    if((current_pos = phdfs_hadoop_hdfs_tell(php_hdfs_hanele.fs,hdfs_file)) != seek_pos) { 
       ZVAL_LONG(return_value,current_pos);
       return;
    }
    ZVAL_LONG(return_value,current_pos);
    return;
}
/* }}} tell */

/* {{{ proto boolean copy(const char* source_file,const char* destination_file)
 */
PHP_METHOD(phdfs,copy) {
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;
    const char *source_file = NULL;
    const char *destination_file;
    int source_file_len = 0;
    int destination_file_len=0;
    phdfs_hadoop_hdfs_file hdfs_file;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss", &_this_zval, _this_zval, &source_file, &source_file_len,&destination_file,&destination_file_len) == FAILURE) {
        return;
    }
    if (!php_hdfs_hanele.fs) {
       zend_throw_exception_ex(_this_ce, 12 TSRMLS_CC, " Failed to hdfs connection ");
       ZVAL_FALSE(return_value);
       return;
    } 
    if(phdfs_hadoop_hdfs_copy(php_hdfs_hanele.fs,source_file,php_hdfs_hanele.fs, destination_file) == 0 ) { 
       ZVAL_TRUE(return_value);
       return;
    }
    ZVAL_FALSE(return_value);
    return;
}
/* }}} copy */

static zend_function_entry phdfs_methods[] = {
    PHP_ME(phdfs,connect, phdfs__connect_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs,disconnect, phdfs__disconnect_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, exists, phdfs__exists_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, write, phdfs__write_args,ZEND_ACC_PUBLIC) 
    PHP_ME(phdfs, read, phdfs__read_args,ZEND_ACC_PUBLIC) 
    PHP_ME(phdfs, file_info, phdfs__file_info_args,ZEND_ACC_PUBLIC) 
    PHP_ME(phdfs, create_directory, phdfs__create_directory_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, rename, phdfs__rename_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, list_directory, phdfs__list_directory_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, tell, phdfs__tell_args,ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, copy, phdfs__copy_args,ZEND_ACC_PUBLIC) 
    PHP_ME(phdfs, delete, phdfs__delete_args,ZEND_ACC_PUBLIC) {
        NULL, NULL, NULL
    }
};

/* }}} Class definitions*/

/* {{{ phdfs_functions[] */
zend_function_entry phdfs_functions[] = {
    { NULL, NULL, NULL}
};
/* }}} */


/* {{{ phdfs_module_entry
 */
zend_module_entry phdfs_module_entry = {
    STANDARD_MODULE_HEADER,
    "phdfs",
    phdfs_functions,
    PHP_MINIT(phdfs), /* Replace with NULL if there is nothing to do at php startup   */
    PHP_MSHUTDOWN(phdfs), /* Replace with NULL if there is nothing to do at php shutdown  */ 
    NULL,
    NULL,
    PHP_MINFO(phdfs),
    PHP_PHDFS_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PHDFS

ZEND_GET_MODULE(phdfs)
#endif


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(phdfs) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "phdfs", phdfs_methods);
    phdfs_ce_ptr = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_string(phdfs_ce_ptr, "ip", strlen("ip"), "127.0.0.1", ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_string(phdfs_ce_ptr, "port", strlen("port"), "9000", ZEND_ACC_PUBLIC TSRMLS_CC);
    REGISTER_LONG_CONSTANT("O_WRONLY",O_WRONLY,	CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("O_CREAT",O_CREAT,	CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("O_APPEND",O_APPEND,	CONST_CS | CONST_PERSISTENT);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(phdfs) { 
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(phdfs) { 
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(phdfs) { 
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(phdfs) { 
    php_info_print_table_start();
    php_info_print_table_header(2, "phdfs support", "enabled");
    php_info_print_table_row(2, "Version", PHP_PHDFS_VERSION " (stable)");
    php_info_print_table_end();
}
/* }}} */

#endif /* HAVE_PHDFS */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
