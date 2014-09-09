--TEST--
Check for phdfs connect function
--SKIPIF--
<?php if (!extension_loaded("phdfs")) print "skip"; ?>
--FILE--
<?php

try {
    $obj = new phdfs("127.0.0.1","9000");
    $log = $obj->connect();
    var_dump($log);
} catch (Exception $ex) {
    var_dump($ex->getMessage());
}
?>
--EXPECT--
bool(true)
