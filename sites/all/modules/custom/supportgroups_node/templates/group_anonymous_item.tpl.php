<?php
// $Id$
/**
 * $user_current
 * $relationship
 * @file
 * Theming for individual activity item
 * $comment_status
 * $profile_url
 * $hearts
 */
?>
<div id='activity-item-<?php print $activity_item->nid ?>' nid='<?php print $activity_item->nid ?>' <?php print $time ? "time='". $time ."' " : ''; ?> class='activity-item-wrapper' >
  <a href='<?php print $node_url; ?>'>
	<!-- user image, name, points, date -->
	<div class='activity-item-userinfo'>
    	<?php print $user_picture; ?>
	    <h5>
			<span id='user-profile-container-<?php print $activity_item->nid; ?>' nid='<?php print $activity_item->nid; ?>' class='user-profile-container'>
  	        <span class="username"><?php print $current_user->name; ?> </span>
          </span>
	    </h5>
    	<div class='when-where'>
      		<?php print $date; ?><span class='in-group'> in <?php print strip_tags($group_list); ?></span>
	    </div>
	</div>
	<div id='activity-item-body-<?php print $activity_item->nid; ?>' class='activity-item-body'>
        <?php print $discussion_body; ?>
	</div>
	<div id='activity-item-info-<?php print $activity_item->nid; ?>' class='activity-item-info'>
        <span class="heart"> &hearts; </span>
	    <span id='activity-item-info-comments<?php print $activity_item->nid; ?>' class='activity-item-info-comments comments' title="Number of comments">&nbsp;<?php print $activity_item->comment_count; ?></span>
	</div>

  </a>
</div>