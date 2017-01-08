<?php
/**
 * @file
 * Template for all support posts
 */
?>
<div class="<?php print $activity_type; ?>">
  <?php print $user_picture; ?>
  <a href="<?php print $activity_node->url; ?>" class="activity-post-content">
		<div class="activity-item-userinfo">
			<div class="username"><?php print $user_name; ?>
      </div>
			<div class="submitted">
				<?php print isset($last_comment_timestamp) ? $last_comment_timestamp : ''; ?> in
				<span class="group-type"><?php print $group_names; ?></span>
			</div>
		</div>
		<div class="body"><?php print $body_w_media; ?>
    </div>
		<div class="comments"><?php print $activity_node->comment_count; ?>
    </div>
  </a>
</div>