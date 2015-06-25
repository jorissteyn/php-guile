--TEST--
Test guile_eval with integer return value
--SKIPIF--
<?php if (!extension_loaded("guile")) print "skip"; ?>
--FILE--
<?php
$data = <<<'SCHEME'
(+ 1 1)
SCHEME;

var_dump(
    guile_eval($data)
);
?>
--EXPECT--
int(2)