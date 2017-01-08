<?php

/**
 * @file
 * Redis specific caching.
 * These items are not cleared via Drupal cache_clear_all() and therefore have to have their own cache management
 */

/**
 * Helper function to save batched sets of values to Redis
 * This works for sorted sets
 *
 * @param String $cid
 * @param Array $values must be in the form $a[0] = score1, $a[1] = item1, $a[2] = score2, $a[3] = item2
 */
function _supportgroups_feed_batch_save_redis_sorted_set($cid, $values, $override_batch_size = 0) {
  $client = _supportgroups_feed_get_cache_client();

  $batch_size = $override_batch_size ? $override_batch_size : SUPPORTGROUPS_FEED_REDIS_CACHE_BATCH_SIZE;
  // to cut down on round-trips to Redis, batch up the writes
  foreach (array_chunk($values, $batch_size) as $vals) {
    // add $cid/key name to the args
    array_unshift($vals, $cid);
    call_user_func_array(array($client, 'zAdd'), $vals);
  }
}


/**
 * Helper function to prepend and cache a new node ID/discussion to the activity list
 *
 * @param Int $nid
 */
function supportgroups_feed_add_recent_activity($nid) {
  $item = supportgroups_feed_get_activity_item($nid);

  if ($item) {
    //NOTE: to keep this function as lightweight as possible, we are NOT
    // removing duplicate node IDs.
    // since the theming side will need to iterate through this array anyway to theme it
    // theme side should keep track of what node IDs have been displayed and suppress excess ones from being shown

    // list will get truncated via cron jobs to avoid unnecessary burden on the server
    // @see supportgroups_feed_trim_recent_activity_cache as defined in supportgroups_feed_cronapi
    $client = _supportgroups_feed_get_cache_client();

    // add item with a "score" of time() to the ordered set. Items are stored in ascending time order
    $client->zAdd(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, time(), serialize($item));
  }
}

/**
 * Helper function to prepend and cache a new node ID/discussion to the activity list
 *
 * @param Array $item
 */
function supportgroups_feed_add_recent_activity_notification($item) {
  static $items = array();

  if ($item) {
    // only allow one add per item per page load
    if (isset($items[$item->nid])) {
      return ;
    }

    $items[$item->nid] = $item->nid;

    // list will get truncated via cron jobs to avoid unnecessary burden on the server
    // @see supportgroups_feed_trim_recent_activity_notifications_cache as defined in supportgroups_feed_cronapi
    $client = _supportgroups_feed_get_cache_client();
    // orig_comment_uid and node_uid
    // add item with a "score" of time() to the ordered set. Items are stored in ascending time order
    if (variable_get('supportgroups_individual_notification_cache', 0)) {
      if ($item->orig_comment_uid > 0) {
        $client->zAdd(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $item->orig_comment_uid, $item->timestamp,
          serialize($item));
      }
      $client->zAdd(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $item->node_uid, $item->timestamp,
        serialize($item));

    }
    else {
      $client->zAdd(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, $item->timestamp,
        serialize($item));
    }
  }
}

/**
 * Helper function to prepend and cache a new node ID/discussion to the activity list
 *
 * @param Array $item
 */
function supportgroups_feed_add_recent_activity_notification_by_uid($item, $uid) {
  static $items = array();

  if ($item) {
    // only allow one add per item per page load
    if (isset($items[$item->nid])) {
      return ;
    }

    $items[$item->nid] = $item->nid;

    // list will get truncated via cron jobs to avoid unnecessary burden on the server
    // @see supportgroups_feed_trim_recent_activity_notifications_cache as defined in supportgroups_feed_cronapi
    $client = _supportgroups_feed_get_cache_client();

    // add item with a "score" of time() to the ordered set. Items are stored in ascending time order
    $client->zAdd(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $uid, $item->timestamp,
      serialize($item));
  }
}


/**
 * Helper function that handles actually retrieving current activity and storing it to Redis keyed array (under key=>SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID)
 * @see supportgroups_feed_get_recent_activity
 * NOTE: we do not extract user information at this point, since we don't know how many items we will be displaying
 *
 */
function _supportgroups_feed_get_recent_activity() {
  $query = 'SELECT tn.nid, tn.changed AS timestamp
            FROM tracker_node tn
            INNER JOIN node n ON tn.nid = n.nid AND n.type IN (:types)
            WHERE published
            AND tn.changed > :timestamp
            ORDER BY timestamp DESC';

  $params = array(':timestamp' => strtotime('-1 day'), ':types' => join(',', _supportgroups_feed_activity_feed_node_types()));
  $result = db_query($query, $params);
  $nids = array();

  // retrieve the nids without having to iterate through the array first
  // in order to not have to perform this query twice, we must store the results and iterate through them to pull
  // out the nids
  $results = $result->fetchAllAssoc('timestamp');

  // put together list of nids to retrieve in as few cache calls as possible
  foreach ($results as $record) {
    // key off node ID to keep these unique
    $nids[$record->nid] = $record->nid;
  }

  $activity_items = supportgroups_feed_get_activity_items(array_values($nids));

  // Create batch for more efficient saving to Redis
  $redis_batch = array();
  foreach ($results as $timestamp => $record) {
    if (isset($nids[$record->nid])) {
      $redis_batch[] = $timestamp;
      $redis_batch[] = serialize($activity_items[$record->nid]);

      // ensure that we don't add duplicates when rebuilding activity queue
      unset($nids[$record->nid]);
    }
  }

  _supportgroups_feed_batch_save_redis_sorted_set(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, $redis_batch, SUPPORTGROUPS_FEED_RECENT_ACTIVITY_REDIS_BATCH_SIZE);
}

/**
 * Helper function that handles recent group activity and storing it to Redis keyed array (under key=>_supportgroups_feed_recent_activity_for_gid_cid($gid))
 * @see supportgroups_feed_get_past_activity_for_group
 * NOTE: we do not extract user information at this point, since we don't know how many items we will be displaying
 *
 */
function _supportgroups_feed_get_recent_activity_for_gid($gid) {
  $data = _supportgroups_feed_get_recent_activity_helper(array($gid), array(), 0, SUPPORTGROUPS_FEED_RECENT_ACTIVITY_GROUP_MAX_ITEMS);

  $client = _supportgroups_feed_get_cache_client();

  $cid = _supportgroups_feed_recent_activity_for_gid_cid($gid);

  // Create batch for more efficient saving to Redis
  $redis_batch = array();
  foreach ($data['activity'] as $timestamp => $record) {
    $redis_batch[] = $timestamp;
    $redis_batch[] = serialize($record);
  }
  _supportgroups_feed_batch_save_redis_sorted_set($cid, $redis_batch, SUPPORTGROUPS_FEED_RECENT_ACTIVITY_REDIS_BATCH_SIZE);
}

/**
 * Helper function to prepend and cache a new node ID/discussion to the activity list for the given group
 *
 * @param Int $nid
 */
function supportgroups_feed_add_recent_activity_for_gid($nid, $gid) {
  $item = supportgroups_feed_get_activity_item($nid);

  if ($item) {
    //NOTE: to keep this function as lightweight as possible, we are NOT
    // removing duplicate node IDs.
    // since the theming side will need to iterate through this array anyway to theme it
    // theme side should keep track of what node IDs have been displayed and suppress excess ones from being shown

    // list will get truncated via cron jobs to avoid unnecessary burden on the server
    // @see supportgroups_feed_trim_recent_activity_cache as defined in supportgroups_feed_cronapi
    $client = _supportgroups_feed_get_cache_client();

    // add item with a "score" of time() to the ordered set. Items are stored in ascending time order
    $client->zAdd(_supportgroups_feed_recent_activity_for_gid_cid($gid), time(), serialize($item));
  }
}

/*********************
 * Remove from redis
 */

/**
 * Helper function to add a simple value to a sorted set
 *
 * @param String $cid
 * @param String $item
 * @param Int $score optional score-- will default to the value of the item if not specified
 */
function _supportgroups_feed_add_item_to_sorted_set($cid, $item, $score = NULL) {
  $client = _supportgroups_feed_get_cache_client();
  if (is_null($score)) {
    $score = $item;
  }
  $client->zAdd($cid, $score, $item);
}

/**
 * Helper function to remove a simple value from a sorted set
 *
 * @param String $cid
 * @param String $item
 * @param Int $score optional score-- will default to the value of the item if not specified
 */
function _supportgroups_feed_remove_item_from_sorted_set($cid, $item) {
  $client = _supportgroups_feed_get_cache_client();
  $client->zRem($cid, $item);
}

/**
 * Helper function to remove an item from a sorted set by score
 *
 * @param String $cid cacheID/Key name
 * @param Int $score score for sorted set item
 * @return Int number of items removed
 */
function _supportgroups_feed_remove_item_from_sorted_set_by_score($cid, $score) {
  $client = _supportgroups_feed_get_cache_client();
  return $client->zRemRangeByScore($cid, $score, $score);
}

/**
 * Helper function to remove an item from sorted set by NID.
 * The sorted set must contain a serialized object with the nid defined in it.
 *
 * Note: this is a processor-intensive function so this should be used judiciously
 * because we have to unserialize each item in the cache
 *
 * @param String $cid Cache/Key ID
 * @param Int  $nid Node ID Optional node ID. If not specified will simply clean up sorted set to remove duplicate nids
 * @param Int $from_timestamp lower bound of the timestamps to look at
 * @param Bool $leave_most_recent send TRUE to leave the most recent copy in the cache, otherwise we will delete all occurrences
 */
function _supportgroups_feed_remove_items_from_sorted_set_by_nid($cid, $nids = array(), $from_timestamp = 0, $leave_most_recent = FALSE) {
  static $processed_nids = array();

  if ($nids && !is_array($nids)) {
    // if the node has already been removed on this execution, then don't take any further action
    if (isset($processed_nids[$cid][$nids]) && !$leave_most_recent) {
      return;
    }

    $nids = array($nids);
  }

  // if node ID is not specified then just clean out older records from the cache leaving only most recent activity
  if (!$nids) {
    $leave_most_recent = TRUE;
  }

  // mark the node as processed
  if (!isset($processed_nids[$cid])) {
    $processed_nids[$cid] = array();
    foreach ($nids as $nid) {
      // set flag so we only process a given nid once in a given page
      $processed_nids[$cid][$nid] = $nid;
    }
  }

  $nids_seen = array();

  // get timestamped items in descending order
  $client = _supportgroups_feed_get_cache_client();
  $results = $client->zRevRangeByScore($cid, '+inf', $from_timestamp, array('withscores' => TRUE));

  foreach ($results as $activity_item => $timestamp) {
    $activity_item = unserialize($activity_item);
    $remove_item = FALSE;

    if ($leave_most_recent) {
      if (!isset($nids_seen[$activity_item->nid])) {
        // if we haven't seen this node already, then leave it and flag it so any subsequent occurrences are deleted
        $nids_seen[$activity_item->nid] = $activity_item->nid;
        continue;
      }
    }

    // if nid is not specified and we've made it through to this point then we delete the node
    if (!$nids) {
      $remove_item = TRUE;
    }
    else {
      // if nid is specified and the current item matches the given nid
      if (in_array($activity_item->nid, $nids)) {
        $remove_item = TRUE;
      }
    }

    // fall through to here-- either we've already seen this nid ($leave_most_recent == TRUE) or we're deleting all occurrences
    $nids_seen[$activity_item->nid] = $activity_item->nid;

    // if flag is set go ahead and delete item from the set
    if ($remove_item) {
      watchdog('supportgroups_feed', 'Deleting nid from CID: '. $cid .', nid '. $activity_item->nid, array(),WATCHDOG_ALERT);
      // How does this delete the item?  It probably doesn't
      _supportgroups_feed_remove_item_from_sorted_set_by_score($cid, $timestamp);
    }
  }// end foreach ($results as $timestamp => $activity_item) {
}// end function _supportgroups_feed_remove_items_from_sorted_set_by_nid($cid, $nids = array(), $from_timestamp = 0, $leave_most_recent = FALSE) {

/**
 * Helper function to delete the given key name entirely from Redis
 * @param String $key_name
 */
function _supportgroups_feed_delete_redis_cache_for_key_name($key_name) {
  $client = _supportgroups_feed_get_cache_client();
  return $client->delete($key_name);
}

/**
 * Helper function to to rebuild the recent activity cache-- e.g. returning from a cache clear
 */
function supportgroups_feed_warm_recent_activity_cache() {
  $client = _supportgroups_feed_get_cache_client();

  // get full count of items in this Redis key-- +inf just means to positive infinity
  $count = $client->zcount(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, 0, '+inf');
  $count = $count ? $count : 0;

  // check for flag in DB. If flag is not set then rebuild recent activity cache
  // note because we are using drupal's cache_get for this we can use the same CID that we use directly in Redis
  // because drupal's cache_get automatically adds additional prefix info
  if (($count < (SUPPORTGROUPS_FEED_RECENT_ACTIVITY_MAX_ITEMS / 2)) || !cache_get(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, SUPPORTGROUPS_FEED_CACHE_TABLE)) {
    // use semaphore lock to prevent multiple processes from trying to rebuild this at once
    if (lock_acquire(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, SUPPORTGROUPS_FEED_LOCK_HOLD_TIME)) {
      _supportgroups_feed_get_recent_activity();

      // set timestamp flag so we know cache has been warmed
      cache_set(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, TRUE, SUPPORTGROUPS_FEED_CACHE_TABLE, strtotime(SUPPORTGROUPS_FEED_MAX_CACHE_AGE));

      // release semaphore
      lock_release(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID);
    }
    else {
      if (lock_wait(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, SUPPORTGROUPS_FEED_MAX_LOCK_WAIT_TIME)) {
        watchdog('supportgroups_feed', 'Lock for '. SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID .' did not release within '. SUPPORTGROUPS_FEED_MAX_LOCK_WAIT_TIME. '. seconds', array(), WATCHDOG_WARNING);
      };
    }
  }
}

/**
 * Helper function to help cut down the number of individual cached activity items in the system over time
 */
function supportgroups_feed_trim_activity_item_cache() {
  $client = _supportgroups_feed_get_cache_client();

  $keys = $client->keys('*'. SUPPORTGROUPS_FEED_CACHE_PREFIX .'act-item-*');

  $keyed_array = array();

  // build up a keyed array of $nid => $redis cache key ID
  foreach ($keys as $index => $item) {
    $nid = end(explode('-', $item));
    $keyed_array[$nid] = $item;
  }

  // get the current set of recent activity
  $recent_activity = supportgroups_feed_get_recent_activity_since_date(0, TRUE);

  // anything that is in the current recent activity, we can likely trim from the individual item cache
  foreach ($recent_activity as $timestamp => $item) {
    unset($keyed_array[$item->nid]);
  }

  // anything left in the $keyed_array doesn't currently exist in the recent activity cache, so might as well clear it out
  foreach ($keyed_array as $cid) {
    $client->del($cid);
  }

}

/**
 * Helper function to de-duplicate the recent activity notifications cache
 *
 */
function supportgroups_feed_trim_duplicates_from_recent_activity_notifications_cache() {
  $results = supportgroups_feed_get_recent_activity_notifications_since_date(0, TRUE);

  $client = _supportgroups_feed_get_cache_client();
  // keep track of items that already appear in the system
  $items = array();
  foreach ($results as $timestamp => $item) {
    unset($item->timestamp);
    $working_timestamp = floor($timestamp / 1000);
    $serialized_item = base64_encode(serialize($item));

    if (isset($items[$working_timestamp])) {

      // if there is something at this timestamp, let's see if it matches what is in the DB already
      if ($items[$working_timestamp] == $serialized_item) {
        // remove this item from the cache
        $client->zRemRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, $timestamp, $timestamp);
        continue;
      }
    }
    else {
      $items[$working_timestamp] = $serialized_item;
    }
  }
}

/**
 * Helper function to de-duplicate the recent activity notifications cache by uid
 *
 */
function supportgroups_feed_trim_duplicates_from_recent_activity_notifications_cache_by_uid($uid) {
  $results = supportgroups_feed_get_recent_activity_notifications_since_date($uid, 0, TRUE);

  $client = _supportgroups_feed_get_cache_client();
  // keep track of items that already appear in the system
  $items = array();
  foreach ($results as $timestamp => $item) {
    unset($item->timestamp);
    $working_timestamp = floor($timestamp / 1000);
    $serialized_item = base64_encode(serialize($item));

    if (isset($items[$working_timestamp])) {

      // if there is something at this timestamp, let's see if it matches what is in the DB already
      if ($items[$working_timestamp] == $serialized_item) {
        // remove this item from the cache
        $client->zRemRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $uid, $timestamp,
          $timestamp);
        continue;
      }
    }
    else {
      $items[$working_timestamp] = $serialized_item;
    }
  }
}

/**
 * Helper function to to rebuild the recent activity cache-- e.g. returning from a cache clear
 */
function supportgroups_feed_warm_recent_activity_notifications_cache() {
  $client = _supportgroups_feed_get_cache_client();

  // get full count of items in this Redis key-- +inf just means to positive infinity
  $count = $client->zcount(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, 0, '+inf');
  $count = $count ? $count : 0;

  // check for flag in DB. If flag is not set then rebuild recent activity cache
  // note because we are using drupal's cache_get for this we can use the same CID that we use directly in Redis
  // because drupal's cache_get automatically adds additional prefix info
  if (($count < (SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_MAX_ITEMS / 4)) || !cache_get(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, SUPPORTGROUPS_FEED_CACHE_TABLE)) {
    // use semaphore lock to prevent multiple processes from trying to rebuild this at once
    if (lock_acquire(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, SUPPORTGROUPS_FEED_LOCK_HOLD_TIME)) {

      if (variable_get('supportgroups_individual_notification_cache', 0)) {
        // Go through all cached user notifications
        $uids = _supportgroups_get_notification_user_keys();
        foreach ($uids as $uid) {
          _supportgroups_feed_get_recent_activity_notifications_by_uid($uid);
        }
      }
      else {
        //TODO if need be, add a pre-filtering date here so we don't keep adding too many duplicate items to the cache
        _supportgroups_feed_get_recent_activity_notifications();
      }

      // set timestamp flag so we know cache has been warmed
      cache_set(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, TRUE, SUPPORTGROUPS_FEED_CACHE_TABLE, strtotime(SUPPORTGROUPS_FEED_MAX_CACHE_AGE));

      // release semaphore
      lock_release(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID);
    }
    else {
      if (lock_wait(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, SUPPORTGROUPS_FEED_MAX_LOCK_WAIT_TIME)) {
        watchdog('supportgroups_feed', 'Lock for '. SUPPORTGROSUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CIDUPS_FEED_RECENT_ACTIVITY_CID .' did not release within '. SUPPORTGROUPS_FEED_MAX_LOCK_WAIT_TIME. '. seconds', array(), WATCHDOG_WARNING);
      };
    }
  }
}

/**
 * Helper function to trim the Redis Sorted Set specified by $key_name to a max of $max_items
 *
 * @param String $key_name
 * @param Int $max_items
 */
function _supportgroups_feed_trim_sorted_set($key_name, $max_items) {
  $client = _supportgroups_feed_get_cache_client();
  // get full count of items in this Redis key-- +inf just means to positive infinity
  $count = $client->zcount($key_name, 0, '+inf');

  if ($count > $max_items) {

    $start = 0;
    // get array position of the last item we want to delete
    $end =  $count - $max_items;

    // truncate the sorted set to keep it to the correct number of items
    //this key is stored in ascending time order, so we want to remove the first X items
    $client->zRemRangeByRank($key_name, 0, $end);
  }
}

/**
 * Helper function to trim the Redis Sorted Set specified by $key_name to a max of $max_items
 *
 * @param String $key_name
 * @param Int $last_date_to_keep
 */
function _supportgroups_feed_trim_sorted_set_by_score($key_name, $last_score_to_keep) {
  $client = _supportgroups_feed_get_cache_client();
  $client->zRemRangeByScore($key_name, 0, $last_score_to_keep);
}

/**
 * Helper function to to rebuild the recent activity cache for x random groups-- e.g. returning from a cache clear
 */
function supportgroups_feed_warm_recent_activity_group_cache($gid = 0) {
  $client = _supportgroups_feed_get_cache_client();

  if ($gid) {
    // spoof this as a database return so we can use the same logic regardless of if gid is specified or not
    $row = new stdClass();
    $row->nid = $gid;
    $result = array($row);
  }
  else {
    $query = "SELECT nid FROM {node} WHERE type = :type ORDER BY RAND() LIMIT ". SUPPORTGROUPS_FEED_RECENT_ACTIVITY_GROUP_MAX_ITEMS_PER_CRON;
    $result = db_query($query, array(':type' => 'set_up_group'));
  }

  foreach ($result as $row) {
    // check to see if cache has been warmed and how recently
    $cid = _supportgroups_feed_recent_activity_for_gid_cid($row->nid);
    // get full count of items in this Redis key-- +inf just means to positive infinity
    $count = $client->zcount($cid, 0, '+inf');
    $count = $count ? $count : 0;

    if (!$client->exists($cid) || $count < (SUPPORTGROUPS_FEED_RECENT_ACTIVITY_GROUP_MAX_ITEMS / 2)) {
      if (lock_acquire($cid, SUPPORTGROUPS_FEED_LOCK_HOLD_TIME)) {
        _supportgroups_feed_get_recent_activity_for_gid($row->nid);

        lock_release($cid);
      }
      else {
        if (lock_wait($cid, SUPPORTGROUPS_FEED_MAX_LOCK_WAIT_TIME)) {
          watchdog('supportgroups_feed', 'Lock for '. $cid .' did not release within '. SUPPORTGROUPS_FEED_MAX_LOCK_WAIT_TIME. '. seconds', array(), WATCHDOG_WARNING);
        };
      }
    }
  }

}

/**
 * Trim the group recent activity caches for X random groups
 * This way we don't need to keep the list trimmed in supportgroups_feed_add_recent_activity_for_gid
 */
function supportgroups_feed_trim_recent_activity_group_cache() {

  $query = "SELECT nid FROM {node} WHERE type = :type ORDER BY RAND() LIMIT ". SUPPORTGROUPS_FEED_RECENT_ACTIVITY_GROUP_MAX_ITEMS_PER_CRON;
  $result = db_query($query, array(':type' => 'set_up_group' ));

  foreach ($result as $row) {
    $cid = _supportgroups_feed_recent_activity_for_gid_cid($row->nid);
    // clean up duplicate notifications in the cache -- e.g. multiple comments on a single node
    _supportgroups_feed_remove_items_from_sorted_set_by_nid($cid, array(), 0, TRUE);
    _supportgroups_feed_trim_sorted_set_by_score($cid, strtotime('-1 day'));

    //TODO if the cache gets unwieldy we can comment this back in to also limit max number of items
    //_supportgroups_feed_trim_sorted_set(_supportgroups_feed_recent_activity_for_gid_cid($row->nid), SUPPORTGROUPS_FEED_RECENT_ACTIVITY_GROUP_MAX_ITEMS);
  }
}

/**********************
 * Get from redis only
 */

/**
 * Return all recent activity for Group ID since date
 *
 * @param Int $gid Group ID
 * @param Int $from_timestamp
 * @param bool $unserialize_return
 * @return Array
 */
function supportgroups_feed_get_recent_activity_for_gid_since_date($gid, $from_timestamp = 0, $unserialize_return = FALSE) {
  $results = array();

  if ($gid) {
    $cid = _supportgroups_feed_recent_activity_for_gid_cid($gid);

    $client = _supportgroups_feed_get_cache_client();

    // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
    $results = $client->zRevRangeByScore($cid, '+inf', $from_timestamp, array('withscores' => TRUE));

    if (!is_array($results) || (count($results) < SUPPORTGROUPS_FEEDS_MIN_RECENT_ACTIVITY)) {
      // make sure cache is warmed first
      supportgroups_feed_warm_recent_activity_group_cache($gid);
      $results = $client->zRevRangeByScore($cid, '+inf', $from_timestamp, array('withscores' => TRUE));
    }

    if (!is_array($results)) {
      $results = array();
    }
    else {
      $temp_results = $results;
      $results = array();

      // results from Redis are returned in the form data => timestamp, and timestamp is a float
      //, so we convert this to int when we process data before returning

      foreach ($temp_results as $activity_item => $timestamp) {
        $activity_item = $unserialize_return ? unserialize($activity_item) : $activity_item;
        $results[intval($timestamp)] = $activity_item;
      }
    }
  }
  return $results;
}


/**
 * Return all recent activity since the given date
 * @param Int $from_timestamp
 * @param int $from_timestamp unix_timestamp of start date
 * @param bool $unserialize_return Redis stores serialized results. set this flag to TRUE to have this function unserialize in-place.
 */
function supportgroups_feed_get_recent_activity_since_date($from_timestamp = 0, $unserialize_return = FALSE) {

  $client = _supportgroups_feed_get_cache_client();

  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  $results = $client->zRevRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, '+inf', $from_timestamp, array('withscores' => TRUE));

  if (!is_array($results) || count($results) < SUPPORTGROUPS_FEEDS_MIN_RECENT_ACTIVITY) {
    supportgroups_feed_warm_recent_activity_cache();
    $results = $client->zRevRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_CID, '+inf', $from_timestamp, array('withscores' => TRUE));
  }



  if (!is_array($results)) {
    $results = array();
  }
  else {
    $temp_results = $results;
    $results = array();

    // results from Redis are returned in the form data => timestamp, and timestamp is a float
    //, so we convert this to int when we process data before returning

    foreach ($temp_results as $activity_item => $timestamp) {
      $activity_item = $unserialize_return ? unserialize($activity_item) : $activity_item;
      $results[intval($timestamp)] = $activity_item;
    }
  }

  return $results;
}

/**
 * Retrieve master list of activity notifications since the given timestamp
 * NOTE: we are expecting a PHP timestamp (seconds), but notifications are stored in milliseconds so we convert in this function
 * @param int $from_timestamp unix_timestamp of start date
 * @param bool $unserialize_return Redis stores serialized results. set this flag to TRUE to have this function unserialize in-place.
 */
function supportgroups_feed_get_recent_activity_notifications_since_date($from_timestamp = 0, $unserialize_return = FALSE) {
  $client = _supportgroups_feed_get_cache_client();
  $from_timestamp = 0;
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  $results = $client->zRevRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, '+inf', 1000 * $from_timestamp, array('withscores' => TRUE));

  if (!is_array($results)) {
    $results = array();
  }
  else {
    $temp_results = $results;
    $results = array();

    // results from Redis are returned in the form data => timestamp, and timestamp is a float
    //, so we convert this to int when we process data before returning

    foreach ($temp_results as $activity_item => $timestamp) {
      $activity_item = $unserialize_return ? unserialize($activity_item) : $activity_item;
      $results[intval($timestamp)] = $activity_item;
    }
  }

  return $results;
}

/**
 * Retrieve master list of activity notifications since the given timestamp
 * NOTE: we are expecting a PHP timestamp (seconds), but notifications are stored in milliseconds so we convert in this function
 * @param int $from_timestamp unix_timestamp of start date
 * @param bool $unserialize_return Redis stores serialized results. set this flag to TRUE to have this function unserialize in-place.
 */
function supportgroups_feed_get_recent_activity_notifications_since_date_by_uid($uid, $from_timestamp = 0,
                                                                          $unserialize_return = FALSE) {
  $client = _supportgroups_feed_get_cache_client();
  $from_timestamp = 0;
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  $results = $client->zRevRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $uid, '+inf',
    1000 * $from_timestamp, array('withscores' => TRUE));

  if (!is_array($results)) {
    $results = array();
  }
  else {
    $temp_results = $results;
    $results = array();

    // results from Redis are returned in the form data => timestamp, and timestamp is a float
    //, so we convert this to int when we process data before returning

    foreach ($temp_results as $activity_item => $timestamp) {
      $activity_item = $unserialize_return ? unserialize($activity_item) : $activity_item;
      $results[intval($timestamp)] = $activity_item;
    }
  }

  return $results;
}

/**
 * Retrieve master list of activity notifications after the given timestamp
 * NOTE: we are expecting a PHP timestamp (seconds), but notifications are stored in milliseconds so we convert in this function
 * This does not query based on user but on timestamp
 * @param int $from_timestamp unix_timestamp of start date
 * @param bool $unserialize_return Redis stores serialized results. set this flag to TRUE to have this function unserialize in-place.
 */
function supportgroups_feed_get_recent_activity_notifications_after_date($from_timestamp = 0,
                                                                         $unserialize_return = FALSE) {
  $client = _supportgroups_feed_get_cache_client();
  // we need to set some kind of limit per batch
  $limit = SUPPORTGROUPS_FEED_REDIS_CACHE_BATCH_SIZE;
//  $from_timestamp = time();
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  $results = $client->zRevRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID, 1000 * $from_timestamp,
    '-inf',
    array('withscores' => TRUE, 'limit' => array(0, $limit)));

  if (!is_array($results)) {
    $results = array();
  }
  else {
    $temp_results = $results;
    $results = array();

    // results from Redis are returned in the form data => timestamp, and timestamp is a float
    //, so we convert this to int when we process data before returning

    foreach ($temp_results as $activity_item => $timestamp) {
      $activity_item = $unserialize_return ? unserialize($activity_item) : $activity_item;
      $results[intval($timestamp)] = $activity_item;
    }
  }

  return $results;
}

/**
 * Retrieve master list of activity notifications after the given timestamp
 * NOTE: we are expecting a PHP timestamp (seconds), but notifications are stored in milliseconds so we convert in this function
 * This does not query based on user but on timestamp
 * @param int $from_timestamp unix_timestamp of start date
 * @param bool $unserialize_return Redis stores serialized results. set this flag to TRUE to have this function unserialize in-place.
 */
function supportgroups_feed_get_recent_activity_notifications_after_date_by_uid($uid, $from_timestamp = 0,
                                                                         $unserialize_return = FALSE) {
  $client = _supportgroups_feed_get_cache_client();
  // we need to set some kind of limit per batch
  $limit = SUPPORTGROUPS_FEED_REDIS_CACHE_BATCH_SIZE;
//  $from_timestamp = time();
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  // items in the ordered set are "scored" by timestamp, so since we want items to return in reverse order we use zRevRangeByScore
  $results = $client->zRevRangeByScore(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $uid,
    1000 * $from_timestamp,
    '-inf',
    array('withscores' => TRUE, 'limit' => array(0, $limit)));

  if (!is_array($results)) {
    $results = array();
  }
  else {
    $temp_results = $results;
    $results = array();

    // results from Redis are returned in the form data => timestamp, and timestamp is a float
    //, so we convert this to int when we process data before returning

    foreach ($temp_results as $activity_item => $timestamp) {
      $activity_item = $unserialize_return ? unserialize($activity_item) : $activity_item;
      $results[intval($timestamp)] = $activity_item;
    }
  }

  return $results;
}

/**
 * Helper function to get all users for which we currently have saved notification threads
 *
 * @return array
 */
function _supportgroups_get_notification_user_keys() {
  $client = _supportgroups_feed_get_cache_client();
  $allkeys = $client->keys(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-*');
  $uids = array();
  foreach ($allkeys as $key) {
    $uids[] = str_replace(SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-', '', $key);
  }
  return $uids;
}

/**
 * Helper function to return the current active Redis client
 * @return mixed
 */
function _supportgroups_feed_get_cache_client() {
  static $client = NULL;
  if (is_null($client)) {
    $client = Redis_Client::getClient();
  }

  return $client;
}

