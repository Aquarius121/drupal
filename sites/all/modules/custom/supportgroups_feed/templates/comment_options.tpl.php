<?php
/*
 *  Template for comment options
 * $cid
 * $nid
 * $pid
 */
?>
<span id='activity-item-comments-options-delete-<?php print $cid; ?>' cid='<?php print $cid ?>' class='activity-item-comments-options-delete' title="Delete this post">
  &nbsp;
</span>
<div id='deletecommentcontainer-<?php print $cid; ?>' class='deletecommentcontainer'>
  <p>Are you sure you want to delete this comment?</p>
  <span id='activity-item-comments-options-delete-confirm-<?php print $cid; ?>' cid='<?php print $cid ?>' class='activity-item-comments-options-delete-confirm orangebutton' title="Delete this post">Delete</span>
  <span id='activity-item-comments-options-delete-cancel-<?php print $cid; ?>' cid='<?php print $cid ?>' class='activity-item-comments-options-delete-cancel graybutton' title="Delete this post">Cancel</span>
</div>
<span id='activity-item-comments-options-edit-<?php print $cid; ?>' cid='<?php print $cid; ?>' pid='<?php print $pid; ?>' nid='<?php print $nid; ?>'  class='activity-item-comments-options-edit' title="Edit this post">&nbsp;</span>
