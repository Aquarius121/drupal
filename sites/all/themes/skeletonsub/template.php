
<?php
// calling out css file for supporters and supporting view pages
define('SKELETONSUB_THEME_PATH', drupal_get_path('theme', 'skeletonsub'));

function skeletonsub_preprocess_views_view(&$vars) {
	switch($vars['view']->name) {
		case 'supportlist':
			if($vars['view']->current_display == 'page' || $vars['view']->current_display == 'page_1') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/supporters.css');
			}
			break;
		// call out css file for profile other pages
		case 'profile_other':
			if($vars['view']->current_display == 'block' || $vars['view']->current_display == 'block') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/profileother.css');
			}
			break;
		// call out css file for support someone
    case 'og_all_user_group_content':
		case 'master':
			if($vars['view']->current_display == 'page' || $vars['view']->current_display == 'page_1') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');
			}
			break;
		// call out css file for one and two comments views
		case 'one_and_two_comments':
			if($vars['view']->current_display == 'page'
				|| $vars['view']->current_display == 'page_1'
				|| $vars['view']->current_display == 'page_2') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');
			}
			break;
		case 'one_and_two_comments':
			if($vars['view']->current_display == 'page'
				|| $vars['view']->current_display == 'page_1'
				|| $vars['view']->current_display == 'page_2') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');
			}
			break;
		// call out css file for group details view at top of groups pages
		case 'group_description':
			if($vars['view']->current_display == 'block' || $vars['view']->current_display == 'block') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/group-details.css');
			}
			break;

		// call out css file for all groups page
		case 'online_support_groups':
			if($vars['view']->current_display == 'block' || $vars['view']->current_display == 'block_1') {
				 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/allgroups.css');
			}
			break;

	// call out css file for blogs and blog page
			case 'blog':
				if($vars['view']->current_display == 'page') {
					 drupal_add_css(SKELETONSUB_THEME_PATH . '/css/blog.css');
				}
				break;
	}
}

// call out css file for front page
function skeletonsub_preprocess_page(&$vars) {
  global $user;
	global $base_url;

	$vars['base_url'] = $base_url;

	if(user_is_anonymous()) {
		$vars['header_logo_link_url'] = $base_url;
	} else {
		$vars['header_logo_link_url'] = $base_url . '/' . drupal_get_path_alias('user/' . $user->uid);

		// force flag.js to appear on all pages
		drupal_add_js(drupal_get_path('module', 'flag') . '/theme/flag.js');
	}

  if($vars['is_front']) {
		drupal_add_css(SKELETONSUB_THEME_PATH . '/css/frontpage.css');
		drupal_add_css(SKELETONSUB_THEME_PATH . '/css/someone.css');
	}
  // Check to see if we are on the user edit page
  // If so, we will send them to the change password page
  if( arg(0) == 'user' && arg(2) == 'edit' ){
    if(!user_access('administer users')){
      drupal_goto( 'user/'.$user->uid.'/changepassword');
    }
  }

	//	drupal_set_message("If this is an emergency, please call 911.<br/>
	//										If you are in crisis, please call<br/>
	//										800-SUICIDE (784-2433)<br/>
	//										800-442-HOPE (4673)<br/>
	//										Twenty-four hours a day, seven days a week", "warning");
}

function skeletonsub_preprocess_node(&$variables) {
	if ($variables['type'] == 'blog') {
		drupal_add_css(SKELETONSUB_THEME_PATH . '/css/blog.css');
	}

	if ($variables['type'] == 'videos') {
		drupal_add_css(SKELETONSUB_THEME_PATH . '/css/videos.css');
	}
}

/**
 * Override core link theme function
 * Hand select links before rendering them to avoid subdomain issues
 * @param array $variables
 * @return string
 */
function skeletonsub_menu_link(array $variables) {

  $element = $variables['element'];
  // change link on non live sites
  if (strpos($element['#href'],'online.supportgroups.com')) {

    switch (variable_get('supportgroups_environment', '')) {
      case 'dev':
        $element['#href'] = str_replace('online.supportgroups.com', 'online.devnew.supportgroups.com',
          $element['#href']);
        break;
      case 'local':
        $element['#href'] = '/node/18719';
        break;
    }
  }
  // override absolute links for gear menu
  foreach($variables['menu'] as $key => $item) {
    if($item['link']['menu_name'] == 'menu-member-gear') {
      $element['#localized_options']['absolute'] = true;

      // allow overrides for user/me to hook the user/% views
      if(strpos($element['#href'], 'user/me') !== FALSE) {
        global $user;
        $element['#href'] = str_replace('user/me', 'user/'.$user->uid, $element['#href']);
      }
      break;
    }
  }

  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}