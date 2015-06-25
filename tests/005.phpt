--TEST--
Test guile_eval with float return value
--SKIPIF--
<?php if (!extension_loaded("guile")) print "skip"; ?>
--FILE--
<?php
$data = <<<'SCHEME'
(/ 10 4)
SCHEME;

var_dump(
    guile_eval($data)
);
?>
--EXPECT--
float(2.5)