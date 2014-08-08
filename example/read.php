<?php

try {
    $obj = new phdfs();
    $obj->port = "9000";
    $obj->ip = "127.0.0.1";
    $obj->connect();
    $log = $obj->read('/test.txt',1024);
} catch (Exception $ex) {
    var_dump($ex->getMessage());
}
?>