<?php
// $Id: callbacks.inc,v 1.1.2.6 2011/01/04 12:14:05 fago Exp $

/**
 * @file
 * Provides various callbacks for the whole core module integration.
 */

/**
 * Callback for getting properties of an entity.
 */
function entity_metadata_entity_get_properties($entity, array $options, $name, $entity_type) {
  if ($name == 'url') {
    $return = entity_uri($entity_type, $entity);
    return url($return['path'], $return['options'] + $options);
  }
}

/**
 * Callback for getting book node properties.
 * @see entity_metadata_book_entity_info_alter()
 */
function entity_metadata_book_get_properties($node, array $options, $name, $entity_type) {
  if (!isset($node->book['bid'])) {
    throw new EntityMetadataWrapperException('This node is no book page.');
  }
  return $node->book['bid'];
}

/**
 * Callback for getting comment properties.
 * @see entity_metadata_comment_entity_info_alter()
 */
function entity_metadata_comment_get_properties($comment, array $options, $name) {
  switch ($name) {
    case 'name':
      return $name = ($comment->uid == 0) ? variable_get('anonymous', t('Anonymous')) : $comment->name;

    case 'mail':
      if ($comment->uid != 0) {
        $account = user_load($comment->uid);
        return $account->mail;
      }
      return $comment->mail;

    case 'edit_url':
      return url('comment/edit/' . $comment->cid, $options);

    case 'node':
      return node_load($comment->nid);

    case 'parent':
      if (!empty($comment->pid)) {
        return $comment->pid;
      }
      throw new EntityMetadataWrapperException('This comment has no parent comment.');

    case 'author':
      return $comment->uid;
  }
}

/**
 * Callback for setting comment properties.
 * @see entity_metadata_comment_entity_info_alter()
 */
function entity_metadata_comment_setter($comment, $name, $value) {
  switch ($name) {
    case 'node':
      $comment->nid = $value;
      // Also set the bundle name.
      $node = node_load($value);
      $comment->node_type = 'comment_node_' . $node->type;
      break;

    case 'author':
      return $comment->uid = $value;
  }
}

/**
 * Callback for getting comment related node properties.
 * @see entity_metadata_comment_entity_info_alter()
 */
function entity_metadata_comment_get_node_properties($node, array $options, $name, $entity_type) {
  switch ($name) {
    case 'comment_count':
      return isset($node->comment_count) ? $node->comment_count : 0;

    case 'comment_count_new':
      return comment_num_new($node->nid);
  }
}

/**
 * Callback for getting node properties.
 * @see entity_metadata_node_entity_info_alter()
 */
function entity_metadata_node_get_properties($node, array $options, $name, $entity_type) {
  switch ($name) {
    case 'is_new':
      return empty($node->nid) || !empty($node->is_new);

    case 'source':
      if (!empty($node->tnid) && $source = node_load($node->tnid)) {
        return $source;
      }
      return NULL;

    case 'edit_url':
      return url('node/' . $node->nid . '/edit', $options);

    case 'author':
      return $node->uid;
  }
}

/**
 * Callback for setting node properties.
 * @see entity_metadata_node_entity_info_alter()
 */
function entity_metadata_node_set_properties($node, $name, $value) {
  if ($name == 'author') {
    $node->uid = $value;
  }
}

/**
 * Callback for determing access for node revision related properties.
 */
function entity_metadata_node_revision_access($op, $name, $entity = NULL, $account = NULL) {
  return $op == 'view' ? user_access('view revisions', $account) : user_access('administer nodes', $account);
}

/**
 * Callback for getting poll properties.
 * @see entity_metadata_poll_entity_info_alter()
 */
function entity_metadata_poll_node_get_properties($node, array $options, $name) {
  $total_votes = $highest_votes = 0;
  foreach ($node->choice as $choice) {
    if ($choice['chvotes'] > $highest_votes) {
      $winner = $choice;
      $highest_votes = $choice['chvotes'];
    }
    $total_votes = $total_votes + $choice['chvotes'];
  }

  if ($name == 'poll_duration') {
    return $node->runtime;
  }
  elseif ($name == 'poll_votes') {
    return $total_votes;
  }
  elseif (!isset($winner)) {
    throw new EntityMetadataWrapperException('There is no poll winner yet.');
  }
  switch ($name) {
    case 'poll_winner_votes':
        return $winner['chvotes'];

    case 'poll_winner':
        return $winner['chtext'];

    case 'poll_winner_percent':
        return ($winner['chvotes'] / $total_votes) * 100;
  }
}

/**
 * Callback for getting statistics properties.
 * @see entity_metadata_statistics_entity_info_alter()
 */
function entity_metadata_statistics_node_get_properties($node, array $options, $name) {
  $statistics = statistics_get($node->nid);

  switch ($name) {
    case 'views':
      return $statistics['totalviews'];

    case 'day_views':
      return $statistics['dayviews'];

    case 'last_view':
      return $statistics['timestamp'];
  }
}

/**
 * Callback for getting site-wide properties.
 * @see entity_metadata_system_entity_info_alter()
 */
function entity_metadata_system_get_properties($data = FALSE, array $options, $name) {
  switch ($name) {
    case 'name':
      return variable_get('site_name', 'Drupal');

    case 'url':
      return url('<front>', $options);

    case 'login_url':
      return url('user', $options);

    case 'current_user':
      return $GLOBALS['user']->uid;

    case 'current_date':
      return time();

    default:
      return variable_get('site_' . $name, '');
  }
}

/**
 * Callback for getting file properties.
 * @see entity_metadata_system_entity_info_alter()
 */
function entity_metadata_system_get_file_properties($file, array $options, $name) {
  switch ($name) {
    case 'name':
      return $file->filename;

    case 'mime':
      return $file->filemime;

    case 'size':
      return $file->filesize;

    case 'url':
      return url(file_create_url($file->uri), $options);

    case 'owner':
      return $file->uid;
  }
}

/**
 * Callback for getting term properties.
 * @see entity_metadata_taxonomy_entity_info_alter()
 */
function entity_metadata_taxonomy_term_get_properties($term, array $options, $name) {
  switch ($name) {
    case 'node_count':
      return count(taxonomy_select_nodes($term->tid));

    case 'description':
      return check_markup($term->description, isset($term->format) ? $term->format : NULL, '', TRUE);

    case 'vocabulary':
      return $term->vid;

    case 'parent':
      if (isset($term->parent[0]) && !is_array(isset($term->parent[0]))) {
        return $term->parent;
      }
      return array_keys(taxonomy_get_parents($term->tid));
  }
}

/**
 * Callback for setting term properties.
 * @see entity_metadata_taxonomy_entity_info_alter()
 */
function entity_metadata_taxonomy_term_setter($term, $name, $value) {
  switch ($name) {
    case 'vocabulary':
      return $term->vid = $value;
    case 'parent':
      return $term->parent = $value;
  }
}

/**
 * Callback for getting vocabulary properties.
 * @see entity_metadata_taxonomy_entity_info_alter()
 */
function entity_metadata_taxonomy_vocabulary_get_properties($vocabulary, array $options, $name) {
  switch ($name) {
    case 'term_count':
      $sql = "SELECT COUNT (1) FROM {taxonomy_term_data} td WHERE td.vid = :vid";
      return db_query($sql, array(':vid' => $vocabulary->vid))->fetchField();
  }
}

/**
 * Callback for getting user properties.
 * @see entity_metadata_user_entity_info_alter()
 */
function entity_metadata_user_get_properties($account, array $options, $name, $entity_type) {
  switch ($name) {
    case 'last_login':
      return empty($account->login) ? NULL : $account->login;

    case 'name':
      return empty($account->uid) ? variable_get('anonymous', t('Anonymous')) : $account->name;

    case 'url':
      if (empty($account->uid)) {
        return NULL;
      }
      $return = entity_uri('user', $account);
      return $return ? url($return['path'], $return['options'] + $options) : '';

    case 'edit_url':
      return empty($account->uid) ? NULL : url("user/$account->uid/edit", $options);

    case 'roles':
      return isset($account->roles) ? array_keys($account->roles) : array();
  }
}

/**
 * Callback for setting user properties.
 * @see entity_metadata_user_entity_info_alter()
 */
function entity_metadata_user_set_properties($account, $name, $value) {
  switch ($name) {
    case 'roles':
      $account->roles = array_intersect_key(user_roles(), array_flip($value));
      break;
  }
}


/**
 * Options list callback for user roles.
 */
function entity_metadata_user_roles() {
  return user_roles(TRUE);
}


/**
 * Callback defining an options list for language properties.
 */
function entity_metadata_language_list() {
  $list = array();
  $list[LANGUAGE_NONE] = t('Language neutral');
  foreach (language_list() as $language) {
    $list[$language->language] = $language->name;
  }
  return $list;
}

/**
 * Callback for getting field property values.
 */
function entity_metadata_field_property_get($entity, array $options, $name, $entity_type) {
  $field = field_info_field($name);
  $columns = array_keys($field['columns']);
  $langcode = field_language($entity_type, $entity, $name, isset($options['language']) ? $options['language']->language : NULL);
  $values = array();
  if (isset($entity->{$name}[$langcode])) {
    foreach ($entity->{$name}[$langcode] as $delta => $data) {
      $values[$delta] = $data[$columns[0]];
    }
  }
  // For an empty single-valued field, we have to return NULL.
  return $field['cardinality'] == 1 ? ($values ? reset($values) : NULL) : $values;
}

/**
 * Callback for setting field property values.
 */
function entity_metadata_field_property_set($entity, $name, $value, $langcode, $entity_type) {
  $field = field_info_field($name);
  $columns = array_keys($field['columns']);
  $langcode = isset($langcode) ? $langcode : LANGUAGE_NONE;
  $values = $field['cardinality'] == 1 ? array($value) : $value;

  $items = array();
  foreach ($values as $delta => $value) {
    $items[$delta][$columns[0]] = $value;
  }
  $entity->{$name}[$langcode] = $items;
}

/**
 * Callback returning the options list of a field.
 */
function entity_metadata_field_options_list($name, $info) {
  if (is_numeric($name) && isset($info['parent'])) {
    // The options list is to be returned for a single item of a multiple field.
    $field_property_info = $info['parent']->info();
    $name = $field_property_info['name'];
  }
  if ($field = field_info_field($name)) {
    return (array) module_invoke($field['module'], 'options_list', $field);
  }
}

/**
 * Callback to verbatim get the data structure of a field. Useful for fields
 * that add metadata for their own data structure.
 */
function entity_metadata_field_verbatim_get($entity, array $options, $name, $entity_type, &$context) {
  // Set contextual info useful for getters of any child properties.
  $entity_info = $context['parent']->info();
  $context['instance'] = field_info_instance($entity_info['type'], $name, $entity_info['bundle']);
  $context['field'] = field_info_field($name);

  $langcode = field_language($entity_type, $entity, $name, isset($options['language']) ? $options['language']->language : NULL);
  if ($context['field']['cardinality'] == 1) {
    return isset($entity->{$name}[$langcode][0]) ? $entity->{$name}[$langcode][0] : NULL;
  }
  return isset($entity->{$name}[$langcode]) ? $entity->{$name}[$langcode] : array();
}

/**
 * Writes the passed field items in the object. Useful as field level setter
 * to set the whole data structure at once.
 */
function entity_metadata_field_verbatim_set($entity, $name, $items, $langcode) {
  $field = field_info_field($name);
  $langcode = isset($langcode) ? $langcode : LANGUAGE_NONE;
  $entity->{$name}[$langcode] = $field['cardinality'] == 1 ? array($items) : $items;
}

/**
 * Callback for getting the sanitized text of 'text_formatted' properties.
 * This callback is used for both the 'value' and the 'summary'.
 */
function entity_metadata_field_text_get($item, array $options, $name, $type, $context) {
  // $name is either 'value' or 'summary'.
  if (!isset($item['safe_' . $name])) {
    // Apply input formats.
    $langcode = isset($options['language']) ? $options['language']->language : '';
    $format = isset($item['format']) ? $item['format'] : filter_default_format();
    $item['safe_' . $name] = check_markup($item[$name], $format, $langcode);
    // To speed up subsequent calls, update $item with the 'safe_value'.
    $context['parent']->set($item);
  }
  return $item['safe_' . $name];
}

/**
 * Defines the list of formats available for the current user.
 */
function entity_metadata_field_text_formats() {
  foreach (filter_formats($GLOBALS['user']) as $key => $format) {
    $formats[$key] = $format->name;
  }
  return $formats;
}

/**
 * Callback for getting the file entity of file fields.
 */
function entity_metadata_field_file_get($item) {
  return $item['fid'];
}

/**
 * Callback for setting the file entity of file fields.
 */
function entity_metadata_field_file_set(&$item, $property_name, $value) {
  $item['fid'] = $value;
}

/**
 * Callback for auto-creating file field $items.
 */
function entity_metadata_field_file_create_item($property_name, $context) {
  // 'fid' is required, so 'file' has to be set as initial property.
  return array('display' => $context['field']['settings']['display_default']);
}

/**
 * Callback for validating file field $items.
 */
function entity_metadata_field_file_validate_item($item, $context) {
  // File-field items require a valid file.
  if (!isset($item['fid']) || !file_load($item['fid'])) {
    return FALSE;
  }
  if (isset($context['property info']['display']) && !isset($item['display'])) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Access callback for the node entity.
 *
 * This function does not implement hook_node_access(), thus it may not be
 * called entity_metadata_node_access().
 */
function entity_metadata_no_hook_node_access($op, $node = NULL, $account = NULL) {
  if (isset($node)) {
    return node_access($op, $node, $account);
  }
  // Is access to all nodes allowed?
  if (!user_access('access content', $account)) {
    return FALSE;
  }
  if (user_access('bypass node access', $account) || (!isset($account) && $op == 'view' && node_access_view_all_nodes())) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Access callback for the user entity.
 */
function entity_metadata_user_access($op, $entity = NULL, $account = NULL, $entity_type) {
  $account = isset($account) ? $account : $GLOBALS['user'];
  // Grant access to the users own user account and to the anonymous one.
  if (isset($entity) && $op != 'delete' && (($entity->uid == $account->uid && $entity->uid) || (!$entity->uid && $op == 'view'))) {
    return TRUE;
  }
  if (user_access('administer users', $account) || user_access('access user profiles', $account) && $op == 'view') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Access callback for the comment entity.
 */
function entity_metadata_comment_access($op, $entity = NULL, $account = NULL) {
  if (isset($entity) && !isset($account) && comment_access($op, $entity)) {
    return TRUE;
  }
  if (user_access('administer comments', $account) || user_access('access comments', $account) && $op == 'view') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Access callback for the taxonomy entities.
 */
function entity_metadata_taxonomy_access($op, $entity = NULL, $account = NULL, $entity_type) {
  if ($entity_type == 'taxonomy_vocabulary') {
    return user_access('administer taxonomy', $account);
  }
  if (isset($entity) && $op == 'edit' && !isset($account) && taxonomy_term_edit_access($entity)) {
    return TRUE;
  }
  if (user_access('administer taxonomy', $account) || user_access('access content', $account) && $op == 'view') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Callback to determine access for properties which are fields.
 */
function entity_metadata_field_access_callback($op, $name, $entity = NULL, $account = NULL, $entity_type) {
  $field = field_info_field($name);
  return field_access($op, $field, $entity_type, $entity, $account);
}

/**
 * Callback to create entity objects.
 */
function entity_metadata_create_object($values = array(), $entity_type) {
  $info = entity_get_info($entity_type);
  // Make sure at least the bundle and label properties are set.
  if (isset($info['entity keys']['bundle']) && $key = $info['entity keys']['bundle']) {
    $values += array($key => NULL);
  }
  if (isset($info['entity keys']['label']) && $key = $info['entity keys']['label']) {
    $values += array($key => NULL);
  }
  $entity = (object) $values;
  $entity->is_new = TRUE;
  return $entity;
}

/**
 * Callback to create a new comment.
 */
function entity_metadata_create_comment($values = array()) {
  $comment = (object) ($values + array(
    'status' => COMMENT_PUBLISHED,
    'pid' => 0,
    'subject' => '',
    'uid' => 0,
    'language' => LANGUAGE_NONE,
    'node_type' => NULL,
    'is_new' => TRUE,
  ));
  $comment->cid = FALSE;
  return $comment;
}

/**
 * Callback to create a new node.
 */
function entity_metadata_create_node($values = array()) {
  $node = (object) array(
    'type' => $values['type'],
    'language' => LANGUAGE_NONE,
    'is_new' => TRUE,
  );
  // Set some defaults.
  $node_options = variable_get('node_options_' . $node->type, array('status', 'promote'));
  foreach (array('status', 'promote', 'sticky') as $key) {
    $node->$key = (int) in_array($key, $node_options);
  }
  foreach ($values as $key => $value) {
    $node->$key = $value;
  }
  return $node;
}

/**
 * Callback to save a user account.
 */
function entity_metadata_user_save(&$account) {
  $edit = (array) $account;
  // Don't save the hashed password as password.
  unset($edit['pass']);
  $account = user_save($account, $edit);
}

/**
 * Callback to delete a file.
 * Watch out to not accidentilly implement hook_file_delete().
 */
function entity_metadata_delete_file($fid) {
  file_delete(file_load($fid), TRUE);
}

/**
 * Callback to view nodes.
 */
function entity_metadata_view_node($entities, $view_mode = 'full', $langcode = NULL) {
  return node_view_multiple($entities, $view_mode, 0, $langcode);
}

/**
 * Callback to view an entity, for which just ENTITYTYPE_view() is available.
 */
function entity_metadata_view_single($entities, $view_mode = 'full', $langcode = NULL, $entity_type) {
  $function = $entity_type . '_view';
  $build = array();
  foreach ($entities as $key => $entity) {
    $build[$key] = $function($entity, $view_mode, $langcode);
  }
  return $build;
}

/**
 * Callback for querying entity properties having their values stored in the
 * entities main db table.
 */
function entity_metadata_table_query($entity_type, $property, $value, $limit) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type, '=')
        ->entityCondition($property, $value, '=')
        ->range(0, $limit);

  $result = $query->execute();
  return array_keys($result[$entity_type]);
}

/**
 * Callback for querying entities by field values. This function just queries
 * for the value of the first specified column. Also it is only suitable for
 * fields that don't process the data, so it's stored the same way as returned.
 */
function entity_metadata_field_query($entity_type, $property, $value, $limit) {
  $query = new EntityFieldQuery();
  $field = field_info_field($property);
  $columns = array_keys($field['columns']);

  $query->entityCondition('entity_type', $entity_type, '=')
        ->fieldCondition($field, $columns[0], $value, '=')
        ->range(0, $limit);

  $result = $query->execute();
  return array_keys($result[$entity_type]);
}
