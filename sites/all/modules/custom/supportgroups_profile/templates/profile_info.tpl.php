<?php
/**
 * Profile info template
 * Variables
 * $supporters
 * $supportings
 * $username
 * $user_picture
 * $uid
 * $supporting_path
 * $supporters_path
 * $profile_url
 */
?>
<div class="post-statistics">
  <?php print $user_picture; ?>

	
  <div class="username">
    <?php print $username; ?>
  </div>

	<?php if($feed_link): ?>
  	<?php print $feed_link; ?>
	<?php endif; ?>
	
	<?php if($profile_link): ?>
  	<br/>
		<?php print $profile_link; ?>
	<?php endif; ?>

  <div class="stats">
    <div class="count">
			<?php print $supporting_link; ?>
    </div>

    <div class="count">
			<?php print $supporters_link; ?>
    </div>
  </div>
</div>