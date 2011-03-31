<?php
// $Id: file_styles.theme.inc,v 1.1.2.2 2011/01/11 18:37:34 aaron Exp $

/**
 * @file file_styles/includes/themes/file_styles.theme.inc
 *
 * Theme and preprocess functions for the File Styles module.
 */

/**
 * Display an image according to the style presented, or raw as specified.
 */
function theme_file_styles_image($variables) {
  // @TODO: Check to see if the image is local as well, for getsize.
  if (isset($variables['image_style'])) {
    $output = theme('image_style', array('style_name' => $variables['image_style'], 'path' => $variables['image_uri'], 'alt' => $variables['alt'], 'title' => $variables['title'], 'getsize' => FALSE, 'attributes' => $variables['attributes']));
  }
  else {
    $output = theme('image', array('path' => $variables['image_uri'], 'alt' => $variables['alt'], 'title' => $variables['title'], 'getsize' => FALSE, 'attributes' => $variables['attributes']));
  }
  return $output;
}

/**
 * Preview image for Styles UI.
 */
function theme_file_styles_image_preview($variables) {
  // Media requires a file object, so we can't style without saving first.
  $sample_image = variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png');
  $variables['object'] = file_uri_to_object($sample_image);
  $variables['field_type'] = 'file';
  return theme('styles', $variables);
}
