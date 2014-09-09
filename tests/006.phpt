--TEST--
Check for phdfs file_info function
--SKIPIF--
<?php if (!extension_loaded("phdfs")) print "skip"; ?>
--FILE--
<?php

try {
    $obj = new phdfs("127.0.0.1","9000");
    $obj->connect();
    $log = $obj->file_info('/test.txt');
    print($log);
} catch (Exception $ex) {
    var_dump($ex->getMessage());
}
?>
--EXPECT--
Array
