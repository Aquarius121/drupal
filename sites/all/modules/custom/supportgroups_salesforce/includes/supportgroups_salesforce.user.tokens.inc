<?php

/**
 * Helper function to build user tokens for salesforce integration.
 **/
function _supportgroups_salesforce_user_token_info() {
  $supportgroups_salesforce_user = array();

  $supportgroups_salesforce_user['support-groups-gender'] = array(
    'name' => t('Profile gender'),
    'description' => t("Provides profile gender if set"),
  );

  $supportgroups_salesforce_user['support-groups-profile-pic-path'] = array(
    'name' => t('Profile picture url'),
    'description' => t("Provides profile pic url if exists"),
  );

  $supportgroups_salesforce_user['support-groups-points'] = array(
    'name' => t("User Points"),
    'description' => t("User points."),
  );

  $supportgroups_salesforce_user['support-groups-birthday'] = array(
    'name' => t("User Birthday"),
    'description' => t("User birthday in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-last-login'] = array(
    'name' => t("User Last Login"),
    'description' => t("User login in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-last-access'] = array(
    'name' => t("User Last Access"),
    'description' => t("User last accessed in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-created'] = array(
    'name' => t("User Created"),
    'description' => t("User created in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-supports'] = array(
    'name' => t("User Supports"),
    'description' => t("The number of other users supported by this user"),
  );

  $supportgroups_salesforce_user['support-groups-last-supports-time'] = array(
    'name' => t("User Last Support Time"),
    'description' => t("Last time this user supported another user in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-supported-by'] = array(
    'name' => t("User Supported By"),
    'description' => t("The number of users who are supporting this user"),
  );

  $supportgroups_salesforce_user['support-groups-last-supported-by-time'] = array(
    'name' => t("User Last Supported By Time"),
    'description' => t("Last time another user supported this user in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-authored'] = array(
    'name' => t("User Authored"),
    'description' => t("The number of discusssion nodes authored by this user."),
  );

  $supportgroups_salesforce_user['support-groups-last-authored-time'] = array(
    'name' => t("User Last Authored Time"),
    'description' => t("Last time this user authored a discussion in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-comments-count'] = array(
    'name' => t("User Comments Count"),
    'description' => t("The number of comments authored by this user"),
  );

  $supportgroups_salesforce_user['support-groups-last-comment-time'] = array(
    'name' => t("User Last Comment Time"),
    'description' => t("Last time this user authored a comment in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-comment-replies-count'] = array(
    'name' => t("User Comments Count"),
    'description' => t("The number of replies authored on comments by this user"),
  );

  $supportgroups_salesforce_user['support-groups-last-comment-reply-time'] = array(
    'name' => t("User Last Comment Reply Time"),
    'description' => t("Last time this user authored a replt to a comment in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-hearts-count'] = array(
    'name' => t("User Hearts Count"),
    'description' => t("The number of hearts created by this user"),
  );

  $supportgroups_salesforce_user['support-groups-last-heart-time'] = array(
    'name' => t("User Last Heart Time"),
    'description' => t("Last time this user created a heart in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-private-messages-count'] = array(
    'name' => t("User Private Messages Count"),
    'description' => t("The number of private messages created by this user"),
  );

  $supportgroups_salesforce_user['support-groups-last-private-message-time'] = array(
    'name' => t("User Created"),
    'description' => t("Last time this user created a private message in Salesforce format. YYYY-MM-DDT00:00:00"),
  );

  $supportgroups_salesforce_user['support-groups-groups-list'] = array(
    'name' => t("User Group List"),
    'description' => t("List of user memberships."),
  );

  $supportgroups_salesforce_user['support-groups-groups-gids'] = array(
    'name' => t("User Groups Gids"),
    'description' => t("User membership ids separated by commas."),
  );

  return $supportgroups_salesforce_user;
}

/**
 * Implements hook_tokens().
 */
function _supportgroups_salesforce_user_tokens($tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  $account = $data['user'];
  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'support-groups-gender':
        $gender = NULL;
        if(is_object($account) && $account->profile_gender) {
          $gender = $account->profile_gender;
        }
        $replacements[$original] = $gender;
        break;
      case 'support-groups-profile-pic-path':
        $profile_pic_path = '';
        if(is_object($account) && $account->picture) {
          if(is_object($account->picture)) {
            $profile_pic_path = file_create_url($account->picture->uri);
          } elseif(is_numeric($account->picture)) {
            $file = file_load($account->picture);
            $profile_pic_path = file_create_url($file->uri);
          }
        }
        $replacements[$original] = $profile_pic_path;
        break;

      case 'support-groups-points':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->points;
        break;

      case 'support-groups-birthday':
        $birthday = NULL;
        if(!empty($account->profile_birthday) && $account->profile_birthday) {
          $date = $account->profile_birthday;
          if(!is_array($account->profile_birthday)) {
            $date = unserialize($date);
          }

          if(is_array($date)) {
            $birthday = date('c', mktime(0, 0, 0, $date['month'], $date['day'], $date['year']));
          }

        }

        $replacements[$original] = $birthday;
        break;

      case 'support-groups-last-login':
        $replacements[$original] = $account->login ? date('c', $account->login) : NULL;
        break;

      case 'support-groups-last-access':
        $replacements[$original] = $account->access ? date('c', $account->access) : NULL;
        break;

      case 'support-groups-created':
        $replacements[$original] = date('c', $account->created);
        break;

      case 'support-groups-supports':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->supports;
        break;

      case 'support-groups-last-supports-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_supports_time) {
          $value = date('c', $user_points->last_supports_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-supported-by':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->supported_by;
        break;

      case 'support-groups-last-supported-by-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_supported_by_time) {
          $value = date('c', $user_points->last_supported_by_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-authored':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->authored_count;
        break;

      case 'support-groups-last-authored-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_authored_time) {
          $value = date('c', $user_points->last_authored_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-comments-count':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->comments_count;
        break;

      case 'support-groups-last-comment-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_comment_time) {
          $value = date('c', $user_points->last_comment_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-comment-replies-count':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->comment_replies_count;
        break;

      case 'support-groups-last-comment-reply-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_comment_reply_time) {
         $value = date('c', $user_points->last_comment_reply_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-hearts-count':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->hearts_count;
        break;

      case 'support-groups-last-heart-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_heart_time) {
          $value = date('c', $user_points->last_heart_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-private-messages-count':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $replacements[$original] = $user_points->private_messages_count;
        break;

      case 'support-groups-last-private-message-time':
        $user_points = _supportgroups_salesforce_load_user_points_entity($account->uid);
        $value = NULL;
        if($user_points->last_private_message_time) {
          $value = date('c', $user_points->last_private_message_time);
        }
        $replacements[$original] = $value;
        break;

      case 'support-groups-groups-list':
        $groups_list = NULL;
        $ids = _supportgroups_salesforce_get_gids_from_account($account);

        // if we have ids then get titles and create list of group titles
        if(count($ids)){
          // sort them by most popular
         $sorted_ids = _supportgroups_salesforce_sort_user_groups($account->uid, $ids);

         if(count($sorted_ids)) {
           $groups = array();

           // get all titles from cache
           $titles = supportgroups_feed_get_group_names();

           // build array of titles
           foreach($sorted_ids as $id) {
             $groups[$id] = $titles[$id];
           }

           // format the list for salesforce return
           $groups_list = implode(";", $groups);
         }
        }
        $replacements[$original] = $groups_list;
        break;

      case 'support-groups-groups-gids':
        $gids = NULL;
        $ids = _supportgroups_salesforce_get_gids_from_account($account);

        if(count($ids)){
          // sort the gids to most popular
          $sorted_ids = _supportgroups_salesforce_sort_user_groups($account->uid, $ids);
          if(count($sorted_ids)) {
            $gids = implode(',', $sorted_ids);
          }
        }

        $replacements[$original] = $gids;
        break;
    }
  }

  return $replacements;
}

/**
 * Helper function to load or create a user points entity
 * @param integer $uid
 */
function _supportgroups_salesforce_load_user_points_entity($uid = 0) {
  static $user_point_objects = array();

  if(isset($user_point_objects[$uid])) {
    return $user_point_objects[$uid];
  }

  $user_points = user_points_load(NULL, array('uid' => $uid));

  if($user_points->user_points_id) {
    $user_point_objects[$uid] = $user_points;
    return $user_points;
  }

  // create a new user points record if none exists
  $user_points = entity_get_controller('user_points')->create($uid);

  // save the user points and force the calculations to update
  $user_points = entity_get_controller('user_points')->save($user_points, TRUE);

  // set the points onto the static array
  $user_point_objects[$uid] = $user_points;

  // return the points to the calling function
  return $user_points;
}

function _supportgroups_salesforce_get_gids_from_account($account) {
  $gids = array();
  foreach($account->og_group_ref['und'] as $group) {
    $gids[$group['target_id']] = $group['target_id'];
  }
  return $gids;
}

function _supportgroups_salesforce_sort_user_groups($uid, $gids = array()) {
  // set up empty array for return
  static $sorted_user_gids = array();

  if(isset($sorted_user_gids[$uid])) {
    return $sorted_user_gids[$uid];
  }

  // get cached list of groups with number of posts
  $groups = supportgroups_views_get_group_activity_count('all');

  // reverse osrt the groups with most popular on top
  arsort($groups);

  // loop the list and if the key is in our gids append to new array for a sorted list
  foreach($groups as $gid => $posts) {
    if(in_array($gid, $gids)) {
      $sorted_user_gids[$uid][] = $gid;
    }
  }

  return $sorted_user_gids[$uid];
}