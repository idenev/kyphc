<div id="container-page">
    <header class="clearfix" role="banner">
		<div id="header-left"></div>
		<div id="header-center">
	<?php if ($logo): ?>
		<div id="header-logo">
		  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
			<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
		  </a>
	  </div>
	<?php endif; ?>
	<?php print render($page['header']); ?>
		</div>		
		<div id="header-right"></div>
  </header>
  <div id="main-wrapper" class="clearfix">
  	<div id="header-menu-area">
  		<?php print render($page['menu_bar']); ?>
		<?php if ($highlighted = render($page['highlighted'])): print $highlighted; endif; ?>
  	</div>
	<div id="header-breadcrumb-area">
		<?php print $breadcrumb; ?>
	</div>
  	<?php print $messages; ?>
  	<?php print render($page['help']); ?>
	<?php if ($sidebar_first = render($page['sidebar_first'])): print $sidebar_first; endif; ?>
	<section id="main-content" role="main" class="clearfix">
		<?php if(arg(0) != 'front_page'): ?>
		<div id="left-navigation">
			<?php print render($page['left_nav']); ?>
		</div>
		<?php endif; ?>
		<div id="<?php print (arg(0) != 'front_page'?'node-content':'front-page-content'); ?>">
		  <?php print render($title_prefix); ?>
		  <?php if ($title): ?>
			<h1 id="page-title"><?php print $title; ?></h1>
		  <?php endif; ?>
		  <?php print render($title_suffix); ?>
	
		  <?php if ($tabs = render($tabs)): ?>
			<div class="tabs"><?php //print $tabs; ?></div>
		  <?php endif; ?>
	
		  <?php if ($action_links = render($action_links)): ?>
			<ul class="action-links"><?php print $action_links; ?></ul>
		  <?php endif; ?>
		  <?php 
			hide($page['content']['system_main']['nodes']);
		  	print render($page['content']); 
		  ?>
	  </div>
	</section>
	<?php if(arg(0) == 'front_page'): ?>
		  <?php print render($page['left_nav']); ?>
		  <?php endif; ?>
	<?php if ($sidebar_second = render($page['sidebar_second'])): print $sidebar_second; endif; ?>
  </div>
  <div id="content-bottom-bg">
  </div>
  <?php if ($footer = render($page['footer'])): ?>
	<footer><?php print $footer; ?>	</footer>
  <?php endif; ?>
  <?php if ($site_name || $site_slogan): ?>
	  <hgroup>
		<?php if ($site_name): ?>
		  <h1>
			<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
			  <?php print $site_name; ?>
			</a>
		  </h1>
		<?php endif; ?>
		<?php if ($site_slogan): ?>
		  <h2><?php print $site_slogan; ?></h2>
		<?php endif; ?>
	  </hgroup>
	<?php endif; ?>
</div>