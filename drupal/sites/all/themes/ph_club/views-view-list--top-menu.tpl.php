<?php
/**
 * @file views-view-list.tpl.php
 * Default simple view template to display a list of rows.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * @ingroup views_templates
 */
 
 // Build the content list
 $active_tab = false;
 $menu_list = '';
 foreach ($rows as $id => $row){
	 $menu_list .= '
      <li class="'.$classes_array[$id];
	  if(preg_match('@class="active"@', $row)){
	  	$menu_list .= ' active';
		$active_tab = true;
	  }
	  $menu_list .= '">'. $row .'</li>';
 }
	
?>
<?php print $wrapper_prefix; ?>
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print $list_type_prefix; ?>
  
  <li class="home <?php print ($active_tab?'':'active'); ?>"><a href="/"<?php print ($active_tab?'':' class="active"'); ?>><div class="top-menu-item-bullet"></div>Home</a></li>
  
    <?php print $menu_list; ?>
  <?php print $list_type_suffix; ?>
<?php print $wrapper_suffix; ?>