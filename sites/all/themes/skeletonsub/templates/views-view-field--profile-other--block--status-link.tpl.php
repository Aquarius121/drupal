<?php
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
  * When fetching output from the $row, copy and paste the following snippet:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified.
  */
?>
<?php
  global $user;

  $remove_link = '';
  $query2 = 'SELECT rid FROM {user_relationships} WHERE rtid = 1 AND approved = 1 AND requestee_id = ' . arg(1) . ' AND requester_id = ' . $user->uid;
  $result2 = db_query($query2);
  $supportflag = 0;
  $rid = 0;
  foreach( $result2 AS $issupporter) {
                                $supportflag = 1;
                                $rid = $issupporter->rid;
   }

                        if($supportflag) {
                                $remove_link = '<div class="supporter-button"><a class="unroll button gray" href="/user/' . $user->uid . '/relationships/' . $rid . '/remove?destination=user/' . arg(1) . '">Supporting</a></div>';
                        } else {
                           if (_supportgroups_user_request_relationship_allowed()) {
                              $remove_link = '<div class="supporter-button"><a class="button green" href="/relationship/' . arg(1) . '/request/1?destination=user/' . arg(1) . '">Support Me</a></div>';
                           }
                        }

print $remove_link;
?>
