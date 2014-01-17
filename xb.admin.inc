<?php

/**
 * @file
 *  Administrative forms and so on..
 */


/**
 * Form builder for associating Exceptional Behaviors to arbitrary page elements.
 */
function xb_assoc($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'xb') . '/xb.admin.css';
  $behaviors = xb_get_items();
  
  // There seems to be an incompatibility with using query parameters in your
  // form while also using #ajax; the query parameters are lost. As a
  // work-around, we're storing the query string 'destination' in
  // $form_state['storage'] so we can persist that value as long as we need in
  // the form_state.
  if (!isset($form_state['storage']['hub_name'])) {
    $form_state['storage']['hub_name'] = isset($_GET['destination']) ? '_' . $_GET['destination'] : '';
  }
  
  $hub_name = __FUNCTION__ . $form_state['storage']['hub_name'];
  
  $hub = hub_load($hub_name);
  
  // I would like to make it possible to have multiple behaviors applied, so in
  // mean time we can set up the structure to expect $delta keys in "each"
  // behavior.
  $delta = $form_state['storage']['behavior_delta'] = 0;
  
  
  $form[$delta]['behavior'] = array(
    '#type' => 'select',
    '#title' => t('Behavior'),
    '#options' => _xb_behaviors_form_options(),
    '#ajax' => array(
      'callback' => '_xb_assoc_ajax_callback',
      'wrapper' => 'xb-assoc-behavior-' . $delta,
      'effect' => 'fade',
    ),
  );

  $form[$delta]['data'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="xb-assoc-behavior-' . $delta .'">',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('behavior-data-container'),
    ),
  );


  // Process behavior configuration form if the behavior has been selected. In
  // most cases, this will render dynamically via #ajax. Since we are also
  // using Hub to export these values, we want to pseudo-populate the
  // form_state value the #ajax is expecting, that way the form state we
  // configured is displayed as soon as we revisit this page.
  $form_state['values'][$delta]['behavior'] = isset($form_state['values'][$delta]['behavior']) ? $form_state['values'][$delta]['behavior'] : (isset($hub[$delta]['behavior']) ? $hub[$delta]['behavior'] : NULL);

  if (!empty($form_state['values'][$delta]['behavior']) || !empty($hub[$delta]['behavior'])) {
    $behavior = $behaviors[$form_state['values'][$delta]['behavior']];

    $form[$delta]['data']['description'] = array(
      '#markup' => $behavior['description'],
      '#prefix' => '<div class="behavior-data-description">',
      '#suffix' => '</div>',
    );

    if (isset($behavior['selectors']) && $behavior['selectors']) {
      $form[$delta]['data']['selectors'] = array(
        '#type' => 'fieldset',
        '#title' => t('Selectors'),
        '#attributes' => array(
          'class' => array('behavior-data', 'selectors'),
        ),
      );

      foreach ($behavior['selectors'] as $name => $description) {
        $form[$delta]['data']['selectors'][$name] = array(
          '#type' => 'textfield',
          '#title' => $name,
          '#description' => $description,
          '#size' => 30,
        );
      }
    }

    if (isset($behavior['config']) && is_array($behavior['config'])) {
      $form[$delta]['data']['config'] = array(
        '#type' => 'fieldset',
        '#title' => t('Configurations'),
        '#attributes' => array(
          'class' => array('behavior-data', 'config'),
        ),
      );

      foreach ($behavior['config'] as $name => $description) {
        $form[$delta]['data']['config'][$name] = array(
          '#type' => 'textfield',
          '#title' => $name,
          '#description' => $description,
          '#size' => 30,
        );
      }
    }
    
    if (isset($behavior['attached'])  && is_array($behavior['attached'])) {
      $form[$delta]['attached'] = array(
        '#type' => 'value',
        '#value' => $behavior['attached'],
      );
    }
  }
  
  return hub_closure($hub_name, $form);
}


/**
 * Ajax callback for xb_assoc().
 * @see xb_assoc().
 */
function _xb_assoc_ajax_callback($form, $form_state) {
  return $form[$form_state['storage']['behavior_delta']]['data'];
}


/**
 * Custom #options array builder.
 * @see xb_assoc().
 */
function _xb_behaviors_form_options() {
  $options = array(NULL => '-- Select Behavior --');
  foreach (xb_get_items() as $name => $info) {
    $options[$name] = $info['title'];
  }
  return $options;
}
