--TEST--
Check for phdfs presence
--SKIPIF--
<?php if (!extension_loaded("phdfs")) print "skip"; ?>
--FILE--
<?php 
echo "phdfs extension is available";
?>
--EXPECT--
phdfs extension is available
