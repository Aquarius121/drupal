<?php
/**
 * @file
 * supportgroups_node.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function supportgroups_node_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function supportgroups_node_node_info() {
  global $base_url;
  $items = array(
    'group_post' => array(
      'name' => t('Discussion'),
      'base' => 'node_content',
      'description' => t('Start a new discussion, introduce yourself or ask a question. Start getting support today.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Start a new discussion, introduce yourself or ask a question. Start getting support today. <a href="' . SG_HTTP_PROTOCOL .'online'.str_replace(SG_HTTP_PROTOCOL, '', $base_url).'">Click here to join more Support Groups</a>.'),
    ),
    'set_up_group' => array(
      'name' => t('Set Up Support Group'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Support Group Name'),
      'help' => '',
    ),
  );
  return $items;
}
