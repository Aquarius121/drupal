<?php
/**
 * @file
 * Cache functions for feeds including solr searches
 * These functions involve data that is cleared via Drupal cache_clear_all()
 */


/**
 * Helper function
 * maintain a static cache of activity items as returned from SOLR keyed off timestmap, e.g.
 * array(
 * $timestamp1 => $activity_item_object as returned by supportgroups_feed_get_activity_item
 * );
 *
 * This is expecting a stdClass return of the parsed ApacheSOLR results
 *
 * If no item is passed in, return the static array
 *
 * @see supportgroups_feed_apachsole_search_result_alter
 *
 * @param stdClass $search_result
 * @return Array
 */
function supportgroups_feed_apachesolr_current_activity_query_static_activity_cache($search_result = NULL) {
  $activities = &drupal_static(__FUNCTION__, array());

  if (is_null($search_result)) {
    return $activities;
  }

  $result = unserialize(base64_decode($search_result->zs_sf_activity_item));

  $timestamp = strtotime($search_result->ds_last_comment_or_change);
  $activities[$timestamp] = $result;
}

/**
 * Helper function
 * maintain a static cache of user IDs
 * If no UID is passed in, return the static array
 *
 * @see supportgroups_feed_apachsole_search_result_alter
 *
 * @param Int $uid
 * @return Array
 */
function supportgroups_feed_apachesolr_current_activity_query_static_user_cache($search_result = NULL) {
  $users = &drupal_static(__FUNCTION__, array());
  if (is_null($search_result)) {
    return $users;
  }
  //TODO when #1197 is resolved, also add any comment UIDs to this
  $users[$search_result->is_uid] = $search_result->is_uid;
}

/**
 * Helper function
 * maintain a static cache of node IDs
 * If no NIDis passed in, return the static array
 *
 * @see supportgroups_feed_apachsole_search_result_alter
 *
 * @param Int $nid
 * @return Array
 */
function supportgroups_feed_apachesolr_current_activity_query_static_nid_cache($nid = NULL) {
  $nids = &drupal_static(__FUNCTION__, array());
  if (is_null($nid)) {
    return $nids;
  }

  $nids[$nid] = $nid;
}



/**
 * Cache-enabled function
 * Retrieve all comments for the given node
 * NOTE: comments are trimmed/filtered per _supportgroups_feed_get_abbreviated_comment_info
 * This also does NOT take into account any comments that may already be displayed on the page (e.g. cached with the activity item itself
 * in sg_supportfeed_get_activity_item) so any display logic must correctly ignore comments that have already been displayed
 *
 *
 * @param Int $nid
 * @param bool $reset
 * @return Array ordered array of
 */
function supportgroups_feed_get_comments_for_nid($nid, $reset = FALSE) {
  if ($nid) {
    $cid = _supportgroups_feed_comments_for_nid_cid($nid);

    if ($reset) {
      $return_comments = array();
      $node = node_load($nid);
      $comments = array();
      if ($node->comment_count) {
        $comment_thread = comment_get_thread($node, COMMENT_MODE_THREADED, $node->comment_count);

        // break up massive number of comments because comment_load_multiple dies if there are more than ~1000 at a time
        $comment_thread_batch = array_chunk($comment_thread, 500);

        foreach ($comment_thread_batch as $thread_batch) {
          $tmp_comments = comment_load_multiple($thread_batch);
          if (is_array($tmp_comments)) {
            $comments = $comments + $tmp_comments;
          }
        }

        // strip unnecessary comment fields
        foreach ($comments as $comment_id => $comment) {
          $return_comments[$comment_id] = _supportgroups_feed_get_abbreviated_comment_info($comment);
        }
      }

      cache_set($cid, $return_comments, SUPPORTGROUPS_FEED_CACHE_TABLE);
      return $return_comments;
    }

    // attempt to retrieve from cache
    $cache = cache_get($cid, SUPPORTGROUPS_FEED_CACHE_TABLE);

    if ($cache) {
      return $cache->data;
    }
    elseif (!$reset) {
      return supportgroups_feed_get_comments_for_nid($nid, TRUE);
    }
  }
  return FALSE;
}

/**
 * Helper function to generate the cache ID (cid)
 */
function _supportgroups_feed_comments_for_nid_cid($nid) {
  return SUPPORTGROUPS_FEED_CACHE_TABLE .'comments-'. $nid;
}


/**
 * Cache-enabled function
 * @param bool $reset
 */
function supportgroups_feed_get_uids_for_group($group_id, $reset = FALSE) {
  if ($group_id) {
    $cid = _supportgroups_feed_uids_for_group_cid($group_id);

    if ($reset) {
      $uids = array();
      $query  = "SELECT etid as uid FROM {og_membership} WHERE entity_type = 'user' AND gid = :gid";
      $result = db_query($query, array(':gid' => $group_id));
      if ($result->rowCount()) {
        $uids = $result->fetchCol();
      }

      cache_set($cid, $uids, SUPPORTGROUPS_FEED_CACHE_TABLE);
      return $uids;
    }

    // attempt to retrieve from cache
    $cache = cache_get($cid, SUPPORTGROUPS_FEED_CACHE_TABLE);

    if ($cache) {
      return $cache->data;
    }
    elseif (!$reset) {
      return supportgroups_feed_get_uids_for_group($group_id, TRUE);
    }
  }
  return FALSE;
}

/**
 * Helper function to generate the cache ID (cid)
 */
function _supportgroups_feed_uids_for_group_cid($group_id) {
  return $group_id;
}



/**
 * Cache-enabled function
 * Return a cached discussion object with comments, etc as needed for support feed/group listing page
 *
 * @param bool $reset
 */
function supportgroups_feed_get_activity_item($nid, $reset = FALSE) {
  static $activity_items = array();
  if (isset($activity_items[$nid])) {
    return $activity_items[$nid];
  }

  if ($nid) {
    $cid = _supportgroups_feed_activity_item_cid($nid);

    if ($reset) {
      $new_item = _supportgroups_feed_get_activity_item_from_node($nid);

      //we'll be caching an object
      //the object must include nid and type in order to be compatible with other logic
      cache_set($cid, $new_item, SUPPORTGROUPS_FEED_CACHE_TABLE);
      $activity_items[$nid] = $new_item;

      return $new_item;
    }

    // attempt to retrieve from cache
    $cache = cache_get($cid, SUPPORTGROUPS_FEED_CACHE_TABLE);

    if ($cache) {
      $activity_items[$nid] = $cache->data;
      return $cache->data;
    }
    elseif (!$reset) {
      return supportgroups_feed_get_activity_item($nid, TRUE);
    }
  }
  return FALSE;
}


/**
 * Cache-enabled function
 * Return associative array of group_id/nid => title
 *
 * @param bool $reset
 */
function supportgroups_feed_get_group_names($reset = FALSE) {
  $cid = _supportgroups_feed_group_names_cid();

  if ($reset) {
    $group_names = array();
    $query= "SELECT title, nid as group_id FROM {node} WHERE type = 'set_up_group'";
    $result = db_query($query, array());

    foreach ($result as $group) {
      $group_names[$group->group_id] = $group->title;
    }

    cache_set($cid, $group_names, SUPPORTGROUPS_FEED_CACHE_TABLE);
    return $group_names;
  }

  // attempt to retrieve from cache
  $cache = cache_get($cid, SUPPORTGROUPS_FEED_CACHE_TABLE);

  if ($cache) {
    return $cache->data;
  }
  elseif (!$reset) {
    return supportgroups_feed_get_group_names(TRUE);
  }

  return array();
}


/**
 * Helper function to clear the cache for a given activity item nid
 * @param Int $nid
 */
function _supportgroups_feed_activity_item_clear_cache($nid) {
  static $cleared_list = array();
  if (isset($cleared_list[$nid])) {
    return;
  }
  cache_clear_all(_supportgroups_feed_activity_item_cid($nid), SUPPORTGROUPS_FEED_CACHE_TABLE);
  cache_clear_all(_supportgroups_feed_comments_for_nid_cid($nid), SUPPORTGROUPS_FEED_CACHE_TABLE);

  // set a static flag so we don't do this multiple times in a single page load
  $cleared_list[$nid] = TRUE;
}

/**
 * Helper function to facilitate clearing redis cache
 * Clear the cache itself (by truncating) and unset flag so we know to rebuild this cache item
 * //TODO refactor to use $client->exists($key_name) instead of Drupal flag to avoid unnecessary cache hits
 * @param unknown_type $key_name
 */
function _supportgroups_feed_clear_redis_cache_for_key_name($key_name) {
  // see function comments for explanation
  _supportgroups_feed_trim_sorted_set($key_name, 0);

  cache_clear_all($key_name, SUPPORTGROUPS_FEED_CACHE_TABLE);
}


/**
 * Helper function to manage cleaning up caches when user relationships are added/removed
 * This also rebuilds the user subscription cache when completed
 *
 * @param stdClass $relationship
 */
function _supportgroups_feed_clear_user_cache_for_relationship($relationship) {
  // clear individual user info caches
  cache_clear_all(_supportgroups_feed_user_info_cid($relationship->requester_id), SUPPORTGROUPS_FEED_CACHE_TABLE);
  cache_clear_all(_supportgroups_feed_user_info_cid($relationship->requestee_id), SUPPORTGROUPS_FEED_CACHE_TABLE);
}

/**
 * General case helper function to handle group inserts/deletes of members and content
 * invalidates caches as appropriate
 * if specific more optimized behavior is desired to handle inserts/deletes, break this out into truly separate hook_insert and hook_delete
 *
 * @see supportgroups_feed_og_membership_insert
 * @param stdClass $og_membership
 * @param String $action insert/delete
 */
function _supportgroups_feed_og_membership_helper($og_membership, $action = 'insert') {
  $is_insert = $action == 'insert';
  $affected_nids = array();
  $affected_uids = array();

  // if a node is being posted to/removed from the group
  if ($og_membership->entity_type == 'node') {
    // no action needed at this time
  }
  elseif ($og_membership->entity_type == 'user') {
    // clear cache of user list for group
    cache_clear_all(_supportgroups_feed_uids_for_group_cid($og_membership->gid), SUPPORTGROUPS_FEED_CACHE_TABLE);

    // clear user info cache so their subscriptions will properly be reflected
    cache_clear_all(_supportgroups_feed_user_info_cid($og_membership->etid), SUPPORTGROUPS_FEED_CACHE_TABLE);
  }
}


/**
 * Cache-enabled function
 * Retreive relevant user information for theming feeds/comments
 *
 * @param bool $reset
 */
function supportgroups_feed_get_user_info($uid, $reset = FALSE) {
  static $user_data = array();
  if (isset($user_data[$uid])) {
    return $user_data[$uid];
  }
  if ($uid) {
    $cid = _supportgroups_feed_user_info_cid($uid);

    if ($reset) {
      $current_user = user_load($uid);

      $user_info = new stdClass();

      $user_info->uid = $uid;

      $user_info->name = $current_user->name;

      $user_info->link = url("user/". $uid);

      // add supported-by
      $user_info->supported_by = _supportgroups_feed_get_supported_by($uid);

      // add supports
      $user_info->supports = _supportgroups_feed_get_supported_users($uid);

      // add authored
      $user_info->authored = _supportgroups_feed_get_nids_authored_by_uids(array($uid));

      // add commented on
      $user_info->commented_on = _supportgroups_feed_get_commented_on_nids_for_uid($uid);

      // add groups
      $user_info->groups = supportgroups_feed_get_groups_for_uid($uid, TRUE);

      // image
      if (is_object($current_user->picture)) {
        $user_info->picture = $current_user->picture->uri;
      }

      // bio info
      $user_info->profile_bio = isset($current_user->profile_bio) ? $current_user->profile_bio : '';

      //user points
      $user_info->points = userpoints_get_current_points($uid);

      cache_set($cid, $user_info, SUPPORTGROUPS_FEED_CACHE_TABLE);
      $user_data[$uid] = $user_info;
      return $user_info;
    }

    // attempt to retrieve from cache
    $cache = cache_get($cid, SUPPORTGROUPS_FEED_CACHE_TABLE);

    if ($cache) {
      $user_data[$uid] = $cache->data;
      return $cache->data;
    }
    elseif (!$reset) {
      return supportgroups_feed_get_user_info($uid, TRUE);
    }
  }
  return FALSE;
}

