<?php
// template for each thread
/**
* Variables
*  $author = name of the author
*  $body = pm body
*  $thread_id = the thread_id number. Needed for an ajax callout
*  $is_new = Determines if this message is new or not
* $timestamp =
* $image
*/
?>
<li class='<?php print $is_new; ?>' timestamp='<?php print $unix_timestamp; ?>'>
  <a href="<?php print $base_url; ?>/privatemessages/<?php print $thread_id; ?>">
      <?php print $image; ?>
    <span class="member">
      <?php print $author;?>
    </span>
    <span class="time">
      <?php print $timestamp;?>
    </span>
    <span class="teaser">
      <?php print $body;?>
    </span>
  </a>
</li>
