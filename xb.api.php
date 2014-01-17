<?php

/**
 * @file
 *  API Documentation and hook examples.
 */


/**
 * Implements hook_xb().
 */
function hook_xb() {
  return array(
    // Unique machine-readable name for the exceptional behavior.
    'alert' => array(
      
      // Provide a human-readable title for the exceptional behavior.
      'title' => t('Alert'),
      
      // Provide an in-depth description of what this behavior does. This is
      // displayed in the configuration page.
      'description' => t('Grab the text within each element of this class and output it as an alert box.'),
      
      // Define the selectors that will be used in your behavior. These aren't
      // necessarily the name of the selectors themselves, but it is the
      // property name that you will refer to in your javascript. Each item key
      // in 'selectors' must be a qualifying identifier, and its value is
      // simply the in-depth description that will be displayed in the
      // configuration form.
      'selectors' => array(
        'target' => t('This class should be applied to the parent element you want to extract text from.'),
        'test' => t('This is a test field for a selector.'),
      ),
      
      // Define the settings that will be needed in your behaviors. These will
      // be available in your javascript via Drupal.settings. Most typical use
      // for this is something like this:
      // 
      // @code
      //  $(this).someCoolPlugin(Drupal.settings.xb.alert.config);
      // @endcode
      // 
      // Same rules apply with valid javascript property names for the keys,
      // and a description of what the option does as its value.
      'config' => array(
        'width' => t('This is the width.'),
        'height' => t('This is the height.'),
      ),
      
      // This works exactly like Drupal's #attached property in renderable arrays.
      // @see drupal_process_attached().
      'attached' => array(
        'js' => array(
          drupal_get_path('module', 'xb') . '/alert.js',
        ),
      ),
    ),
  );
}
