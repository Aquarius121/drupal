<?php
// $Id$
/**
 * $user_picture
 * $date
 * $current_user
 * $profile_url
 * $hearts
 * $spam
 * $has_pid
 * $pid
 * $length
 * $body
 * $showmore
 * @file
 * Theming for individual comment item
 */
?>
<div id='activity-item-comment-<?php print $comment->cid; ?>'  cid='<?php print $comment->cid; ?>' pid='<?php print $pid; ?>' time='<?php print $comment->changed ?>' class='activity-item-comment-wrapper <?php print $has_pid; ?>'>

    <div id='comment-options-<?php print $comment->cid; ?>' class='comment-options'>
        <span id='activity-item-options-comment-flag-<?php print $comment->nid; ?>' class='activity-item-options-comment-flag' title="Flag this post"><?php print $spam; ?></span>
    	<?php
    	  if( isset($comment_options) ){
    	    print $comment_options;
    	  }
    	?>
    </div>

  <div class='activity-item-comment-userinfo'>
    <?php print $user_picture; ?>
    <h5>
      <span id='user-profile-container-comment-<?php print $comment->cid; ?>' cid='<?php print $comment->cid; ?>' class='user-profile-container-comment'>
        <a class="username" href='<?php print $profile_url; ?>' <?php if(isset($no_link)){ print $no_link;} ?> ><?php print $comment->name; ?> </a>
        <span id='user-profile-mini-block-comment-<?php print $comment->cid; ?>' class='user-profile-mini-block-comment'>
          <span class='mini-profile-image'><?php print $user_picture_mini; ?></span>
          <a href='<?php print $profile_url; ?>' <?php if(isset($no_link)){ print $no_link;} ?> ><span class='mini-profile-name'><?php print $comment->name; ?></span></a>
          <span class="mini-points">[<?php print $comment->points; ?>]</span>
					<span class="user-badges"><?php print $user_badges; ?></span>
          <span class='mini-profile-bio'><?php print str_replace(array('“','”','"'),array('','',''), substr($comment->profile_bio,0,100)) . "&hellip;"; ?></span>
          <span class='mini-profile-relationship'><?php if(isset($no_link)){}else{print $relationship;} ?></span>
        </span>
      </span>
      <span id='userpoints-container-comment-<?php print $comment->cid; ?>' cid='<?php print $comment->cid; ?>' class='userpoints-container-comment'>
    	[<?php print $comment->points; ?>]
      </span>

    </h5>
    <span><?php print $date; ?> </span>
  </div>
  <div id='activity-item-comment-body-<?php print $comment->cid; ?>' class='activity-item-comment-body <?php print $length; ?>'>
    <?php print $body; ?>
  </div>

  <?php if(isset($showmore)){ print $showmore;} ?>

  <div id='activity-item-comment-info-<?php print $comment->cid; ?>' class='activity-item-comment-info'>
    <span id='activity-item-comment-info-hearts-<?php print $comment->cid; ?>' class='activity-item-comment-info-hearts hearts'><?php print $hearts; ?></span>
    <span id='activity-item-comment-info-comments-<?php print $comment->cid; ?>' class='activity-item-comment-info-comments comments'></span>
  </div>
  <div class='comment-form-field'>
    <span id='comment-form-reply-button-<?php print $comment->cid; ?>' class='comment-form-reply-button' nid='<?php print $comment->nid; ?>' pid='<?php print $pid; ?>' cid='<?php print $comment->cid; ?>' username='<?php print $comment->name; ?>'>Reply</span>
    <div id='comment-form-container-<?php print $comment->cid; ?>' class='comment-form-container'>
    </div>
  </div>
  <div class='clear-block clearfix'></div>
</div>