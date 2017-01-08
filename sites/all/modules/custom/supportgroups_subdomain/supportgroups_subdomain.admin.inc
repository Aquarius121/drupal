<?php

/**
 * @file
 *   Admin settings for subdomain
 */

function supportgroups_subdomain_admin_settings_form() {

  $form['supportgroups_environment'] = array(
    '#title' => 'Environment',
    '#description' => 'This value is set in settings.php but we have it here because this is important.  It controls
    whether or not email will be sent and subdomain behaviour',
    '#type' => 'select',
    '#options' => array('dev' => 'Dev', 'live' => 'Live', 'local' => 'Local'),
    '#default_value' => variable_get('supportgroups_environment', 'dev'),
  );

  return system_settings_form($form);

}