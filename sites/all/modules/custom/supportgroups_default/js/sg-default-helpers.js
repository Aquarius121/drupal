// adding mobile device check using open source http://detectmobilebrowsers.com
(function(a){(jQuery.browser=jQuery.browser||{}).mobile=/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od|ad)|iris|kindle|lge |maemo|midp|mmp|mobile.+firefox|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(a)||/1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(a.substr(0,4))})(navigator.userAgent||navigator.vendor||window.opera);

(function($){
  // add mobile class to body if mobile true
  if($.browser.mobile) {
    $('body').addClass('mobile');
    
    // search the page for the discussion form and if it exists then edit the group text to be more explicit
    $("form.group-post-node-form #group-dropdown").html("Click below to select a group. <span='arrow'>&#9660;</span>");
    
  }
  
  Drupal.behaviors.supportgroups_default =  {
    attach: function(context, settings){
      // Check the remove image checkbox when this id is clicked
      $("#gear:not(.processed)").addClass('processed').click(function(){
          return false; 
        }
      );


      // check for interval already running to avoid attachBehaviors
      if(!Drupal.settings.supportgroups_default.userinfo.running) {
    	// set the recent interval id to running
        Drupal.settings.supportgroups_default.userinfo.running = 1;

        // Check to see if user is logged off
        if( Drupal.settings.supportgroups_default.userinfo.uid == 0  ){
          // check to for email arguments
          var url_source = window.location.href;
          var url_source_array = url_source.split("?");
          if( url_source_array.length == 2 ){
            var email_text = url_source_array[1];
          
            email_text = email_text.split("=");

            if( email_text.length == 2 ){
              if( email_text[1] == 'yes' ){
                $("#block-user-login").addClass('fromemail');
                $("#login-close").click(function(){
                  // remove class
                  $("#block-user-login").removeClass('fromemail');                
                });
              }
            }
          } // check for ?
          $(".comment-form-reply-button").click(function(){
              var nid = $(this).attr('nid');
              $("#anonymous-comment-body-"+nid).focus();          
          });
        }// End of user login check

      }
      
      // add the sticky class and body scroll functions once
      if(!$('body').hasClass('sticky-processed')) {
        $('body').addClass('sticky-processed');
        
        _process_user_preferences();
        
        // create sticky header
        $(window).scroll(function() {  
          stickyBody();  
        });
      }

    }
  };
  
  // checking for mobile browser and anonymous users
  //TODO Drupal.settings.supportgroups_default.userinfo.uid == 0 is not working in the console
  //TODO replace does login block exist statement...
  if($.browser.mobile && $('#edit-pass').length && !$.cookie('sg_mobile_anon')){
    console.log("We are both mobile and anonymous. Show mobile popup");
    
    // show the mobile popup and enable the links
    $('#mobile-popup').show();
    
    // enable the close-button
    $('#mobile-popup div.close-button').click(function(){
      $('#mobile-popup').hide();
      
      // set a cookie to prevent popup for 1 week
      $.cookie("sg_mobile_anon", true, { expires: 7 });
      
      return false;
    });
    
    // on mobile reigster link click, pop the register modal
    $('#mobile-register').click(function(){
      $("#block-supportgroups-user-supportgroups-register").show();
      $('#mobile-popup').hide();
      
      // forward to mobile register page
      // leaving relative to work in all environs
      window.location = '/welcome';
      return false;
    });
    
    // on mobile login link click focus the login form and highlight the fields
    $('#mobile-login').click(function(){
      $('#mobile-popup').hide();
      $('#edit-name').focus().effect("highlight", {}, 5000);
      $('#edit-pass').effect("highlight", {}, 5000);
      return false;  
    });
  }
}(jQuery));

// The private messaging notification requires this as well
// This function will recreate the cookie
function recreate_cookie(cookie_name){
  // create cookie on page load if it doesn't exist
  set_cookie(cookie_name);    
}

// This function will set the cookie
function set_cookie(cookie_name){
  // this logic returns the unix timestamp
  var new_timestamp = Math.round(new Date().getTime() / 1000) ;  
  // set cookie to expire in one day
  var exdate=new Date();
  exdate.setDate(exdate.getDate() + 1);
  document.cookie=cookie_name + "=" + new_timestamp+";expires="+exdate.toUTCString()+";domain=.supportgroups.com;path=/";
}

// This function will get the cookie
function get_cookie(cookie_name){
  var cookie_value = '';
  // break cookie into an array
  var cookie_array = document.cookie.split(';');
  var cookie_name_equalsign = cookie_name+'=';
  // loop through each cookie and find the cookie for the pm timestamp
  for( var i = 0; i < cookie_array.length; i++ ){
    // check to see if this is the cookie we are looking for
    if(cookie_array[i].indexOf(cookie_name) != -1){
      var single_cookie_array = cookie_array[i].split('=');          
      // get the cookie value
      cookie_value = single_cookie_array[1];
    }
  }      
  return cookie_value;
}// end of get cookie function

/**
 * Helper function to round seconds to the nearest coeff in milliseconds
 * Example: 60 * 1000 = 1 minute
 **/
function _get_current_time_rounded_to_coeff(coeff) {
  // round js calls to the minute
  var date = new Date();  //or use any other date
  return parseInt((Math.round(date.getTime() / coeff) * coeff) / 1000); 
}

function stickyBody() {
  if (jQuery(window).scrollTop() > 0) {   
    jQuery('body').addClass('sticky');  
  } else {  
    jQuery('body').removeClass('sticky');   
  }  
}

function _process_user_preferences(){
  // get user preferences
  //TODO function checking because this file is being loaded on Garland pages and jQuery might not be avail
  if(jQuery.cookie && 'function' == typeof(jQuery.cookie)) {
    preferences_s = jQuery.cookie('supportgroups_user_preferences');

    // if preferences exist act on them
    if(preferences_s && preferences_s.length) {
      preferences = jQuery.parseJSON(preferences_s);

      // check for hidden logo on scroll preference
      if('undefined' != typeof(preferences.hide_logo_on_scroll) && preferences.hide_logo_on_scroll != 0) {
        jQuery('#header .logo').addClass('hide');
      }
    }
  }
}