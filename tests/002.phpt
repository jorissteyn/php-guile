--TEST--
Test guile initialization
--SKIPIF--
<?php if (!extension_loaded("guile")) print "skip"; ?>
--FILE--
<?php
guile_init();
echo "guile initialized";
?>
--EXPECT--
guile initialized
