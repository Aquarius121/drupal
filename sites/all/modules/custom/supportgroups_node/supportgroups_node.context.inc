<?php
/**
 * @file
 * supportgroups_node.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function supportgroups_node_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'groups_page_context';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'set_up_group' => 'set_up_group',
      ),
      'options' => array(
        'node_form' => '0',
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
        'views-group_description-block' => array(
          'module' => 'views',
          'delta' => 'group_description-block',
          'region' => 'content_top',
          'weight' => '-9',
        ),
        'supportgroups_node-sg_group_post_block' => array(
          'module' => 'supportgroups_node',
          'delta' => 'sg_group_post_block',
          'region' => 'content_top',
          'weight' => '-8',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;
  $export['groups_page_context'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'groups_page_context_anonymous';
  $context->description = 'Groups page for anonymou users';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'set_up_group' => 'set_up_group',
      ),
      'options' => array(
        'node_form' => '0',
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
        'views-group_description-block' => array(
          'module' => 'views',
          'delta' => 'group_description-block',
          'region' => 'content_top',
          'weight' => '-10',
        ),
        'supportgroups_user-anonymous_discussion_form' => array(
          'module' => 'supportgroups_user',
          'delta' => 'anonymous_discussion_form',
          'region' => 'content_top',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Groups page for anonymou users');
  $export['groups_page_context_anonymous'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'profile_me';
  $context->description = 'SupportFeed';
  $context->tag = 'profile';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'current',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'supportgroups_node-sg_group_post_block' => array(
          'module' => 'supportgroups_node',
          'delta' => 'sg_group_post_block',
          'region' => 'content_top',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('SupportFeed');
  t('profile');
  $export['profile_me'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'profile_other';
  $context->description = 'Looking at someone\'s profile';
  $context->tag = 'profile';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'other',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-profile_other-block' => array(
          'module' => 'views',
          'delta' => 'profile_other-block',
          'region' => 'content_top',
          'weight' => '-10',
        ),
        'views-profile_latest_post-block' => array(
          'module' => 'views',
          'delta' => 'profile_latest_post-block',
          'region' => 'content_top',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Looking at someone\'s profile');
  t('profile');
  $export['profile_other'] = $context;

  return $export;
}
