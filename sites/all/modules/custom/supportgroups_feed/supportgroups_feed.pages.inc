<?php

/**
 * Page callback for the support someone page
 */
function _supportgroups_feed_support_someone_page() {
  drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');

  // landing page for support someone
  $data = supportgroups_feed_get_recent_activity();

  $output = "";
  foreach($data['activity'] as $nid => $activity_node) {
    $activity_node_user = $data['activity']['users'][$activity_node->uid];
    $output .= theme('sg_f_activity_post', array(
      'activity_type' => 'front-posts',
      'activity_node' => $activity_node,
      'activity_node_user' => $activity_node_user
    ));
  }
  return $output;
}


/**
 * Page callback for the All Support Groups page
 */
function _supportgroups_feed_all_support_groups_page() {
  // landing page for support someone
  $data = supportgroups_feed_get_recent_activity(0, 20, array());
  drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');

  //TODO Steve--theme here
  //TODO supportgroups_feed_get_recent_activity is the same callback needed for master::block_1 ("latest home page") and master::block_2 ("latest posts") displayed
  $output = "";
  foreach($data['activity'] as $nid => $activity_node) {
    $activity_node_user = $data['activity']['users'][$activity_node->uid];
    $output .= theme('sg_f_activity_post', array(
      'activity_type' => 'front-posts',
      'activity_node' => $activity_node,
      'activity_node_user' => $activity_node_user
    ));
  }
  return $output;
}

function _supportgroups_feed_onecomment_page() {
  // landing page for support someone
  if(isset($_GET['page'])){
    $currentPage = intval($_GET['page']);
  } else {
    $currentPage = 1;
  }

  if(isset($_GET['t'])){
    $totalCount = intval($_GET['t']);
  } else {
    $totalCount = supportgroups_feed_get_onecomment_count();
  }

  $nids = supportgroups_feed_get_onecomment_list($currentPage);

  $activity_items = supportgroups_feed_get_activity_items(array_values($nids));
  
  drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');

  //TODO Steve--theme here
  //TODO supportgroups_feed_get_recent_activity is the same callback needed for master::block_1 ("latest home page") and master::block_2 ("latest posts") displayed
  $output = "";
  foreach($activity_items as $nid => $activity_node) {
    $activity_node_user = $activity_items['users'][$activity_node->uid];
    $output .= theme('sg_f_activity_post', array(
      'activity_type' => 'front-posts',
      'activity_node' => $activity_node,
      'activity_node_user' => $activity_node_user
    ));
  }
  $output .= generatePage($currentPage, $totalCount);
  return $output;
}

/**
 * Pagination for onecomment
 */
function generatePage($current, $total){
  $max = intval(($total + 20) / 20);
  $str = '<div class="item-list"><ul class="pager">';

  if($current > 1)
    $str .= '<li class="pager-first first"><a title="Go to first page" href="/onecomment?page=1&t='.$total.'">« first</a></li>
    <li class="pager-previous"><a title="Go to previous page" href="/onecomment?page='.($current-1).'&t='.$total.'">‹ previous</a></li>';

  $sdt = false;

  if($current < 3){
    $start = 1;
    $last = 5;
    $lsdt = true;
  } else if($current > $max - 2){
    $fsdt = true;
    $start = $max - 4;
    $last = $max;
  } else {
    $lsdt = true;
    $fsdt = true;
    $start = $current - 2;
    $last = $current + 2;
  }

  if($fsdt)
      $str .= '<li class="pager-item">...</li>';

  for($i = $start; $i<=$last; $i++){
      if($i != $current){
        $str .= '<li class="pager-item"><a title="Go to page '.$i.'" href="/onecomment?page='.$i.'&t='.$total.'">'.$i.'</a></li>';
      } else {
        $str .= '<li class="pager-current">'.$current.'</li>';
      }
  }

  if($lsdt)
      $str .= '<li class="pager-item">...</li>';

  if($current < $max){
    $str .='<li class="pager-next"><a title="Go to next page" href="/onecomment?page='.($current + 1).'&t='.$total.'">next ›</a></li>
      <li class="pager-last last"><a title="Go to last page" href="/onecomment?page='.$max.'&t='.$total.'">last »</a></li>';    
  }
  $str .= '</ul></div>';
  return $str;
}

/**
 * Page callback for user supporting page
 */
function _supportgroups_feed_user_supporting($user) {
  drupal_add_css(SKELETONSUB_THEME_PATH . '/css/supporters.css');

  $user_info = supportgroups_feed_get_user_info($user->uid);
  $users_supported_by_uid = $user_info->supports;

  $output = "<div class='view view-supportlist view-id-supportlist view-display-id-page'>\n";
  foreach($users_supported_by_uid as $uid) {
    $supported_by_user = user_load((int) $uid);
    if($supported_by_user) {
      $output .= theme('sg_f_user_support_profile', array('user' => $supported_by_user));
    }
  }
  $output .= "</div>\n";

  return $output;
}

/**
 * Page callback for user supporters page
 */
function _supportgroups_feed_user_supporters($user) {
  drupal_add_css(SKELETONSUB_THEME_PATH . '/css/supporters.css');


  $user_info = supportgroups_feed_get_user_info($user->uid);
  $users_supporting_uid= $user_info->supported_by;

  $output = "<div class='view view-supportlist view-id-supportlist view-display-id-page'>\n";
  foreach($users_supporting_uid as $uid) {
    $supported_user = user_load((int) $uid);
    if($supported_user) {
      $output .= theme('sg_f_user_support_profile', array('user' => $supported_user));
    }
  }
  $output .= "</div>\n";

  return $output;
}
