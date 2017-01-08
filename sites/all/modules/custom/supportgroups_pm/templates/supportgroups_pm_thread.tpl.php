<?php
// template for each thread in list of private messages
/**
* Variables
*  //$author = name of the author
*  $body = pm body
*  $thread_id = the thread_id number. Needed for an ajax callout
*  $is_new = Determines if this message is new or not
* $timestamp =
* $image
* $author
* $delete
* $block
* $mark
* $recipient
*/
?>
<li class='threadrow <?php print $is_new; ?>' thread_id='<?php print $thread_id; ?>'>
    <span id='threadoptions-<?php print $thread_id; ?>' class='threadoptions' thread_id='<?php print $thread_id; ?>'>
        <?php print $delete; ?>
		<?php print $block; ?>
        <?php print $mark; ?>    
    </span>
    <a href="<?php print $base_url; ?>/privatemessages/<?php print $thread_id; ?>">
		<?php print $image; ?>
        <span class="conversation">
         Conversation with <?php print $recipients;?>
        </span>
        <span class="author">
          <?php print $author; ?>
        </span>
        <span class="time">
            <?php print $timestamp;?>
        </span>
        <span class="teaser">
            <?php print $body;?>
        </span>
    </a>
</li>
