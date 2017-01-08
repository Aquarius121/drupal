<?php
/**
 * @file
 * Apache solr functions
 */

/**
 * Implementation of hook_apachesolr_search_result_alter
 * Peel off Node ID, user ID, and unserialize the info payload before going into theming
 *
 * @param ApacheSolrDocument $document
 * @param array $extra
 * @param DrupalSolrQueryInterface $query
 */
function supportgroups_feed_apachesolr_search_result_alter($document, array &$extra, DrupalSolrQueryInterface $query) {

  // we only want to do this for support-feed queries, so simply return if not doing one of these queries
  // NOTE this must be kept in sync with the names used in any custom queries in order to work properly
  if (!in_array($query->getName(), array('sg_get_past_activity_for_user', 'sg_get_past_activity_for_group', 'sg_get_past_activity'))) {
    return ;
  }

  // peel off uid, node ID, and unserialized cached data object for theming
  //_supportgroups_feed_solr_current_activity_query_static_activity_cache($document);
  supportgroups_feed_apachesolr_current_activity_query_static_activity_cache($document);
  supportgroups_feed_apachesolr_current_activity_query_static_nid_cache($document->entity_id);
  supportgroups_feed_apachesolr_current_activity_query_static_user_cache($document);
}

/**
 * Helper function to build SOLR search params for activity searches (e.g. past activity for support feed/group activities)
 *
 * @param Int $before_timestamp
 * @param Int $num_rows
 * @return Array
 */
function supportgroups_feed_apachesolr_get_basic_activity_search_params($before_timestamp = 0, $num_rows = SUPPORTGROUPS_FEED_DEFAULT_NUM_RECORDS) {
  if (!$before_timestamp) {
    $before_timestamp = time();
  }

  $params = apachesolr_search_basic_params();
  $params['fq'][] = 'bundle:group_post';

  $params['fq'][] = 'ds_last_comment_or_change:[* TO '. apachesolr_date_iso($before_timestamp) .']';

  $params['fl'][] = 'zs_sf_activity_item';
  $params['fl'][] = 'ds_last_comment_or_change';
  $params['rows'] = $num_rows;

  return $params;
}

/**
 * Implementation of hook_apachesolr_index_document_build_ENTITY_TYPE
 *
 * @param ApacheSolrDocument $document
 * @param stdClass $entity
 * @param String $env_id
 */
function supportgroups_feed_apachesolr_index_document_build_node(ApacheSolrDocument $document, $entity, $env_id) {
  if ($entity->type == 'group_post') {
    $item = supportgroups_feed_get_activity_item($entity->nid, TRUE);
    $document->setField('zs_sf_activity_item', base64_encode(serialize($item)));
  }
}

/**
 * Implementation of hook_apachesolr_query_prepare
 * Add additional filters as required for search
 *
 * @param DrupalSolrQueryInterface $query
 */
function supportgroups_feed_apachesolr_query_prepare(DrupalSolrQueryInterface $query) {
  $query_name = $query->getName();
  if (!in_array($query_name, array('sg_get_past_activity_for_user', 'sg_get_past_activity_for_group', 'sg_get_past_activity'))) {
    return;
  }

  // create master subquery to add all of the content restrictions
  $subquery = apachesolr_drupal_subquery('OR');

  // track max number of items just to make sure we don't run into issues
  // and add too many parameters to the subquery
  $max_params = SUPPORTGROUPS_FEED_APACHESOLR_MAX_SUBQUERY_ITEMS;
  $subquery_param_count = 0;

  $uid_list = array();
  $group_list = array();
  if ($query_name == 'sg_get_past_activity_for_user') {

    // extract UID from passed-in filters
    $filter = $query->getFilters('uid');
    // remove this-- we'll pass it in
    $query->removeFilter('uid');
    $filter = current($filter);
    $uid = $filter['#value'];

    if (!user_access('view full support feed')) {

      $user_info = supportgroups_feed_get_user_info($uid);
      $uid_list = $user_info->supports;
      $uid_list[] = $uid;

      $group_list = $user_info->groups;
    }
  }
  else if ($query_name == 'sg_get_past_activity_for_group') {
    // extract GID from passed-in filters
    $filter = $query->getFilters('gid');
    // remove this-- we'll pass it in
    $query->removeFilter('gid');
    $filter = current($filter);
    $gid = $filter['#value'];

    $group_list[] = $gid;
  }

  // add group list parameters to query
  if (count($group_list)) {
    $subquery_param_count++;
    $list = '"node:'. join('" OR "node:', $group_list) .'"';
    if (count($group_list) > 1) {
      $list = '('. $list .')';
    }
    $subquery->addFilter('sm_og_group_ref', $list);
  }

  // add uid list parameters to query
  if (count($uid_list)) {
    $subquery_param_count++;
    $list = join(' OR ', $uid_list);
    if (count($uid_list) > 1) {
      $list = '('. $list .')';
    }
    $subquery->addFilter('is_uid', $list);
  }

  // set to sort by most recent change or comment (descending)
  $query->setAvailableSort('ds_last_comment_or_change', array(
    'title' => t('Last Comment or Change'),
    'default' => 'desc',
  ));
  $query->setSolrsort('ds_last_comment_or_change', 'desc');

  if ($subquery_param_count) {
    // add group/subscribed user constraints to query
    $query->addFilterSubQuery($subquery);
  }
}
