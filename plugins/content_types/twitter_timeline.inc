<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Twitter timeline'),
  'description' => t('Shows a twitter timeline with basic configuration options.'),
  'category' => t('Social media'),
  'edit form' => 'twitter_timeline_twitter_timeline_edit_form',
  'render callback' => 'twitter_timeline_twitter_timeline_render',
  'admin info' => 'twitter_timeline_twitter_timeline_admin_info',
  'defaults' => array(
    'username' => '',
    'widget_id' => '',
  )
);

/**
 * 'admin info' callback for panel pane.
 */
function twitter_timeline_twitter_timeline_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showing @tweets_to_show tweets from <em>@@username</em>.', array(
      '@tweets_to_show' => $conf['tweets_to_show'],
      '@username' => $conf['username'],
    ));
    return $block;
  }
}

/**
 * 'Edit form' callback for the content type.
 */
function twitter_timeline_twitter_timeline_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['username'] = array(
    '#title' => t('Twitter username'),
    '#description' => t('The username of the twitter account in which to pull the tweets from.'),
    '#type' => 'textfield',
    '#default_value' => $conf['username'],
    '#required' => TRUE,
  );

  if (!empty($conf['widget_id'])) {
    $description = t('The widget id of the widget created from !link.', array('!link' => l('widget configuration', 'https://twitter.com/settings/widgets/' . $conf['widget_id'] . '/edit')));
  } else {
    $description = t('The widget id of the widget created from !link.', array('!link' => l('widget configuration', 'https://twitter.com/settings/widgets/new')));
  }

  $form['widget_id'] = array(
    '#title' => t('Twitter widget id'),
    '#description' => $description,
    '#type' => 'textfield',
    '#default_value' => $conf['widget_id'],
    '#required' => TRUE,
  );
  /*
  $form['tweets_to_show'] = array(
    '#title' => t('Number of tweets to show'),
    '#description' => t('Used to control the number of tweets shown on the page initially. Defaults to 5.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(3, 12)),
    '#default_value' => $conf['tweets_to_show'],
    '#required' => TRUE,
  );
  */

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function twitter_timeline_twitter_timeline_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function twitter_timeline_twitter_timeline_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();

  // initial content is blank
  $block->title = '';
  $block->content = '';

  // Add in the content
  $block->content .= '
    <a class="twitter-timeline" href="https://twitter.com/' . check_plain($conf['username']) . '" data-widget-id="' . check_plain($conf['widget_id'])  . '">Tweets af @' . check_plain($conf['username']) . '</a>
    <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?\'http\':\'https\';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>';

  return $block;
}
