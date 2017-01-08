<?php
/**
 * @file
 * supportgroups_default.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function supportgroups_default_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'anonymous_front';
  $context->description = 'Context for front page anonymous content';
  $context->tag = 'default';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'nice_menus-1' => array(
          'module' => 'nice_menus',
          'delta' => '1',
          'region' => 'header',
          'weight' => '-10',
        ),
        'nice_menus-1' => array(
          'module' => 'nice_menus',
          'delta' => '1',
          'region' => 'header',
          'weight' => '-10',
        ),
        'block-137' => array(
          'module' => 'block',
          'delta' => '137',
          'region' => 'header',
          'weight' => '-9',
        ),
        'user-login' => array(
          'module' => 'user',
          'delta' => 'login',
          'region' => 'header',
          'weight' => '-8',
        ),
        'views-master-block_1' => array(
          'module' => 'views',
          'delta' => 'master-block_1',
          'region' => 'content',
          'weight' => '-10',
        ),
        'supportgroups_user-supportgroups_register' => array(
          'module' => 'supportgroups_user',
          'delta' => 'supportgroups_register',
          'region' => 'sidebar_first',
          'weight' => '-53',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for front page anonymous content');
  t('default');
  $export['anonymous_front'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'anonymous_not_front';
  $context->description = 'Context for sitewide anonymous content';
  $context->tag = 'default';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~<front>' => '~<front>',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'nice_menus-1' => array(
          'module' => 'nice_menus',
          'delta' => '1',
          'region' => 'header',
          'weight' => '-10',
        ),
        'block-137' => array(
          'module' => 'block',
          'delta' => '137',
          'region' => 'header',
          'weight' => '-9',
        ),
        'user-login' => array(
          'module' => 'user',
          'delta' => 'login',
          'region' => 'header',
          'weight' => '-8',
        ),
        'supportgroups_user-supportgroups_register' => array(
          'module' => 'supportgroups_user',
          'delta' => 'supportgroups_register',
          'region' => 'closure',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for sitewide anonymous content');
  t('default');
  $export['anonymous_not_front'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'authenticated';
  $context->description = 'Context for sitewide authenticated content';
  $context->tag = 'default';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~admin*' => '~admin*',
      ),
    ),
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'supportgroups_pm-pm_notifications' => array(
          'module' => 'supportgroups_pm',
          'delta' => 'pm_notifications',
          'region' => 'header',
          'weight' => '0',
        ),
        'supportgroups_feed-supportgroups_feed_notifications' => array(
          'module' => 'supportgroups_feed',
          'delta' => 'supportgroups_feed_notifications',
          'region' => 'header',
          'weight' => '1',
        ),
        'search-form' => array(
          'module' => 'search',
          'delta' => 'form',
          'region' => 'header',
          'weight' => '5',
        ),
        'nice_menus-2' => array(
          'module' => 'nice_menus',
          'delta' => '2',
          'region' => 'header',
          'weight' => '4',
        ),
        'nice_menus-1' => array(
          'module' => 'nice_menus',
          'delta' => '1',
          'region' => 'header',
          'weight' => '3',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for sitewide authenticated content');
  t('default');
  $export['authenticated'] = $context;

  return $export;
}
