<?php

try {
    $obj = new phdfs("127.0.0.1","9000");
    $obj->connect();
    $log = $obj->copy('/test.txt','/test1.txt');
} catch (Exception $ex) {
    var_dump($ex->getMessage());
}
?> 
