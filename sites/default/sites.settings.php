<?php

if (isset($_SERVER['PANTHEON_ENVIRONMENT']) &&
  ($_SERVER['PANTHEON_ENVIRONMENT'] === 'live') &&
  (php_sapi_name() != "cli")) {
  if ($_SERVER['HTTP_HOST'] == 'serverinfo.supportgroups.com') {
    header('HTTP/1.0 301 Moved Permanently');
    header('Location: http://www.supportgroups.com/node/391342');
    exit();
  }
}

// default settings.php
$update_free_access = FALSE;
$drupal_hash_salt = '';
$cookie_domain = '.supportgroups.com';

ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 5184000);
ini_set('display_errors', 1);



// default database settings needed for drush
$databases['default']['default'] = array(
  'driver' => 'mysql',
  'database' => '',
  'username' => '',
  'password' => '',
  'host' => 'localhost',
  'prefix' => '',
);



$databases['default']['default']['database'] = 'support_groups';
$databases['default']['default']['username'] = 'root';
$databases['default']['default']['password'] = 'root';

/**
 * LOCALHOST
 */
// Check for local development and configure overrides
// if (!isset($_SERVER['PANTHEON_ENVIRONMENT'])) {
if (!defined('PANTHEON_ENVIRONMENT')) {

  // default database settings needed for drush
  $databases['default']['default'] = array(
    'driver' => 'mysql',
    'database' => '',
    'username' => '',
    'password' => '',
    'host' => 'localhost',
    'prefix' => '',
  );

  $conf['mail_system'] = array(
    'default-system' => 'DevelMailLog',
  );

  $conf['image_allow_insecure_derivatives'] = TRUE;
  $conf['css_gzip_compression'] = FALSE;
  $conf['js_gzip_compression'] = FALSE;

  $conf['preprocess_js'] = 0;
  $conf['preprocess_css'] = 0;

  $conf['cache'] = 0;
  $conf['block_cache'] = 0;

  // override salesforce
  // add conf for Salesforce API
  // This will override live settings after a data migration. The refresh_token will still need to be reset
  // DEV APP SETTINGS FOR CRIFKIN DEV APP
  $conf['salesforce_consumer_key'] = '3MVG9A2kN3Bn17hvKLbuslHwx0oO5ZBSti96ElcBnW5g7iyqIkZuwyyxQEgK03fnJYV.5sCRAEr9zEptFIOBH';
  $conf['salesforce_consumer_secret'] = '5981110820495634192';

  // override for local devs
  if (!isset($_SERVER['DEVELOPER'])) {
    $_SERVER['DEVELOPER'] = $_SERVER['USER'];
  }

  // Add developer overrides for local development.
  // In order for this to work, you must update your apache/nginx server configuration
  // to include the DEVELOPER environment var
  if (isset($_SERVER['DEVELOPER'])) {
    switch(strtolower($_SERVER['DEVELOPER'])) {
      case 'steven':
        ini_set('error_reporting', E_ALL);

        // adding my own base_url
        $base_url = 'http://local.supportgroups.com';  // NO trailing slash!

        $databases['default']['default']['database'] = 'sg';
        $databases['default']['default']['username'] = 'root';
        $databases['default']['default']['password'] = 'root';

        $cookie_domain = 'local.supportgroups.com';

        // file proxy for local
        $conf['stage_file_proxy_origin'] = 'http://www.supportgroups.com';

        // Use Redis for caching.
        $conf['redis_client_interface'] = 'PhpRedis';
        $conf['cache_backends'][] = 'sites/all/modules/contrib/redis/redis.autoload.inc';
        $conf['cache_default_class'] = 'Redis_Cache';
        $conf['cache_prefix'] = array('default' => 'sg-redis');
        $conf['cache_host'] = '127.0.0.1';

        // Add alternate handler for Page Cache
        $conf['cache_backends'][] = 'sites/all/modules/custom/supportgroups_page_cache/sgPageCacheRedis.inc';
        $conf['cache_class_cache_page'] = 'sgPageCacheRedis';
        $conf['cache_class_cache_field'] = 'Redis_Cache';


        // LOCAL APP FOR LIVE DEEP DIVE ACCOUNT IN SALESFORCE
        $conf['salesforce_consumer_key'] = '3MVG9rFJvQRVOvk4X7FgMUGmVXe9_BBDT7qM4KRXVTcueb7ZCgP0OszXBeQN9hmQpgsLZH_x.kiMvYCDlTbX6';
        $conf['salesforce_consumer_secret'] = '1225422280565196968';

        // DEV ACCOUNT SETTINGS
        // $conf['salesforce_consumer_key'] = '3MVG9A2kN3Bn17hvKLbuslHwx0vhsVtAmpFsOHVsQ7622g_lVQmeEEIUR9NNGlMQe8H.N9KUTAU18fCkGEccJ';
        // $conf['salesforce_consumer_secret'] = '8799962656409664001';

        break;
      case 'cmceldowney':

        // Use Redis for caching.
        $conf['redis_client_interface'] = 'PhpRedis';
        $conf['cache_backends'][] = 'sites/all/modules/contrib/redis/redis.autoload.inc';
        $conf['cache_default_class'] = 'Redis_Cache';
        $conf['cache_prefix'] = array('default' => 'pantheon-redis');

        // Add alternate handler for Page Cache
        $conf['cache_backends'][] = 'sites/all/modules/custom/supportgroups_page_cache/sgPageCacheRedis.inc';
        $conf['cache_class_cache_page'] = 'sgPageCacheRedis';
        $conf['cache_class_cache_field'] = 'Redis_Cache';


        // commenting out the form cache for the DB due to the heavy load
        //$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

        $databases = array (
          'default' =>
            array (
              'default' =>
                array (
                  'database' => 'sg',
                  'username' => 'root',
                  'password' => '',
                  'host' => 'localhost',
                  'port' => '',
                  'driver' => 'mysql',
                  'prefix' => '',
                ),
            ),
        );

        ini_set('error_reporting', E_ALL);
        $cookie_domain = 'local.supportgroups.com';
        $conf['mail_system'] = array( 'default-system' => 'DevelMailLog', );
        break;

      case 'kevinkhuat':
        $databases['default']['default']['database'] = 'support_groups';
        $databases['default']['default']['username'] = 'root';
        $databases['default']['default']['password'] = 'root';
        break;

      // Copy the following settings, uncomment, and change the values
      // case 'YOUR_NAME':
      // 	add you settings
      // 	$databases['default']['default']['database'] = 'support_groups';
      // 	$databases['default']['default']['username'] = 'root';
      // 	$databases['default']['default']['password'] = 'root';
      // 	break;
    }
  }
} else { // we are on Pantheon
  // begin with pantheon environment default

  // Use Redis for caching.
  $conf['redis_client_interface'] = 'PhpRedis';
  $conf['cache_backends'][] = 'sites/all/modules/contrib/redis/redis.autoload.inc';
  $conf['cache_default_class'] = 'Redis_Cache';
  $conf['cache_prefix'] = array('default' => 'pantheon-redis');

  // Add alternate handler for Page Cache
  $conf['cache_backends'][] = 'sites/all/modules/custom/supportgroups_page_cache/sgPageCacheRedis.inc';
  $conf['cache_class_cache_page'] = 'sgPageCacheRedis';

  // commenting out the form cache for the DB due to the heavy load
  //$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

  // add comments to the indexing for nodes-- better site search results on keywords
  $conf['apachesolr_index_comments_with_node'] = TRUE;

  switch($_SERVER['PANTHEON_ENVIRONMENT']) {
    case 'dev':
      $base_url = 'http://dev.supportgroups.com';  // NO trailing slash!

      // refs #1742 -- override Redis cache in favor of DB
      // to see if Redis is the form 550 issue
      // $conf[‘cache_class_cache_form’] = ‘DrupalDatabaseCache’;

      // use devel for mailing outside of prod
      $conf['mail_system'] = array(
        'default-system' => 'DevelMailLog',
      );
      break;

    case 'test':
      // disable warnings
      ini_set('error_reporting', E_ALL & ~(E_STRICT | E_NOTICE | E_WARNING));

      $base_url = 'http://test.supportgroups.com';  // NO trailing slash!

      // refs #1742 -- override Redis cache in favor of DB
      // to see if Redis is the form 550 issue
      // $conf[‘cache_class_cache_form’] = ‘DrupalDatabaseCache’;

      // use devel for mailing outside of prod
      $conf['mail_system'] = array(
        'default-system' => 'DevelMailLog',
      );

      // add conf for Salesforce API
      // TEST APP SETTINGS FOR DEEP DIVE ACCOUNT IN SALESFORCE
      $conf['salesforce_consumer_key'] = '3MVG9rFJvQRVOvk4X7FgMUGmVXZikMbqrByiHql1rhlJcvFJaqA5ovhISoJeQ3qP9mJiyZy40RATpOGCYcs_Q';
      $conf['salesforce_consumer_secret'] = '7086437157220210141';
      break;

    case 'live': // This is the live Panteon environment
      // disable warnings
      ini_set('error_reporting', E_ALL & ~(E_STRICT | E_NOTICE | E_WARNING));

      $base_url = 'http://www.supportgroups.com';  // NO trailing slash!

      // refs #1742 -- override Redis cache in favor of DB
      // to see if Redis is the form 550 issue
      // $conf[‘cache_class_cache_form’] = ‘DrupalDatabaseCache’;

      // if this is not the base domain of the env then redirect
      $parts = explode('.', $_SERVER['HTTP_HOST']);
      if ($parts[0] != 'www' && $parts[0] != 'supportgroups') {
        $subdomains = variable_get('supportgroups_default_subdomains', array());
        if (isset($subdomains[$_SERVER['HTTP_HOST']]) && $subdomains[$_SERVER['HTTP_HOST']] > 0) {
          header('HTTP/1.0 301 Moved Permanently');
          header('Location: http://www.supportgroups.com/node/' . $subdomains[$_SERVER['HTTP_HOST']]);
          exit();
        }
      }
      if($_SERVER['HTTP_HOST'] == 'supportgroups.com' && isset($_SERVER['PANTHEON_ENVIRONMENT'])) {
        header('HTTP/1.0 301 Moved Permanently');
        header('Location: ' . $base_url . $_SERVER['REQUEST_URI']);
        exit();
      }

      // LIVE APP SETTINGS FOR DEEP DIVE ACCOUNT IN SALESFORCE
      $conf['salesforce_consumer_key'] = '3MVG9rFJvQRVOvk4X7FgMUGmVXVGRBiJEfJYTIXcphxhcZZ3F3kD9F_ctxAJc.Cb8rfN2CUthBrSLL._m5H2f';
      $conf['salesforce_consumer_secret'] = '1180454893930789863';
      break;

    case 'viewrefctr':
      $base_url = 'http://viewrefctr-supportgroups.pantheonsite.io';  // NO trailing slash!
      $cookie_domain = '.viewrefctr-supportgroups.pantheonsite.io';

      // use devel for mailing outside of prod
      $conf['mail_system'] = array(
        'default-system' => 'DevelMailLog',
      );
      break;
  }
}

$config['user_password_reset_timeout'] = 86400;

// enable fast 404
$conf['404_fast_paths_exclude'] = '/\/(?:styles|sitemap)\//';
$conf['404_fast_paths'] = '/server-status|\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$conf['404_fast_html'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><meta name="description" content="SupportGroups 404 error page" /><title>404 Error - SupportGroups</title><style type="text/css">body {font-family: Arial, Helvetica, sans-serif;background:#239948;color: #ffffff;text-align: center;margin:0; } #container {width:80%;margin-top:2%; } .logo {margin:5% auto; } h1 {font-size:100%; }</style></head><body><div class="logo"><a href="http://www.supportgroups.com/user/register"><img src="http://www.supportgroups.com/sites/all/themes/skeletonsub/logo.png" width="250" height="35" border="0" alt="supportgroups logo" /></a></div><h1>Page not found (404 error)</h1><p>We\'re sorry but we can\'t find the page you were looking for.</p><p>Please try again, or <a href="http://www.supportgroups.com/user/register">follow this link to sign in or register</a>.</p></body></html>';
drupal_fast_404();
