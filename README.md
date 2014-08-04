
###Summary

phdfs - PHP extensions, using PHP to manipulate Hadoop HDFS

###Maintainers
yuduanchen


## Install

## Requirements

* PHP Version: PHP 5.3+
* ZTS Enabled ( Thread Safety )
* Installing Hadoop


##Hadoop installation

hadoop docs: http://hadoop.apache.org/docs/r1.0.4/cn/quickstart.html

## hadoop Local environment configuration
vim /etc/profile

32-bit system
```
export JAVA_HOME=/usr/jdk1.7.0_40/
export JRE_HOME=$JAVA_HOME/jre/
export HADOOP_HOME=/usr/local/hadoop/
export C_INICLUDE_PATH=$HADOOP_HOME/hdfs/src/c++/libhdfs:$JAVA_HOME/include:$JAVA_HOME/include/Linux
export LD_LIBRARY_PATH=$HADOOP_HOME/c++/Linux-i386-32/lib:$JAVA_HOME/jre/lib/i386/server
for i in $HADOOP_HOME/*.jar
do
    CLASSPATH=$CLASSPATH:$i
done
for i in $HADOOP_HOME/lib/*.jar
do
     CLASSPATH=$CLASSPATH:$i
done
export CLASSPATH

```
64-bit system
```
export JAVA_HOME=/usr/jdk1.7.0_40/
export JRE_HOME=$JAVA_HOME/jre/
export HADOOP_HOME=/usr/local/hadoop/
export C_INICLUDE_PATH=$HADOOP_HOME/hdfs/src/c++/libhdfs:$JAVA_HOME/include:$JAVA_HOME/include/Linux
export LD_LIBRARY_PATH=$HADOOP_HOME/c++/Linux-amd64-64/lib:$JAVA_HOME/jre/lib/i386/server
for i in $HADOOP_HOME/*.jar
do
    CLASSPATH=$CLASSPATH:$i
done
for i in $HADOOP_HOME/lib/*.jar
do
     CLASSPATH=$CLASSPATH:$i
done
export CLASSPATH

```

```
source /etc/profile
```


vim /etc/ld.so.conf

32-bit system
```
${HADOOP_HOME}/c++/Linux-i386-32/lib
${JAVA_HOME}/jre/lib/i386/client

```
64-bit system
```
${HADOOP_HOME}/c++/Linux-amd64-64/lib
${JAVA_HOME}jre/lib/amd64/

```
```
ldconfig -v

```

##phdfs installation
```
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config/
$make && make install
```


## Documentation



### The phdfs class
```c
class phdfs {
    /* variable */
    public $ip= "127.0.0.1" ;//hadoop ip
    public $port = "9000";//hadoop port /

    /***
     * Connect to a hdfs file system. Connect to the hdfs.
     * Returns true on success, false on error.
     */
    public function connect();
    /***
     * Disconnect from the hdfs file system. Disconnect from hdfs.
     * Returns true on success, false on error.
     */
    public function disconnect();
    /***
     * Checks if a given path exsits on the filesystem
     * Returns true on success, false on error.
     */
    public function exists(string $path);
     /***
     * Write data into an open file.
     * int mode: O_WRONLY 、O_CREAT、O_APPEND
     * Returns true on success, false on error.
     */
    public function write(string $file,string buffer [, int $mode ]);
     /***
     * Read data from an open file.
     * $buffer_len default 1024k
     * Returns true on success, false on error.
     */
    public function read(string $file[,int $buffer_len]);
     /***
     * Get information about a path
     * Returns array on success, false on error.
     */
    public function file_info(string $path);

    /***
     * make the given file and all non-existent parents into directories.
     * Returns true on success, false on error.
     */
    public function create_directory(string $path);
     /***
     * Rename file.
     * Returns true on success, false on error.
     */
    public function rename(sting $old_path,sting $new_ptah);
     /***
     * Get list of files/directories for a given directory-path
     * Returns true on success, false on error.
     */
    public function list_directory(string $path);
    /***
     * Get the current offset in the file, in bytes.
     * Returns Current offset,  false on error.
     */
    public function tell(string $file  [, int $buffer_len ]);
    /***
     * Copy file
     * Returns true on success, false on error.
     */
    public function copy(string $source_file,string $destination_file);
    /***
     * Delete file
     * Returns true on success, false on error.
     */
    public function delete($string $file);

}

```

##  Demo

```php

<?php
    try {
        $obj = new phdfs();
        $obj->port = "9000";
        $obj->ip = "127.0.0.1";
        $obj->connect();
        //create file
        $log =  $obj->write("/a/b/c/test1.txt","test",O_WRONLY|O_CREAT);
        //appden file
        $log =  $obj->write("/a/b/c/test2.txt","test",O_WRONLY|O_APPEND);
        var_dump($log);
        echo $obj->rename("/a/b/c/test2.txt","/a/b/c/test3.txt");
    }catch (Exception $ex) {
        echo $ex->getMessage();
    }
?>
```



### more

Author blog: http://www.woyuw.com


