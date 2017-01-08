<?php
/**
 * @file
 * supportgroups_flags.features.inc
 */

/**
 * Implements hook_flag_default_flags().
 */
function supportgroups_flags_flag_default_flags() {
  $flags = array();
  // Exported flag: "Flag Comment".
  $flags['flag_comment'] = array(
    'content_type' => 'comment',
    'title' => 'Flag Comment',
    'global' => 1,
    'types' => array(
      0 => 'comment_node_group_post',
    ),
    'flag_short' => '&nbsp;',
    'flag_long' => 'Flag',
    'flag_message' => '',
    'unflag_short' => '&nbsp;',
    'unflag_long' => 'Unflag',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 2,
        1 => 3,
        2 => 16,
      ),
      'unflag' => array(
        0 => 2,
        1 => 3,
        2 => 16,
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_comment' => 1,
    'module' => 'supportgroups_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Flag Content".
  $flags['flag_content'] = array(
    'content_type' => 'node',
    'title' => 'Flag Content',
    'global' => 1,
    'types' => array(
      0 => 'group_post',
    ),
    'flag_short' => '&nbsp;',
    'flag_long' => 'Flag as Offensive',
    'flag_message' => '',
    'unflag_short' => '&nbsp;',
    'unflag_long' => 'Unflag',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 2,
        1 => 3,
        2 => 16,
      ),
      'unflag' => array(
        0 => 2,
        1 => 3,
        2 => 16,
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'supportgroups_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Flag User".
  $flags['flag_user'] = array(
    'content_type' => 'user',
    'title' => 'Flag User',
    'global' => 1,
    'types' => array(),
    'flag_short' => 'Flag',
    'flag_long' => 'Flag',
    'flag_message' => '',
    'unflag_short' => 'Unflag',
    'unflag_long' => 'Unflag',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 3,
        1 => 16,
      ),
      'unflag' => array(
        0 => 3,
        1 => 16,
      ),
    ),
    'weight' => 0,
    'show_on_form' => FALSE,
    'access_author' => '',
    'show_on_profile' => 1,
    'access_uid' => '',
    'module' => 'supportgroups_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Like Heart".
  $flags['like'] = array(
    'content_type' => 'node',
    'title' => 'Like Heart',
    'global' => 0,
    'types' => array(
      0 => 'group_post',
    ),
    'flag_short' => '&hearts;',
    'flag_long' => 'Empathize',
    'flag_message' => '',
    'unflag_short' => '&hearts;',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 2,
      ),
      'unflag' => array(
        0 => 2,
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'supportgroups_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Like Heart Comments".
  $flags['like_comments'] = array(
    'content_type' => 'comment',
    'title' => 'Like Heart Comments',
    'global' => 0,
    'types' => array(
      0 => 'comment_node_group_post',
    ),
    'flag_short' => '&hearts;',
    'flag_long' => 'Empathize',
    'flag_message' => '',
    'unflag_short' => '&hearts;',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 2,
      ),
      'unflag' => array(
        0 => 2,
      ),
    ),
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_comment' => 1,
    'module' => 'supportgroups_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}
