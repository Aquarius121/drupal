<?php
// $Id$
/**
 * $user_current
 * $relationship
 * @file
 * Theming for individual activity item
 * $comment_status
 */
?>

<div id='activity-item-<?php print $activity_item->nid ?>' nid='<?php print $activity_item->nid ?>' <?php print $time ? "time='". $time ."' " : ''; ?> class='activity-item-wrapper' >
	<!-- Flag/Edit/Delete options -->
	<div id='activity-item-options-<?php print $activity_item->nid; ?>' class='activity-item-options'>
        <span id='activity-item-options-flag-<?php print $activity_item->nid; ?>' class='activity-item-options-flag' title="Flag this post"><?php print $spam; ?></span>
        <?php
          if(isset( $discussion_options)){
            print $discussion_options;
          }
        ?>
	</div>
	<!-- user image, name, points, date -->
	<div class='activity-item-userinfo'>
    	<?php print $user_picture; ?>
	    <h5>
			<span id='user-profile-container-<?php print $activity_item->nid; ?>' nid='<?php print $activity_item->nid; ?>' class='user-profile-container'>
  	        <a class="username" href='<?php print $profile_url; ?>' <?php if(isset($no_link)){ print $no_link;} ?> ><?php print $current_user->name; ?> </a>
			<!-- rollover mini profile -->
            <span id='user-profile-mini-block-<?php print $activity_item->nid; ?>' class='user-profile-mini-block'>
                <span class='mini-profile-image'><?php print $user_picture_mini; ?></span>
                <a href='<?php print $profile_url; ?>' <?php if(isset($no_link)){ print $no_link;} ?> ><span class='mini-profile-name'><?php print $current_user->name; ?></span></a>
				<span class="mini-points">[<?php print $current_user->points; ?>]</span>
				<span class="user-badges"><?php print $user_badges; ?></span>
                <span class='mini-profile-bio'><?php print str_replace(array('”','“','"'), array('','',''), substr($current_user->profile_bio,0,100)) . "&hellip;"; ?></span>
				<span class='mini-profile-relationship'><?php if(isset($no_link)){}else{print $relationship;} ?></span>
            </span><!-- /rollover mini profile -->
          </span>
          <span id='userpoints-container-<?php print $activity_item->nid; ?>' nid='<?php print $activity_item->nid; ?>' class='userpoints-container'>
    	    [<?php print $current_user->points; ?>]
          </span>
	    </h5>
    	<div class='when-where'>
      		<a href='<?php print $node_url; ?>'><?php print $date; ?></a><span class='in-group'> in <?php print $group_list; ?></div>
	    </span>
	</div>
	<div id='activity-item-body-<?php print $activity_item->nid; ?>' class='activity-item-body'>
      <?php print $discussion_body; ?>
	</div>
    <span id='comment-postbutton-<?php print $activity_item->nid; ?>' class='comment-postbutton' nid='<?php print $activity_item->nid; ?>'>
      Comment
    </span>
	<div id='activity-item-info-<?php print $activity_item->nid; ?>' class='activity-item-info'>
    	<span id='activity-item-info-hearts-<?php print $activity_item->nid; ?>' class='activity-item-info-hearts hearts' title="Empathize"><?php print $hearts; ?></span>
	    <span id='activity-item-info-comments<?php print $activity_item->nid; ?>' class='activity-item-info-comments comments' title="Number of comments">&nbsp;<?php print $activity_item->comment_count; ?></span>
	</div>
    <?php
      if(isset( $more_comments_button )){
        print $more_comments_button;
      }
    ?>
    <div id='activity-item-comments-<?php print $activity_item->nid; ?>' class='activity-item-comments  <?php print $comment_status; ?>'>
	  <?php
	    if(isset($comments)){
	      print $comments;
	    }
	  ?>
    </div>
    <div id='comment-form-<?php print $activity_item->nid; ?>' class='comment-form' nid='<?php print $activity_item->nid; ?>'>
      <?php
        if(isset($comment_form)){
          print $comment_form;
        }
      ?>
    </div>
</div>