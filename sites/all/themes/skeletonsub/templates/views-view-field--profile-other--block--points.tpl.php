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
  $points = $row->userpoints_total_points;
  if(module_exists('supportgroups_utility')){
    // get userpoints
    $badges = supportgroups_utility_get_badges($points);  
  }  
print "<div class='profile-userpoints'> Support Points: ".$points."</div>";
print $badges;


$supportingcount;
$supportercount;              
              $supportingcountresult = db_query('SELECT COUNT(requestee_id) as thecount FROM {user_relationships} WHERE approved = 1 AND rtid = 1 AND requester_id = ' . arg(1));
              foreach( $supportingcountresult AS $temp){
                $supportingcount = $temp->thecount;
                break;
              } 
              
              $supportercountresult = db_query('SELECT COUNT(requester_id) as thecount FROM {user_relationships} WHERE approved = 1 AND rtid = 1 AND requestee_id = ' . arg(1));
              foreach( $supportercountresult AS $temp2){
                $supportercount = $temp2->thecount;
                break;
              }

              if( $supportingcount == 0){
                print '<div class="count"><span class="cat">Supports</span>' . $supportingcount . '</div>';
              }
              else{
                print '<div class="count">' . l('<span class="cat">' . t('Supports') . '</span>' . $supportingcount, 'user/'.arg(1).'/supporting', array('absolute' => true, 'html' => true)) . '</div>';
              }

              if( $supportercount == 0){
                print '<div class="count"><span class="cat">Has</span>' . $supportercount . ' Supporters</div>';
              }
              else{
                print '<div class="count">' .  l('Has ' . $supportercount . ' ' . format_plural($supportercount, 'Supporter', 'Supporters'), 'user/'.arg(1).'/supporters', array('absolute' => true, 'html' => true)) . '</div>';
              }


?>
