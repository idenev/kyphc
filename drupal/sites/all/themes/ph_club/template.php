<?php


function ph_club_menu_tree__user_menu($variables) {
  return '<div id="user_menu_left"></div><ul class="menu">' . $variables['tree'] . '</ul><div id="user_menu_right"></div>';
}

function ph_club_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = t('Search'); // Change the text on the label element
    $form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
    $form['search_block_form']['#size'] = 20;  // define size of the textfield
    $form['search_block_form']['#value'] = t('Search'); // Set a default value for the textfield
    $form['actions']['submit']['#value'] = t('GO!'); // Change the text on the submit button
    $form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/images/mag_glass.png');

// Add extra attributes to the text box
    $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
    $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";
  }
  if($form_id == 'user_login_block'){
	/*
	dsm($form);*/
	$form['links'] = array('#markup' => '<div id="login_link" style="display: none;"><a href="/user/login">login</a></div><div id="register_link" class="item-list" style="display: none;"><span thmr="thmr_5"><a href="/user/register" title="Create a new user account.">register</a></span></div>');
	
  }
} 



function ph_club_preprocess_html(&$vars) {
    // Create our meta variable for additional meta tags in the header
    $vars['meta'] = '';

    // SEO optimization, add in the node's teaser, or if on the homepage, the site slogan as a
    // description of the page that appears in search engines
    $vars['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
    // Grab the site name while we're at it
    $vars['site_name'] = filter_xss_admin(variable_get('site_name', ''));


    // Set the charset to UTF-8
    $vars['meta'] .= '<meta charset="UTF-8">' . "\n";

    // Meta description goodness. Still used by Google to give page descriptions in search results
    if ($vars['is_front'] != '') {
        $vars['meta'] .= '<meta name="description" content="' . ph_club_trim_text($vars['site_slogan']) . '" />' . "\n";
    } else if (isset($vars['node']->teaser) && $vars['node']->teaser != '') {
        $vars['meta'] .= '<meta name="description" content="' . ph_club_trim_text($vars['node']->teaser) . '" />' . "\n";
    } else if (isset($vars['node']->body) && $vars['node']->body != '') {
        $vars['meta'] .= '<meta name="description" content="' . ph_club_trim_text($vars['node']->body) . '" />' . "\n";
    }

    // Set the author. This could be changed to also look for a node author and change it accordingly
    $vars['meta'] .= '<meta name="author" content="' . $vars['site_name'] . '">' . "\n";

    // SEO optimization, if the node has tags, use these as keywords for the page
    if (isset($vars['node']->taxonomy)) {
        $keywords = array();
        foreach ($vars['node']->taxonomy as $term) {
            $keywords[] = $term->name;
        }
        $vars['meta'] .= '<meta name="keywords" content="' . implode(',', $keywords) . '" />' . "\n";
    }

    // SEO optimization, avoid duplicate titles in search indexes for pager pages
    if (isset($_GET['page']) || isset($_GET['sort'])) {
        $vars['meta'] .= '<meta name="robots" content="noindex,follow" />' . "\n";
    }

    // Add in the optimized mobile viewport: j.mp/bplateviewport
    $vars['meta'] .= '<meta name="viewport" content="width=device-width, initial-scale=1.0" />' . "\n";

    // Optional method to display an Apple touch icon. Disabled by default.
    // $vars['meta'] .= '<link rel="apple-touch-icon" href="/apple-touch-icon.png">';
    //
    // We're adding these additional javascripts to the header because they need to be loaded before
    // the page and I don't know of a better way to do it. Anyone is welcome to apply a better approach.
    //
    // Let's assign our path instead of calling the function over and over.
    $path_prefix = path_to_theme();

    // Pull in the touch icons and show some Apple love
    $vars['meta'] .= '<link rel="apple-touch-icon" href="/' . $path_prefix . '/apple-touch-icon.png">' . "\n";

    // Plant the modernizr js. We're planting it this way because we want to still have Drupal optimize
    // pages by keeping the script loading before the closing <body> tag.
    $vars['meta'] .= '<script src="/' . $path_prefix . '/js/libs/modernizr-1.7.min.js"></script>' . "\n";

    $vars['belatedpng'] = '<!--[if lt IE 7 ]>' . "\n" . '<script src="/' . $path_prefix . '/js/libs/dd_belatedpng.js"></script>' . "\n" . '<script> DD_belatedPNG.fix(\'img, .png_bg\'); </script>' . "\n" . '<![endif]-->';
}

function ph_club_preprocess(&$variables, $hook) {
	if($hook == 'block'){
		
	}
}

function ph_club_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
	$delimiter = '<span class="breadcrumb-delimiter"> » </span>';
	$title = drupal_get_title();
    $output .= '<div class="breadcrumb">' . implode($delimiter, $breadcrumb);
	if($title != "Search"){
		$output .= $delimiter.$title;
	}
	$output .= '</div>';
    return $output;
  }else{
	  $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
	  $output .= '<div class="breadcrumb">Home</div>';
	  return $output;
  }
}


function ph_club_preprocess_node(&$vars) {
  $vars['datetime'] = format_date($vars['created'], 'custom', 'c');
  if (variable_get('node_submitted_' . $vars['node']->type, TRUE)) {
    $vars['submitted'] = t('Submitted by !username on !datetime',
      array(
        '!username' => $vars['name'],
        '!datetime' => '<time datetime="' . $vars['datetime'] . '" pubdate="pubdate">' . $vars['date'] . '</time>',
      )
    );
  }
  else {
    $vars['submitted'] = '';
  }
  $vars['unpublished'] = '';
  if (!$vars['status']) {
    $vars['unpublished'] = '<div class="unpublished">' . t('Unpublished') . '</div>';
  }
}

function ph_club_trim_text($text, $length = 150) {
    // remove any HTML or line breaks so these don't appear in the text
    $text = trim(str_replace(array("\n", "\r"), ' ', strip_tags($text)));
    $text = trim(substr($text, 0, $length));
    $lastchar = substr($text, -1, 1);
    // check to see if the last character in the title is a non-alphanumeric character, except for ? or !
    // if it is strip it off so you don't get strange looking titles
    if (preg_match('/[^0-9A-Za-z\!\?]/', $lastchar)) {
        $text = substr($text, 0, -1);
    }
    // ? and ! are ok to end a title with since they make sense
    if ($lastchar != '!' && $lastchar != '?') {
        $text .= '...';
    }
    return $text;
}