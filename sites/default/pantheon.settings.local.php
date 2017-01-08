<?php
ini_set('display_errors', 1);
// default settings.php
$update_free_access = FALSE;
$drupal_hash_salt = '';
//$cookie_domain = '.supportgroups.com';

ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 5184000);

$name = get_cfg_var('dbconfig.DB_USER');
$pass = get_cfg_var('dbconfig.DB_PASS');
$host = get_cfg_var('dbconfig.DB_HOST');


// default database settings needed for drush
$databases['default']['default'] = array(
  'driver' => 'mysql',
  'database' => 'supportgroups',
  'username' => $name,
  'password' => $pass,
  'host' => $host,
  'prefix' => '',
);


// ini_set('error_reporting', E_ALL);

// adding my own base_url
//$base_url = 'http://local.supportgroups.com';  // NO trailing slash!

// file proxy for local
//$conf['stage_file_proxy_origin'] = 'http://www.supportgroups.com';
//
//// Use Redis for caching.
//$conf['redis_client_interface'] = 'PhpRedis';
//$conf['cache_backends'][] = 'sites/all/modules/contrib/redis/redis.autoload.inc';
//$conf['cache_default_class'] = 'Redis_Cache';
//$conf['cache_prefix'] = array('default' => 'sg-redis');
//$conf['cache_host'] = '127.0.0.1';

// Add alternate handler for Page Cache
//$conf['cache_backends'][] = 'sites/all/modules/custom/supportgroups_page_cache/sgPageCacheRedis.inc';
//$conf['cache_class_cache_page'] = 'sgPageCacheRedis';
//$conf['cache_class_cache_field'] = 'Redis_Cache';


// LOCAL APP FOR LIVE DEEP DIVE ACCOUNT IN SALESFORCE
//$conf['salesforce_consumer_key'] = '3MVG9rFJvQRVOvk4X7FgMUGmVXe9_BBDT7qM4KRXVTcueb7ZCgP0OszXBeQN9hmQpgsLZH_x.kiMvYCDlTbX6';
//$conf['salesforce_consumer_secret'] = '1225422280565196968';

// DEV ACCOUNT SETTINGS
// $conf['salesforce_consumer_key'] = '3MVG9A2kN3Bn17hvKLbuslHwx0vhsVtAmpFsOHVsQ7622g_lVQmeEEIUR9NNGlMQe8H.N9KUTAU18fCkGEccJ';
// $conf['salesforce_consumer_secret'] = '8799962656409664001';


//// enable fast 404
//$conf['404_fast_paths_exclude'] = '/\/(?:styles|sitemap)\//';
//$conf['404_fast_paths'] = '/server-status|\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
//$conf['404_fast_html'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><meta name="description" content="SupportGroups 404 error page" /><title>404 Error - SupportGroups</title><style type="text/css">body {font-family: Arial, Helvetica, sans-serif;background:#239948;color: #ffffff;text-align: center;margin:0; } #container {width:80%;margin-top:2%; } .logo {margin:5% auto; } h1 {font-size:100%; }</style></head><body><div class="logo"><a href="http://www.supportgroups.com/user/register"><img src="http://www.supportgroups.com/sites/all/themes/skeletonsub/logo.png" width="250" height="35" border="0" alt="supportgroups logo" /></a></div><h1>Page not found (404 error)</h1><p>We\'re sorry but we can\'t find the page you were looking for.</p><p>Please try again, or <a href="http://www.supportgroups.com/user/register">follow this link to sign in or register</a>.</p></body></html>';
//drupal_fast_404();

$parts = explode('.', $_SERVER['HTTP_HOST']);
if ($parts[0] != 'www' && $parts[0] != 'supportgroups') {
  $subdomains = variable_get('supportgroups_default_subdomains', array());
  if (isset($subdomains[$_SERVER['HTTP_HOST']]) && $subdomains[$_SERVER['HTTP_HOST']] > 0) {
    header('HTTP/1.0 301 Moved Permanently');
    header('Location: http://www.supportgroups.com/node/' . $subdomains[$_SERVER['HTTP_HOST']]);
    exit();
  }
}