<?php

try {
    $obj = new phdfs();
    $obj->port = "9000";
    $obj->ip = "127.0.0.1";
    $log = $obj->connect();
} catch (Exception $ex) {
    var_dump($ex->getMessage());
}
?> 