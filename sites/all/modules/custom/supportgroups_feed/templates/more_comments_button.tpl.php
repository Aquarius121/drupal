<?php
/* Template for the view all comment button
 * $comment_count
 * $nid
 */
?>
<div id='show-more-comments-container-<?php print $nid; ?>' nid='<?php print $nid; ?>' class='show-more-comments-container'>
  <div id='show-more-comments-<?php print $nid; ?>' nid='<?php print $nid; ?>' class='show-more-comments'>
    <?php print $comment_count; ?>
  </div>
  <div class='waiting-icon'>
  </div>
</div> 