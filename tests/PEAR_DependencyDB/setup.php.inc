<?php
error_reporting(E_ALL);
require_once "PEAR/DependencyDB.php";
require_once dirname(dirname(__FILE__)) . DIRECTORY_SEPARATOR . 'phpt_test.php.inc';

// ------------------------------------------------------------------------- //

function cleanall($dir = null)
{
    if ($dir !== null) {
        $statedir = $dir;
    } else {
        $statedir = $GLOBALS['statedir'];
    }
    if (file_exists($statedir)) {
        $dp = opendir($statedir);
        while ($ent = readdir($dp)) {
            if (in_array($ent, array('.', '..'))) {
                continue;
            }
            if (is_dir($statedir . DIRECTORY_SEPARATOR . $ent)) {
                cleanall($statedir . DIRECTORY_SEPARATOR . $ent);
                continue;
            }
            unlink($statedir . DIRECTORY_SEPARATOR . $ent);
        }
        closedir($dp);
        rmdir($statedir);
    }
}

function copyItem($name, $nested = false)
{
    global $php_dir;
    switch ($name) {
        case 'registry' :
            if ($nested) {
                $reg = $nested;
                @mkdir($copydir = $php_dir . DIRECTORY_SEPARATOR . '.registry' .
                    DIRECTORY_SEPARATOR . basename($nested));
            } else {
                $reg = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'tester' .
                    DIRECTORY_SEPARATOR . '.registry';
                @mkdir($copydir = $php_dir . DIRECTORY_SEPARATOR . '.registry');
            }
            if (file_exists($reg)) {
                $dp = opendir($reg);
                while ($ent = readdir($dp)) {
                    if (in_array($ent, array('.', '..'))) {
                        continue;
                    }
                    if (is_dir($reg . DIRECTORY_SEPARATOR . $ent)) {
                        copyItem($name, $reg . DIRECTORY_SEPARATOR . $ent);
                    } else {
                        copy($reg . DIRECTORY_SEPARATOR . $ent,
                             $copydir . DIRECTORY_SEPARATOR . $ent);
                    }
                }
            }
            closedir($dp);
            if (!$nested) {
                copyItem('channels');
            }
        break;
        case 'channels' :
            if ($nested) {
                $reg = $nested;
                @mkdir($copydir = $php_dir . DIRECTORY_SEPARATOR . '.channels' .
                    DIRECTORY_SEPARATOR . basename($nested));
            } else {
                $reg = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'tester' .
                    DIRECTORY_SEPARATOR . '.channels';
                @mkdir($copydir = $php_dir . DIRECTORY_SEPARATOR . '.channels');
            }
            if (file_exists($reg)) {
                $dp = opendir($reg);
                while ($ent = readdir($dp)) {
                    if (in_array($ent, array('.', '..'))) {
                        continue;
                    }
                    if (is_dir($reg . DIRECTORY_SEPARATOR . $ent)) {
                        copyItem($name, $reg . DIRECTORY_SEPARATOR . $ent);
                    } else {
                        copy($reg . DIRECTORY_SEPARATOR . $ent,
                             $copydir . DIRECTORY_SEPARATOR . $ent);
                    }
                }
            }
            closedir($dp);
        break;
        case 'filemap' :
            $reg = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'tester';
            copy($reg . DIRECTORY_SEPARATOR . '.filemap', $php_dir . DIRECTORY_SEPARATOR .
                '.filemap');
        break;
        case 'db' :
            $reg = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'tester';
            copy($reg . DIRECTORY_SEPARATOR . '.depdb', $php_dir . DIRECTORY_SEPARATOR .
                '.depdb');
        break;
    }
}

register_shutdown_function('cleanall');
$statedir = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'registry_tester';
cleanall();
$temp_path = $statedir;
if (!is_dir($temp_path)) {
    mkdir($temp_path);
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'php')) {
    mkdir($php_dir = $temp_path . DIRECTORY_SEPARATOR . 'php');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'data')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'data');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'doc')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'doc');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'test')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'test');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'ext')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'ext');
    $ext_dir = $temp_path . DIRECTORY_SEPARATOR . 'ext';
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'script')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'script');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'tmp')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'tmp');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'bin')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'bin');
}
if (!is_dir($temp_path . DIRECTORY_SEPARATOR . 'cache')) {
    mkdir($temp_path . DIRECTORY_SEPARATOR . 'cache');
}
// make the fake configuration - we'll use one of these and it should work
$config = serialize(array(
    'preferred_state' => 'stable',
    'cache_dir' => $temp_path . DIRECTORY_SEPARATOR . 'cache',
    'php_dir' => $temp_path . DIRECTORY_SEPARATOR . 'php',
    'ext_dir' => $temp_path . DIRECTORY_SEPARATOR . 'ext',
    'data_dir' => $temp_path . DIRECTORY_SEPARATOR . 'data',
    'doc_dir' => $temp_path . DIRECTORY_SEPARATOR . 'doc',
    'test_dir' => $temp_path . DIRECTORY_SEPARATOR . 'test',
    'bin_dir' => $temp_path . DIRECTORY_SEPARATOR . 'bin',));
$fp = fopen($statedir . DIRECTORY_SEPARATOR . 'pear.ini', 'wb');
fwrite($fp, $config, strlen($config));
fclose($fp);
putenv('PHP_PEAR_SYSCONF_DIR='.$statedir);
$home = getenv('HOME');
if (!empty($home)) {
    // for PEAR_Config initialization
    putenv('HOME="'.$statedir);
}
$config = &PEAR_Config::singleton($statedir . DIRECTORY_SEPARATOR . 'pear.ini');

$phpunit = &new PEAR_PHPTest(true);
?>