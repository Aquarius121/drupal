<?php
// $Id$
/**
 * @file
 * Contains all ajax helper functions
 */

/* ***************************************
 * AJAX helper function
 * ***************************************/

/**
 * Wrapper AJAX method around supportgroups_feed_get_recent_activity_for_uid
 * Params:
 * uid : optional -- if not passed, use current $user object uid
 * from : optional baseline timestamp. If not specified will simply return the most recent X items
 * max : optional maximum number of rows to return
 */
function _supportgroups_feed_ajax_get_recent_activity() {
  $uid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $from_timestamp = isset($_REQUEST['from']) ? $_REQUEST['from'] : 0;
  $num_rows = isset($_REQUEST['max']) ? $_REQUEST['max'] : SUPPORTGROUPS_FEED_DEFAULT_NUM_RECORDS;
  $data = supportgroups_feed_get_recent_activity_for_uid($uid, $from_timestamp, $num_rows);

  if (isset($data['activity']) && is_array($data['activity'])) {
    // prepare themed_output to return a keyed array of themed items to insert onto the page
    foreach($data['activity'] as $timestamp => $item) {
      // override the index to prevent v8 in chrome and opera from sorting the indexed node ids
      $index = SUPPORTGROUPS_FEED_INDEX_OVERRIDE . $item->nid;
      
      $data['themed_output'][$index][] = theme('sg_f_activity_item', array('activity_item' => $item, 'users' => $data['users'], 'time' => $timestamp));
    }
  }

  //remove unused raw activity and users info we want payload as small as possible
  unset($data['activity']);
  unset($data['users']);

  $data['interval'] = supportgroups_feed_get_ajax_update_interval();

  drupal_json_output($data);
  exit;
  //drupal_exit();
}

/**
 * Wrapper AJAX method around supportgroups_feed_get_recent_activity_for_group
 * Params:
 * uid : optional -- if not passed, use current $user object uid
 * from : optional baseline timestamp. If not specified will simply return the most recent X items
 * max : optional maximum number of rows to return
 */
function _supportgroups_feed_ajax_get_recent_activity_group() {
  $gid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $from_timestamp = isset($_REQUEST['from']) ? $_REQUEST['from'] : 0;
  $num_rows = isset($_REQUEST['max']) ? $_REQUEST['max'] : SUPPORTGROUPS_FEED_DEFAULT_NUM_RECORDS;
  $data = supportgroups_feed_get_recent_activity_for_group($gid, $from_timestamp, $num_rows);

  // prepare themed_output to return a keyed array of themed items to insert onto the page
  $data['themed_output'] = array();
  foreach($data['activity'] as $timestamp => $item) {
    // override the index to prevent v8 in chrome and opera from sorting the indexed node ids
    $index = SUPPORTGROUPS_FEED_INDEX_OVERRIDE . $item->nid;
    
    $data['themed_output'][$index][] = theme('sg_f_activity_item', array('activity_item' => $item, 'users' => $data['users'], 'time' => $timestamp));
  }

  //remove unused raw activity and users info we want payload as small as possible
  unset($data['activity']);
  unset($data['users']);

  $data['interval'] = supportgroups_feed_get_ajax_update_interval();
  drupal_json_output($data);
  exit;
  //drupal_exit();
}

/**
 * Wrapper AJAX method around supportgroups_feed_get_past_activity_for_group
 * Params:
 * uid : optional -- if not passed, use current $user object uid
 * before : optional baseline timestamp. If not specified will simply return the most recent X items
 * max : optional maximum number of rows to return
 */
function _supportgroups_feed_ajax_get_past_activity_group() {
  global $user;
  $gid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $before_timestamp = isset($_REQUEST['before']) ? $_REQUEST['before'] : time();
  $num_rows = isset($_REQUEST['max']) ? $_REQUEST['max'] : SUPPORTGROUPS_FEED_DEFAULT_NUM_RECORDS;
  $data = supportgroups_feed_get_past_activity_for_group($gid, $before_timestamp, $num_rows);
  // prepare themed_output to return a keyed array of themed items to insert onto the page
  $data['themed_output'] = array();
  foreach($data['activity'] as $timestamp => $item) {
    if( $user->uid > 0 ){
      $theme_output = theme('sg_f_activity_item',array('activity_item' => $item, 'users' => $data['users'], 'time' => $timestamp));
    } else{
      $theme_output = theme('group_anonymous_item',array('activity_item' => $item, 'users' => $data['users'], 'time' => $timestamp));
    }
    
    // override the index to prevent v8 in chrome and opera from sorting the indexed node ids
    $index = SUPPORTGROUPS_FEED_INDEX_OVERRIDE . $item->nid;

    $data['themed_output'][$index][] = str_replace(array('“','”'),array('',''), $theme_output,0,100));

  }

  //remove unused raw activity and users info we want payload as small as possible
  unset($data['activity']);
  unset($data['users']);
  
  $data['interval'] = supportgroups_feed_get_ajax_update_interval();
  drupal_json_output($data);
  exit;
  //drupal_exit();
}

/**
 * Wrapper AJAX method around supportgroups_feed_get_past_activity_for_uid
 * Params:
 * uid : optional -- if not passed, use current $user object uid
 * before : optional baseline timestamp. If not specified will simply return the most recent X items
 * max : optional maximum number of rows to return
 */
function _supportgroups_feed_ajax_get_past_activity() {
  $uid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $before_timestamp = isset($_REQUEST['before']) ? $_REQUEST['before'] : time();
  $num_rows = isset($_REQUEST['max']) ? $_REQUEST['max'] : SUPPORTGROUPS_FEED_DEFAULT_NUM_RECORDS;
  $data = supportgroups_feed_get_past_activity_for_user($uid, $before_timestamp, $num_rows);

  // prepare themed_output to return a keyed array of themed items to insert onto the page
  $data['themed_output'] = array();
  foreach($data['activity'] as $timestamp => $item) {
    // override the index to prevent v8 in chrome and opera from sorting the indexed node ids
    $index = SUPPORTGROUPS_FEED_INDEX_OVERRIDE . $item->nid;
    
    $data['themed_output'][$index][] = theme('sg_f_activity_item', array('activity_item' => $item, 'users' => $data['users'], 'time' => $timestamp));
  }

  //remove unused raw activity and users info we want payload as small as possible
  unset($data['activity']);
  unset($data['users']);

  $data['interval'] = supportgroups_feed_get_ajax_update_interval();

  drupal_json_output($data);
  exit;
//  drupal_exit();
}

/**
 * Wrapper AJAX method around supportgroups_feed_get_comments_for_nid
 */
function _supportgroups_feed_ajax_get_comments() {
  $comments = array();
  $nid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $discussion_page = isset($_REQUEST['discussion_page']) ? $_REQUEST['discussion_page'] : 0;
  $themed_output = "";
  $users = array();
  if ($nid) {
    $comments = supportgroups_feed_get_comments_for_nid($nid);
    $users = _supportgroups_feed_get_user_info_for_comments($comments);

    $theme_options = array(
      'comments' => $comments,
      'users' => $users,
    );

    // if this is a discussion page, then run a different template
    if( $discussion_page) {
      $theme_options['page'] = 'group_post';
    }

    $themed_output = theme('sg_f_comments', $theme_options);
  }

  $data = array(
    'themed_output' => utf8_encode($themed_output),
    'users' => $users,
  );

  drupal_json_output($data);
  exit;
  //drupal_exit();
}

/**
 * Wrapper AJAX method around supportgroups_feed_get_user_info_multi
 * Pass in an array of user Ids and returns relevant info for the user(s)
 */
function _supportgroups_feed_ajax_get_user_info() {
  $users = array();
  $uids = isset($_REQUEST['uids']) ? $_REQUEST['uids'] : 0;
  if (!is_array($uids)) {
    $uids = array($uids);
  }

  drupal_json_output(array('users' => supportgroups_feed_get_user_info_multi($uids)));
  exit;
  //drupal_exit();
}

/**
 * Wrapper AJAX method around supportgroups_feed_get_recent_activity_notifications_for_uid
 */
function _supportgroups_feed_ajax_get_recent_activity_notifications() {
  $uid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $since = isset($_REQUEST['since']) ? $_REQUEST['since'] : time();
  $num_rows = variable_get('supportgroups_feed_number_notifications', 8);

  $data = supportgroups_feed_get_recent_activity_notifications_for_uid($uid, $since, $num_rows);
//  watchdog('notification', 'REQUEST <pre>' . print_r($_REQUEST,1));
//  watchdog('notification', 'since <pre>' . date('d m Y', $_REQUEST['since']));

//  $data['interval'] = supportgroups_feed_get_ajax_update_interval();

  $data['themed_output'] = _supportgroups_feed_get_notifications($data['activity_count'], $data['activity'],
    $data['users'], $data['more']);

  //remove unused raw activity and users info we want payload as small as possible
  unset($data['activity']);
  unset($data['users']);
  unset($data['more']);

  drupal_json_output($data);
  exit;
  //drupal_exit();
}

function _supportgroups_feed_ajax_get_unseen_notifications_count() {
  $uid = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;
  $notifications_unseen = supportgroups_user_get_notifications_unseen($uid);
  drupal_json_output($notifications_unseen);
  exit;

}
/**
 * Ajax method to remove discussion
 */
function _supportgroups_feed_ajax_remove_discussion( $nid = 0 ){
  global $user;
  $status = 'false';
  $message = '';
  $uid = 0;
  // Check to see if the nid is not equal to 0
  if ($nid) {
    // Find the author uid of the discussion
    $result = db_query("SELECT uid FROM {node}
      WHERE nid = :nid", array( ':nid' => $nid ) );
    $uid = 1;
    foreach( $result as $row){
      $uid = $row->uid;
      break;
    }
    // If the author uid and the logged in uid match, then we can delete the node
    if ( $user->uid == $uid || user_access('administer nodes')  ){
      node_delete($nid);
      $status = 'true';
      $message = 'Discussion was removed sucessfully';
      // This needs to be removed from the activity cache

    }
    else {
      $status = 'false';
      $message = 'Request Failed';
    }
    // return sucess message so that we can refresh the node
    // remove node
  }
  $data = array(
    'status' => $status,
    'message' => $message
  );
  drupal_json_output($data);
  drupal_exit();
}


/**
 * Ajax method to remove comment
 */
function _supportgroups_feed_ajax_remove_comment( $cid = 0 ){
  global $user;
  $status = 'false';
  $message = '';
  $not_parent = true;
  // Check to see if the cid is not equal to 0
  if( $cid > 0 || user_access('administer comments')   ){
    // Find the author uid of the discussion
    $result = db_query("SELECT uid FROM {comment}
      WHERE cid = :cid", array( ':cid' => $cid ) );
    foreach( $result as $row){
      $uid = $row->uid;
      break;
    }

    // Make sure that the comment is not a parent of another comment
    $result_pid = db_query("SELECT cid FROM {comment}
      WHERE pid = :cid", array( ':cid' => $cid) );
    foreach( $result_pid as $row_pid ){
      // if there exists a record, then we cannot delete this comment
      $not_parent = false;
      break;
    }

    // If the author uid and the logged in uid match, then we can delete the comment
    if( (($user->uid == $uid) && $not_parent) || (user_access('administer comments')  && $not_parent ) ){
      // Delete comment
      // Is this ok to do?
      comment_delete($cid);
      $status = 'true';
      $message = 'Comment was removed sucessfully';
    }
    else{
      $status = 'false';
      $message = 'Request Failed';
    }
    // return sucess message so that we can refresh the node
    // remove comment
  }
  $data = array(
    'status' => $status,
    'message' => $message
  );
  drupal_json_output($data);
  exit;
  //drupal_exit();
}




/**
 * Ajax method to add comment form
 */
function _supportgroups_feed_ajax_comment_form( $nid = 0, $pid = 0, $cid = 0 ){
  global $user;
  $status = 'false';
  $message = '';
  $pid_is_parent = true;
  $commentform = '';
  // Check to see if the nid is not equal to 0
  if( $nid > 0 && $user->uid > 0){
    // Check to see that comment is not a child of a comment
    $result = db_query("SELECT pid FROM {comment}
      WHERE nid = :nid AND cid = :pid", array( ':nid' => $nid, ':pid' => $pid ) );
    foreach( $result as $row){
      if( $row->pid != 0){
        $pid_is_parent = false;
      }
      break;
    }
    // If the pid is equal to 0 or pid is not a child, then we can create form
    if( $pid == 0 || $pid_is_parent ){
      $status = 'true';
      $message = 'Comment form was added';
      // add comment form
      if( module_exists('supportgroups_comment')){
        $commentform = _supportgroups_comment_get_comment_form($nid, $pid );
      }
    }
    else{
      $status = 'false';
      $message = 'Request Failed';
    }
    // return sucess message so that we can refresh the node
    // remove comment
  }
  $data = array(
    'status' => $status,
    'message' => $message,
    'commentform' => $commentform
  );
  drupal_json_output($data);
  exit;
  //drupal_exit();
}


/**
 * Ajax method to add comment form
 */
function _supportgroups_feed_ajax_edit_comment( $cid = 0, $nid = 0, $pid = 0 ){
  global $user;
  $status = 'false';
  $message = '';
  $commentform = '';
  // Check to see if the nid is not equal to 0
  if( $cid > 0 || user_access('administer comments')   ){
    // Find the author uid of the comment
    $result = db_query("SELECT uid FROM {comment}
      WHERE cid = :cid", array( ':cid' => $cid ) );
    foreach( $result as $row){
      $uid = $row->uid;
      break;
    }
    // If comment author and logged in user are the same, then we can edit the node
    if( $uid == $user->uid || user_access('administer comments') ){
      $status = 'true';
      $message = 'Comment edit form was added';
      // add comment form
      if( module_exists('supportgroups_comment')){
        $commentform = _supportgroups_comment_get_comment_form($nid, $pid , TRUE, $cid);
      }
    }
    else{
      $status = 'false';
      $message = 'Request Failed';
    }
    // return sucess message so that we can refresh the node
    // remove comment
  }
  $data = array(
    'status' => $status,
    'message' => $message,
    'commentform' => $commentform
  );
  drupal_json_output($data);
  exit;
  //drupal_exit();
}






/**
 * Ajax method to edit a discussion
 */
function _supportgroups_feed_ajax_edit_discussion( $nid = 0 ){
  global $user;
  $status = 'false';
  $message = '';
  // Check to see if the nid is not equal to 0
  if( $nid > 0  || user_access('administer nodes')    ){
    // Find the author uid of the discussion
    $result = db_query("SELECT uid FROM {node}
      WHERE nid = :nid", array( ':nid' => $nid ) );
    foreach( $result as $row){
      $uid = $row->uid;
      break;
    }
    // If the author uid and the logged in uid match, then we can delete the node
    if( $user->uid == $uid || user_access('administer nodes')   ){
      // add discussion edit form
      // May need to make supportgroups_node a dependency in the .info file
      if(module_exists('supportgroups_node')){
        // retrieve the edit discussion form
        $discussion = _supportgroups_node_group_post_form($user, $nid,TRUE);
      }
      $status = 'true';
      $message = 'Discussion form produced sucessfully';
    }
    else{
      $status = 'false';
      $message = 'Request Failed';
    }
    // return sucess message so that we can refresh the node
    // remove node
  }
  $data = array(
    'status' => $status,
    'message' => $message,
    'discussion' => $discussion
  );
  drupal_json_output($data);
  exit;
// drupal_exit();
}

