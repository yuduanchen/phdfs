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

#include "php_phdfs.h"

#if HAVE_PHDFS 

/* {{{ Class definitions */

zend_class_entry *phdfs_ce_ptr;

static zend_object_handlers phdfs_object_handlers;
typedef int (*php_phdfs_read_t)(ze_phdfs_object *phdfs_object, zval **retval TSRMLS_DC);
typedef int (*php_phdfs_write_t)(ze_phdfs_object *phdfs_object, zval *newval TSRMLS_DC);

typedef struct _ptp_phdfs_prop_handler {
    const char *name;
    size_t name_length;
    php_phdfs_read_t read_func;
    php_phdfs_write_t write_func;
} php_phdfs_prop_handler;

/* Class object properties */
static HashTable php_phdfs_properties;
#if  PHP_VERSION_ID >= 50400
zval *php_phdfs_read_property(zval *object, zval *member, int type, const zend_literal *key TSRMLS_DC) {
#else 
    zval *php_phdfs_read_property(zval *object, zval *member, int type  TSRMLS_DC) {
#endif
    zval tmp_member;
    zval *retval;
    ze_phdfs_object *obj;
    php_phdfs_prop_handler *hnd;
    int ret; 
    ret = FAILURE;
    obj = (ze_phdfs_object *) zend_objects_get_address(object TSRMLS_CC); 
    if (Z_TYPE_P(member) != IS_STRING) {
        tmp_member = *member;
        zval_copy_ctor(&tmp_member);
        convert_to_string(&tmp_member);
        member = &tmp_member;
    }
    ret = zend_hash_find(&php_phdfs_properties, Z_STRVAL_P(member), Z_STRLEN_P(member) + 1, (void **) &hnd);
    if (ret == SUCCESS && hnd->read_func) {
        ret = hnd->read_func(obj, &retval TSRMLS_CC);
        if (ret == SUCCESS) {
            Z_SET_REFCOUNT_P(retval, 0);
        } else {
            retval = EG(uninitialized_zval_ptr);
        }
    } else {
        zend_object_handlers * std_hnd = zend_get_std_object_handlers();
        #if  PHP_VERSION_ID >= 50400
        retval = std_hnd->read_property(object, member, type, key TSRMLS_CC);
        #else 
        retval = std_hnd->read_property(object, member, type TSRMLS_CC);
        #endif
    }
    if (member == &tmp_member) {
        zval_dtor(member);
    }
    return (retval);
}
#if  PHP_VERSION_ID >= 50400
void php_phdfs_write_property(zval *object, zval *member, zval *value, const zend_literal *key TSRMLS_DC) {
#else
void php_phdfs_write_property(zval *object, zval *member, zval *value  TSRMLS_DC) {
#endif
    zval tmp_member;
    ze_phdfs_object *obj;
    php_phdfs_prop_handler *hnd;
    int ret;
    if (Z_TYPE_P(member) != IS_STRING) {
        tmp_member = *member;
        zval_copy_ctor(&tmp_member);
        convert_to_string(&tmp_member);
        member = &tmp_member;
    }
    ret = FAILURE;
    obj = (ze_phdfs_object *) zend_objects_get_address(object TSRMLS_CC);
    ret = zend_hash_find(&php_phdfs_properties, Z_STRVAL_P(member), Z_STRLEN_P(member) + 1, (void **) &hnd);
    if (ret == SUCCESS && hnd->write_func) {
        hnd->write_func(obj, value TSRMLS_CC);
        if (!PZVAL_IS_REF(value) && Z_REFCOUNT_P(value) == 0) {
            Z_ADDREF_P(value);
            zval_ptr_dtor(&value);
        }
    } else {
        zend_object_handlers * std_hnd = zend_get_std_object_handlers();
        #if  PHP_VERSION_ID >= 50400
        std_hnd->write_property(object, member, value, key TSRMLS_CC);
        #else
        std_hnd->write_property(object, member, value  TSRMLS_CC);
        #endif
    }
    if (member == &tmp_member) {
        zval_dtor(member);
    }
}

static void phdfs_objects_free_storage(void *object TSRMLS_DC) {
    ze_phdfs_object *intern = (ze_phdfs_object *) object;
    if (!intern) {
        return;
    }
    zend_object_std_dtor(&intern->zo TSRMLS_CC);
    efree(intern);
}

/* }}} */
 #if  PHP_VERSION_ID >= 50400
static int php_phdfs_has_property(zval *object, zval *member, int has_set_exists, const zend_literal *key TSRMLS_DC) {
#else
static int php_phdfs_has_property(zval *object, zval *member, int has_set_exists TSRMLS_DC) {
#endif

    php_phdfs_prop_handler *hnd;
    int ret = 0;
    if (zend_hash_find(&php_phdfs_properties, Z_STRVAL_P(member), Z_STRLEN_P(member) + 1, (void **) &hnd) == SUCCESS) {
        switch (has_set_exists) {
            case 2:
                ret = 1;
                break;
            case 0:
            {
                #if  PHP_VERSION_ID >= 50400
                zval *value = php_phdfs_read_property(object, member, BP_VAR_IS, key TSRMLS_CC);
                #else
                zval *value = php_phdfs_read_property(object, member, BP_VAR_IS TSRMLS_CC);
                #endif
                
                if (value != EG(uninitialized_zval_ptr)) {
                    ret = Z_TYPE_P(value) != IS_NULL ? 1 : 0;
                    Z_ADDREF_P(value);
                    zval_ptr_dtor(&value);
                }
                break;
            }
            default:
            {
                #if  PHP_VERSION_ID >= 50400
                zval *value = php_phdfs_read_property(object, member, BP_VAR_IS, key TSRMLS_CC);
                #else
                zval *value = php_phdfs_read_property(object, member, BP_VAR_IS TSRMLS_CC);
                #endif
                if (value != EG(uninitialized_zval_ptr)) {
                    convert_to_boolean(value);
                    ret = Z_BVAL_P(value) ? 1 : 0;
                    Z_ADDREF_P(value);
                    zval_ptr_dtor(&value);
                }
                break;
            }
        }
    } else {
        zend_object_handlers * std_hnd = zend_get_std_object_handlers();
        #if  PHP_VERSION_ID >= 50400
        ret = std_hnd->has_property(object, member, has_set_exists, key TSRMLS_CC);
        #else
        ret = std_hnd->has_property(object, member, has_set_exists TSRMLS_CC);
        #endif
        
    }
    return ret;
}

static HashTable *php_phdfs_get_properties(zval *object TSRMLS_DC) {
    ze_phdfs_object *obj;
    php_phdfs_prop_handler *hnd;
    HashTable *props;
    zval *val;
    char *key;
    uint key_len;
    HashPosition pos;
    ulong num_key;
    obj = (ze_phdfs_object *) zend_objects_get_address(object TSRMLS_CC);
    props = zend_std_get_properties(object TSRMLS_CC);
    zend_hash_internal_pointer_reset_ex(&php_phdfs_properties, &pos);
    while (zend_hash_get_current_data_ex(&php_phdfs_properties, (void**) &hnd, &pos) == SUCCESS) {
        zend_hash_get_current_key_ex(&php_phdfs_properties, &key, &key_len, &num_key, 0, &pos);
        if (!hnd->read_func || hnd->read_func(obj, &val TSRMLS_CC) != SUCCESS) {
            val = EG(uninitialized_zval_ptr);
            Z_ADDREF_P(val);
        }
        zend_hash_update(props, key, key_len, (void *) &val, sizeof (zval *), NULL);
        zend_hash_move_forward_ex(&php_phdfs_properties, &pos);
    }
    return obj->zo.properties;
}

static zend_object_value phdfs_objects_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */ {
    zend_object_value retval;
    ze_phdfs_object *intern;
    intern = emalloc(sizeof (ze_phdfs_object)); 
    memset(&intern->zo, 0, sizeof (ze_phdfs_object));
    zend_object_std_init(&intern->zo, class_type TSRMLS_CC);
    object_properties_init(&intern->zo, class_type);
    retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, (zend_objects_free_object_storage_t) phdfs_objects_free_storage, NULL TSRMLS_CC);
    retval.handlers = (zend_object_handlers *) & phdfs_object_handlers;
    return retval;
}

/* {{{ Methods */

/* {{{ proto boolean __construct()
 */
PHP_METHOD(phdfs, __construct) { 
    zval * _this_zval = NULL;
    int hdfs_host_len = 0;
    int hdfs_port_len = 0;
    char *hdfs_host,*hdfs_port;
    zval *id; 
    id = getThis();
    ze_phdfs_object *intern; 
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &hdfs_host,&hdfs_host_len,&hdfs_port,&hdfs_port_len) == FAILURE) {
        return;
    }  
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    intern->hdfs_host = hdfs_host; 
    intern->hdfs_port = hdfs_port;
    intern->ptr = NULL; 
    zend_update_property_string(phdfs_ce_ptr,id,"host",strlen("host"),hdfs_host  TSRMLS_CC);
    zend_update_property_string(phdfs_ce_ptr,id,"port",strlen("port"),hdfs_port  TSRMLS_CC);    
}
/* }}} __construct */

/* {{{ proto boolean __destruct()
 */
PHP_METHOD(phdfs, __destruct) {
}
/* }}} __destruct */

/* {{{ proto boolean connect()
 */
PHP_METHOD(phdfs, connect) {
    zval * _this_zval = NULL;
    zval *id,*host,*port;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, _this_zval) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);  
    host = zend_read_property(Z_OBJCE_P(_this_zval),id, ZEND_STRL("host"),0 TSRMLS_CC);
    port = zend_read_property(Z_OBJCE_P(_this_zval),id,ZEND_STRL("port"),0 TSRMLS_CC);  
    intern->ptr = phdfs_hadoop_hdfs_connect(Z_STRVAL_P(host) ? Z_STRVAL_P(host) : 0, atoi(Z_STRVAL_P(port) ? Z_STRVAL_P(port) : 0));
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
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
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    const char *buffer = NULL;
    int buffer_len = 0;
    int mode = O_WRONLY | O_CREAT;
    int mode_len = 0;
    zval *id;
    phdfs_hadoop_tsize buffer_size;
    phdfs_hadoop_hdfs_file hdfs_file;
    ze_phdfs_object *intern;
    phdfs_hadoop_tsize num_written_bytes;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss|l", &_this_zval, _this_zval, &path, &path_len, &buffer, &buffer_len, &mode, &mode_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    } 
    buffer_size = strtoul(buffer, NULL, 10); 
    hdfs_file = phdfs_hadoop_hdfs_open_file(intern->ptr, path, mode, buffer_size, 0, 0); 
    if (!hdfs_file) { 
        ZVAL_FALSE(return_value);
        return;
    } 
    num_written_bytes = phdfs_hadoop_hdfs_write(intern->ptr, hdfs_file, (void*) buffer, strlen(buffer) + 1);
    if (phdfs_hadoop_hdfs_flush(intern->ptr, hdfs_file)) { 
        ZVAL_FALSE(return_value);
        return;
    }
    phdfs_hadoop_hdfs_close_file(intern->ptr, hdfs_file);
    ZVAL_TRUE(return_value);
    return;
}

/* }}} write */

/* {{{ proto boolean disconnect()
 */
PHP_METHOD(phdfs, disconnect) {
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    if(phdfs_hadoop_hdfs_disconnect(intern->ptr)==0){
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
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int state;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &path, &path_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    state = phdfs_hadoop_hdfs_exists(intern->ptr,path);
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
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int state;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &path, &path_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    state = phdfs_hadoop_hdfs_create_directory(intern->ptr, path);
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
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int state;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &path, &path_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    state = phdfs_hadoop_hdfs_delete(intern->ptr, path);
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
    zval * _this_zval = NULL;
    const char * old_path = NULL;
    const char * new_path = NULL;
    int old_path_len = 0;
    int new_path_len = 0;
    int state;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss", &_this_zval, _this_zval, &old_path, &old_path_len, &new_path, &new_path_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    state = phdfs_hadoop_hdfs_rename(intern->ptr, old_path,new_path);
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
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int read_length = 1024;
    phdfs_hadoop_hdfs_file hdfs_file;
    char* buffer;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, _this_zval, &path, &path_len, &read_length) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    hdfs_file = phdfs_hadoop_hdfs_open_file(intern->ptr,path,O_RDONLY, read_length, 0, 0);
    buffer = emalloc(sizeof(char) * read_length); 
    phdfs_hadoop_hdfs_read(intern->ptr, hdfs_file, (void*)buffer,read_length);
    ZVAL_STRINGL(return_value,buffer,read_length, 1);
    efree(buffer);
    return;
}
/* }}} read */

/* {{{ proto array file_info(const char* path)
 */
PHP_METHOD(phdfs, file_info) { 
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    phdfs_hadoop_hdfs_file_info *hdfs_file_info = NULL;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, _this_zval, &path, &path_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    if((hdfs_file_info = phdfs_hadoop_hdfs_get_path_info(intern->ptr, path)) != NULL) { 
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
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int level = 0;
    zval *subarray;
    int i = 0;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    phdfs_hadoop_hdfs_file_info *hdfs_file_info = NULL;
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, _this_zval, &path, &path_len, &level) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    if((hdfs_file_info = phdfs_hadoop_hdfs_list_directory(intern->ptr, path,&level)) != NULL) { 
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
PHP_METHOD(phdfs, tell) { 
    zval * _this_zval = NULL;
    const char * path = NULL;
    int path_len = 0;
    int read_length = 1024;
    phdfs_hadoop_hdfs_file hdfs_file;
    phdfs_hadoop_toffset current_pos = -1;
    phdfs_hadoop_toffset seek_pos = 1;
    char* buffer;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, _this_zval, &path, &path_len, &read_length) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    hdfs_file = phdfs_hadoop_hdfs_open_file(intern->ptr,path,O_RDONLY, read_length, 0, 0);
    if((current_pos = phdfs_hadoop_hdfs_tell(intern->ptr,hdfs_file)) != seek_pos) { 
       ZVAL_LONG(return_value,current_pos);
       return;
    }
    ZVAL_LONG(return_value,current_pos);
    return;
}
/* }}} tell */

/* {{{ proto boolean copy(const char* source_file,const char* destination_file)
 */
PHP_METHOD(phdfs, copy) {
    zval * _this_zval = NULL;
    const char *source_file = NULL;
    const char *destination_file;
    int source_file_len = 0;
    int destination_file_len = 0;
    phdfs_hadoop_hdfs_file hdfs_file;
    zval *id;
    ze_phdfs_object *intern;
    id = getThis();
    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss", &_this_zval, _this_zval, &source_file, &source_file_len, &destination_file, &destination_file_len) == FAILURE) {
        return;
    }
    intern = (ze_phdfs_object *) zend_object_store_get_object(id TSRMLS_CC);
    if (!intern->ptr) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
						"Unable to connect to the server");
        ZVAL_FALSE(return_value);
        return;
    }
    if(phdfs_hadoop_hdfs_copy(intern->ptr,source_file,intern->ptr, destination_file) == 0 ) { 
       ZVAL_TRUE(return_value);
       return;
    }
    ZVAL_FALSE(return_value);
    return;
}

/* }}} copy */

static zend_function_entry phdfs_methods[] = {
    PHP_ME(phdfs, __construct, phdfs__construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(phdfs, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(phdfs, connect, phdfs__connect_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, disconnect, phdfs__disconnect_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, exists, phdfs__exists_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, write, phdfs__write_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, read, phdfs__read_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, file_info, phdfs__file_info_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, create_directory, phdfs__create_directory_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, rename, phdfs__rename_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, list_directory, phdfs__list_directory_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, tell, phdfs__tell_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, copy, phdfs__copy_args, ZEND_ACC_PUBLIC)
    PHP_ME(phdfs, delete, phdfs__delete_args, ZEND_ACC_PUBLIC) {
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
    memcpy(&phdfs_object_handlers, zend_get_std_object_handlers(), sizeof (zend_object_handlers));
    phdfs_object_handlers.read_property = php_phdfs_read_property;
    phdfs_object_handlers.write_property = php_phdfs_write_property;
    phdfs_object_handlers.has_property = php_phdfs_has_property;
    phdfs_object_handlers.get_properties = php_phdfs_get_properties;
    INIT_CLASS_ENTRY(ce, "phdfs", phdfs_methods);
    ce.create_object = phdfs_objects_new;
    phdfs_object_handlers.clone_obj = NULL;
    phdfs_ce_ptr = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_string(phdfs_ce_ptr, "host", strlen("host"), "127.0.0.1", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
    zend_declare_property_string(phdfs_ce_ptr, "port", strlen("port"), "9000", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
    zend_hash_init(&php_phdfs_properties, 0, NULL, NULL, 1);
    REGISTER_LONG_CONSTANT("O_WRONLY", O_WRONLY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("O_CREAT", O_CREAT, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("O_APPEND", O_APPEND, CONST_CS | CONST_PERSISTENT);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(phdfs) {  
    zend_hash_destroy(&php_phdfs_properties); 
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
