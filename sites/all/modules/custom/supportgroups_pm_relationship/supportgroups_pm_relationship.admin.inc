<?php

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function supportgroups_pm_relationship_admin_form($form_state) {

  // Contains the list of users who can private message all users
  $form['supportgroups_pm_relationship_admin_user_list'] = array(
      '#title' => t('Users who can access all private messages'),
      '#type' => 'textarea',
      '#rows' => 3,
      '#cols' => 10,
      '#default_value' => variable_get('supportgroups_pm_relationship_admin_user_list', ''),
      '#description' => t('This is the list of users who can receive and send messages to any
        user.  Use commas as delimiters.'),
  );
  return system_settings_form($form);
}