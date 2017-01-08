<?php
/**
 * @file
 * comment_delete.admin.inc
 */

/**
 * Page callback for comment deletion settings form.
 */
function comment_delete_settings_form() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings']['comment_delete_default'] = array(
    '#type' => 'radios',
    '#title' => t('Default Delete Option'),
    '#description' => t('Choose which option to use as the selected default when deleting comments.'),
    '#options' => array(
      0 => t('Delete comment and replies'),
      1 => t('Delete comment and move replies up'),
      2 => t('Delete comment and keep replies'),
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('comment_delete_default', 0),
  );
  $form['settings']['comment_delete_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Delete Threshold'),
    '#description' => t('Define allowable comment deletion threshold in seconds. Enter zero (0) to disable.'),
    '#size' => 10,
    '#default_value' => variable_get('comment_delete_threshold', 0),
  );
  $form['settings']['comment_delete_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Delete Confirmation Message'),
    '#description' => t('Enter message which is displayed after successfully removing comment.'),
    '#default_value' => variable_get('comment_delete_message', ''),
  );
  return system_settings_form($form);
}

/**
 * Validation callback for comment deletion settings form.
 */
function comment_delete_settings_form_validate($form, &$form_state) {

  // Check threshold is numerical value.
  if (!is_numeric($form_state['values']['comment_delete_threshold'])) {
    form_set_error('comment_delete_threshold', t('Threshold should be greater than or equal to zero (0).'));
  }

  // Check threshold is not negative.
  if ($form_state['values']['comment_delete_threshold'] < 0) {
    form_set_error('comment_delete_threshold', t('Threshold should be greater than or equal to zero (0).'));
  }

  // Check threshold does not include decimals.
  if (preg_match('/\./i', $form_state['values']['comment_delete_threshold'])) {
    form_set_error('comment_delete_threshold', t('Threshold should not include decimals.'));
  }
}
