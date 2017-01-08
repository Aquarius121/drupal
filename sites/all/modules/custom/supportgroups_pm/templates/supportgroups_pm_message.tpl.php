<?php
// template for each message
/**
* Variables
* $author = name of author
* $body = body of message
* Timestamp = date the message was created
* $image
*/
?>
<li>
    	<?php print $image; ?>
    <span class="member">
   		<?php print $author; ?>
    </span>
    <span class="time">
    	<?php print $timestamp; ?>
    </span>
    <span class="teaser">
    	<?php print $body; ?>
    </span>
</li>
