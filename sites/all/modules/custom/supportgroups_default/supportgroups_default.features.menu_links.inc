<?php
/**
 * @file
 * supportgroups_default.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function supportgroups_default_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-member-gear:http://#
  $menu_links['menu-member-gear:http://#'] = array(
    'menu_name' => 'menu-member-gear',
    'link_path' => SG_HTTP_PROTOCOL .'#',
    'router_path' => '',
    'link_title' => '.',
    'options' => array(
      'attributes' => array(
        'id' => 'gear',
        'class' => array(
          0 => 'gearitem',
        ),
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -50,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('.');


  return $menu_links;
}
