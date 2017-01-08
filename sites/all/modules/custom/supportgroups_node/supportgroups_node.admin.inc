<?php

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function supportgroups_node_groups_admin_settings_form($form_state) {
  $form['groups']['supportgroups_node_no_media_for_groups_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Block Media For Groups'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );

  $form['groups']['supportgroups_node_no_media_for_groups_wrapper']['supportgroups_node_no_media_for_groups_message'] = array(
      '#title' => t('Blocked Media Message'),
      '#type' => 'textfield',
      '#size' => 100,
      '#default_value' => variable_get('supportgroups_node_no_media_for_groups_message', 'Sorry, images and videos are not allowed in the [group_name] group'),
      '#description' => t('Message to the user when they have selected a group that has blocked media. [group_name] is a token that will be replaced with the actual group name.'),
  );

  $groups = supportgroups_feed_get_group_names();
  $form['groups']['supportgroups_node_no_media_for_groups_wrapper']['supportgroups_node_no_media_for_groups'] = array(
      '#title' => t('Block media in discussions for the following groups'),
      '#type' => 'checkboxes',
      '#options' => $groups,
      '#default_value' => variable_get('supportgroups_node_no_media_for_groups', array()),
      '#description' => t('Select all groups that should have the ability to attach media to a discussion blocked.'),
  );

  return system_settings_form($form);
}