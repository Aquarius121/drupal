<?php
/**
 * Probably not needed
 *
 * @param $uid
 * @param $timestamp
 * @param $hashed_pass
 */
function supportgroups_user_auto_login($uid, $timestamp, $hashed_pass) {
  global $user;

  // When processing the one-time login link, we have to
  // make sure that a user isn't already logged in.
  if ($user->uid) {
    // The existing user is already logged in.
    if ($user->uid == $uid) {
      drupal_set_message(t('You are currently authenticated as user %user.', array('%user' => $user->name)) . ' ' . l(t('Change your password'), 'user/' . $user->uid . '/edit'));
      drupal_goto('user');
    }
    // A different user is already logged in on the computer.
    else {
      $reset_link_account = user_load($uid);
      if (!empty($reset_link_account)) {
        drupal_set_message(t('Another user (%other_user) is already authenticated to the site, but you tried to use a one-time link for user %resetting_user.', array('%other_user' => $user->name, '%resetting_user' => $reset_link_account->name)) . ' ' . t('Please !logout and try using the link again.', array('!logout' => l(t('logout'), 'user/logout'))));
      }
      else {
        // Invalid one-time link specifies an unknown user.
        drupal_set_message(t('The one-time login link you clicked is invalid.'), 'error');
      }
    }
  }
  else {
    // Time out, in seconds, until login URL expires. 24 hours = 86400 seconds.
    $timeout = 86400;
    $current = REQUEST_TIME;
    $timestamp_created = $timestamp - $timeout;

    $account = user_load($uid);
    $created = $user->created;
    // If this person has been created longer than the timeout, then assume this is a password reset
    if ($current > ($created + $timeout)) {
      supportgroups_user_reset_password($uid, $timestamp, $hashed_pass);
    }
    // Timestamp can not be larger then current.
    if ($timestamp_created <= $current && $account) {
      // Check if we have to enforce experation for activation links.

      // Else try to activate the account.
      // Password = user's password - timestamp = current request - login = username.
      // user_pass_rehash($password, $timestamp, $login, $uid)
      if ($account->uid && $timestamp >= $account->created && !$account->login && $hashed_pass == user_pass_rehash($account->pass, $timestamp, $account->name, $account->uid)) {
        // Format the date, so the logs are a bit more readable.
        $date = format_date($timestamp);
        watchdog('user', 'User %name used one-time login link at time %timestamp.', array('%name' => $account->name, '%timestamp' => $date));
        // Activate the user and update the access and login time to $current.
        $account = user_save($account, array(
            'status' => 1,
            'access' => $current,
            'login' => $current)
        );
        // Set the new user.
        $user = $account;
        // user_login_finalize() also updates the login timestamp of the
        // user, which invalidates further use of the one-time login link.
        user_login_finalize();

        // Display default welcome message.
        drupal_set_message(t('You have just used your one-time login link. Your account is now active and you are authenticated.'));
        // And just redirect to /user if this site does not have commerce,
        // or if the user does not have an order in the cart.
        drupal_goto('user');
      }
      // Something else is wrong, redirect to the password
      // reset form to request a new activation e-mail.
      else {
        drupal_set_message(t('You have tried to use a one-time login link that has either been used or is no longer valid. Please request a new one using the form below.'), 'error');
      }
    }
    else {
      // Deny access, no more clues.
      // Everything will be in the watchdog's URL for the administrator to check.
      drupal_access_denied();
      drupal_exit();
    }
  }

}

function supportgroups_user_reset_password($uid, $timestamp, $hashed_pass) {
  $form = drupal_get_form('user_password_reset', $uid, $timestamp, $hashed_pass);
}

function supportgroups_user_logintoboggan_resend_validation($account) {
  // Log user in if not already
  if (!logintoboggan_revalidate_access($account)) {
    global $user;
    $user = $account;
    drupal_session_regenerate();
  }
  module_load_include('inc', 'logintoboggan', 'logintoboggan.validation');
  logintoboggan_resend_validation($account);
}

/*
 * Redirect to help page
 */
function supportgroups_user_welcome_page() {
  drupal_goto('node/401646');
}