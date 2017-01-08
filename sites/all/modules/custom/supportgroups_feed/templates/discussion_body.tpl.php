<?php
/**
 * Template for the discussion content
 * Variables
 * $nid
 * $body
 * $extra
 * $length
 * $showmore
 *
 */
?>
<?php print $extra; ?>
<span id='activity-item-body-text-<?php print $nid; ?>' class='activity-item-body-text <?php print $length; ?>'>
  <?php print $body; ?>
</span>
<?php if(isset($showmore)){print $showmore;} ?>
