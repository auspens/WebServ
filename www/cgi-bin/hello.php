#!/usr/bin/env php
<?php

$name = isset($_GET['name']) && trim($_GET['name']) !== "" ? htmlspecialchars($_GET['name']) : "you";

echo "<!DOCTYPE html>
<html>
<head><title>CGI Response</title></head>
<body>";

echo "<h1>Hello $name, this is PHP!</h1>";

echo "
</body>
</html>";

?>
