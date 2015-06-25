--TEST--
Test guile_eval with string return value
--SKIPIF--
<?php if (!extension_loaded("guile")) print "skip"; ?>
--FILE--
<?php
guile_init();

$data = <<<'SCHEME'
(string-concatenate '("hello" "guile"))
SCHEME;

var_dump(
    guile_eval($data)
);
?>
--EXPECT--
string(10) "helloguile"