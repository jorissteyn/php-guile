--TEST--
Test guile initialization
--SKIPIF--
<?php if (!extension_loaded("guile")) print "skip"; ?>
--FILE--
<?php
echo "guile initialized";
?>
--EXPECT--
guile initialized
