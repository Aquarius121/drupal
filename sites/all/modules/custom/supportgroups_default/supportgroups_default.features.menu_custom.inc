<?php
/**
 * @file
 * supportgroups_default.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function supportgroups_default_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-member-gear.
  $menus['menu-member-gear'] = array(
    'menu_name' => 'menu-member-gear',
    'title' => 'Member Gear',
    'description' => 'gear dropdown for member functions',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Member Gear');
  t('gear dropdown for member functions');


  return $menus;
}
