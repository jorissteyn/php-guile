--TEST--
Test guile_eval with single pair as return value
--SKIPIF--
<?php if (!extension_loaded("guile")) print "skip"; ?>
--FILE--
<?php
$data = <<<'SCHEME'
(cons 1 2)
SCHEME;

var_dump(
    guile_eval($data)
);
?>
--EXPECT--
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}