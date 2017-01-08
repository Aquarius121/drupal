<?php
/**
 *  Template for individual notification items
 * Variables
 * $username
 * $title
 * $description
 * $timestamp
 * $path;
 * $count;
 * $date;
 * $picture;
*/
?>
<li timestamp='<?php print $timestamp; ?>'>
  <a href='<?php print $path; ?>'<?php print isset($class) ? 'class="'.$class .'"' : ''?>>
    <span class="user-picture">
      <?php print $picture; ?> 
    </span>
    <span class="member">
      <?php print $username; ?>
      <?php print $count; ?>
    </span> 
    <span>
      <?php print $description; ?>
    </span>
    <span class="teaser">
      <?php print $title; ?> 
    </span>
    <span class="timestamp">
      <?php print $date; ?> 
    </span>
    </a>
</li>