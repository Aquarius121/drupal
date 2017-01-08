<?php

function _supportgroups_feed_ajax_unflag() {
  $author_uid = $_REQUEST['uid'];
  $timestamp = $_REQUEST['timestamp'];
  // We need to check if the item was flagged since the last reset to know whether or not to deduct
  $notifications_unseen = supportgroups_user_get_notifications_unseen($author_uid);

  if ($timestamp > $notifications_unseen['timestamp']) {
    supportgroups_user_update_notifications_unseen($author_uid, FALSE);
  }

  exit;
}