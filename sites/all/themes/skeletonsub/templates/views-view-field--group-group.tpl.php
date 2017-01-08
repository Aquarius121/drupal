<?php
// $Id: views-view-field.tpl.php,v 1.1 2008/05/16 22:22:32 merlinofchaos Exp $
 /**
  * This template is used to print a single field in a view. It is not
  * actually used in default Views, as this is registered as a theme
  * function which has better performance. For single overrides, the
  * template is perfectly okay.
  *
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, this construct should be used:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified.
  */
?>
<?php
  global $user;
  if($user->uid > 0){  
    if(strpos($output, 'Subscribe')){
     $output =  preg_replace( '/Subscribe to group/', 'Join', $output);
    }
    elseif(strpos($output, 'Unsubscribe')) {
     $output =  preg_replace( '/Unsubscribe from group/', 'Leave', $output);
    }
    elseif( strpos($output,'Request group membership') ){
    	$output =  preg_replace( '/Request group membership/', 'Join', $output);  		
		}
  } else{
    // change join button text for anonymous users
    if( arg(0) == 'node' && arg(1) == 18719){
			$output =  preg_replace( '/Request group membership/', 'Join', $output);  		
		} else{
    	$output =  preg_replace( '/Request group membership/', 'Join - We want you here!', $output);  
		}
  }
  print $output;
?>
