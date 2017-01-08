<div id="wrap">

  <?php if ($page['full_width']): ?>
    <?php print render($page['full_width']); ?>
  <?php endif; ?>

	<!-- #header -->
	<?php if ($page['header_right']) { ?>
    <div id="header" class="five columns clearfix">
  <?php } else { ?>
    <div id="header" class="sixteen columns clearfix">
  <?php } ?>

      <!-- .inner -->
      <div class="inner">
			<!-- ORIGINAL LOGO CALLOUT THAT INTEGRATES WITH THEME SETTINGS
        <?php //if ($logo): ?>
          <a href="<?php print $base_url; ?>" title="<?php //print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php //print $logo; ?>" alt="<?php //print t('Home'); ?>" />
          </a>
        <?php //endif; ?>
-->

				<a href="<?php print $header_logo_link_url; ?>" class="homelink"><span class="logo"></span></a>

        <!-- #navigation -->
        <div id="navigation">
          <?php if ($page['header']) : ?>
            <div class="menu-header">
              <?php print drupal_render($page['header']); ?>
				    </div><!-- /menu-header -->
          <?php endif; ?>
			  </div><!-- /#navigation -->
		</div><!-- /inner -->
	</div><!-- /#header -->

	<?php if ($page['header_right']) : ?>
    <!-- #header-right -->
    <div id="header-right" class="eleven columns clearfix">

        <div class="inner">
	        <?php print render($page['header_right']); ?>
        </div>

	  </div><!-- /#header-right -->
	<?php endif; ?>

	<div class="clear"></div>

  <div class="container">
    <div id="content" class="ten columns">
      <?php if ($messages): ?>
        <div id="messages">
        	<?php print $messages; ?>
	      </div><!-- /#messages -->
      <?php endif; ?>

      <div id="main">

		    <?php if ($page['highlighted']): ?>
		      <div id="highlighted" class="clearfix">
		        <?php print render($page['highlighted']); ?>
		      </div>
		    <?php endif; ?>

        <?php print render($title_prefix); ?>

        <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>

        <?php print render($title_suffix); ?>

		    <?php if ($tabs && $is_admin): ?>
          <div class="tabs">
            <?php print render($tabs); ?>
          </div>
        <?php endif; ?>

        <?php print render($page['help']); ?>

        <?php if ($action_links): ?>
          <ul class="action-links">
			      <?php print render($action_links); ?>
          </ul>
        <?php endif; ?>

		    <?php if ($page['content_top']): ?>
			    <?php print render($page['content_top']); ?>
        <?php endif; ?>

        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>

		    <?php if ($page['content_bottom']): ?>
			    <?php print render($page['content_bottom']); ?>
        <?php endif; ?>

      </div><!-- /main -->

    </div><!-- /#content -->

    <?php if ($page['sidebar_first']): ?>
    <!-- #sidebar-first -->
    <div id="sidebar-first" class="six columns">
        <?php print render($page['sidebar_first']); ?>
    </div><!-- /#sidebar-first -->
    <?php endif; ?>

    <?php if ($page['sidebar_second']): ?>
    <!-- #sidebar-second -->
    <div id="sidebar-second" class="five columns">
        <?php print render($page['sidebar_second']); ?>
    </div><!-- /#sidebar-second -->
    <?php endif; ?>

    <div class="clear"></div>

    <?php if ($page['featured_left'] || $page['featured_right']): ?>
    <!-- #featured -->
    <div id="featured" class="sixteen columns clearfix">
        <?php if ($page['featured_left'] && $page['featured_right']) { ?>
        <div class="one_half">
        	<?php print render($page['featured_left']); ?>
        </div>

        <div class="one_half last">
        	<?php print render($page['featured_right']); ?>
        </div>
        <?php } else { ?>

        <?php print render($page['featured_left']); ?>
        <?php print render($page['featured_right']); ?>

        <?php } ?>

    </div><!-- /#featured -->
    <?php endif; ?>

	</div><!-- /container ten columns -->

	<div id="footer">
        <div class="container">
        	<div class="sixteen columns clearfix">

                <div class="one_third">
                	<?php if ($page['footer_first']): ?><?php print render($page['footer_first']); ?><?php endif; ?>
                </div>

                <div class="one_third">
                	<?php if ($page['footer_second']): ?><?php print render($page['footer_second']); ?><?php endif; ?>
                </div>

                <div class="one_third last">
                	<?php if ($page['footer_third']): ?><?php print render($page['footer_third']); ?><?php endif; ?>
                </div>

                <div class="clear"></div>

                <?php if ($page['footer']): print render($page['footer']); endif; ?>

                <div class="clear"></div>

        	</div>
        </div>
    </div><!-- /footer -->

		<?php if (isset($mobile_popup) && $mobile_popup): print $mobile_popup; endif; ?>

    <?php if($page['closure']): ?>
      <?php print render($page['closure']); ?>
    <?php endif; ?>
</div> <!-- /#wrap -->
