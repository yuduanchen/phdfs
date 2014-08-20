<?php

try {
    $obj = new phdfs("127.0.0.1","9000");
    $log = $obj->connect();
} catch (Exception $ex) {
    var_dump($ex->getMessage());
}
?> 
