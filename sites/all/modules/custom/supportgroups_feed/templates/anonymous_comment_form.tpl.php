<?php
 /*
 * Template for anonymous user comment form
 *
 *$nid
 *$pid
 */
?>
<form id='anonymous-comment-form-<?php print $nid; ?>' class='anonymous-comment-form' method='post' action=''>
  <textarea id='anonymous-comment-body-<?php print $nid; ?>' class='anonymous-comment-body' placeholder="Lend your support"></textarea>
  <input type='button' id='anonymous-comment-button-<?php print $nid; ?>' nid='<?php print $nid; ?>' pid='<?php print $pid; ?>' class='anonymous-comment-button' value='Post'/>
</form>