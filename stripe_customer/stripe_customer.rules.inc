<?php

/**
 * @file
 * Provides Rules integration for the Stripe Customer module.
 */

/**
 * Provide data info to Rules for customer ID.
 */
function stripe_customer_rules_data_info() {
  return array(
    'customer_id' => array(
      'label' => t('Stripe customer ID'),
      'group' => t('Stripe'),
      'token type' => 'stripe-customer',
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 *
 * Provide new Rules events for reacting upon Stripe Customer operations.
 */
function stripe_customer_rules_event_info() {
  $events = array();
  $events['stripe_customer_created'] = array(
    'label' => t('A Stripe Customer was created.'),
    'group' => t('Stripe'),
    'variables' => array(
      'stripe_customer' => array(
        'label' => t('Stripe Customer'),
        'type' => 'stripe_customer',
      ),
      'account' => array(
        'label' => t('Drupal User'),
        'type' => 'user',
      ),
    ),
    'access callback' => 'stripe_customer_rules_access',
  );
  $events['stripe_customer_updated'] = array(
    'label' => t('A Stripe Customer was updated.'),
    'group' => t('Stripe'),
    'variables' => array(
      'stripe_customer' => array(
        'label' => t('Stripe Customer'),
        'type' => 'stripe_customer',
        'skip save' => TRUE,
      ),
      'account' => array(
        'label' => t('Drupal User'),
        'type' => 'user',
      ),
    ),
    'access callback' => 'stripe_customer_rules_access',
  );
  return $events;
}
