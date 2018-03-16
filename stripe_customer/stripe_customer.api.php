<?php

/**
 * @file
 * API documentation for the Stripe Customer module.
 */

/**
 * Provide new properties to save to a Customer based on a Drupal user.
 *
 * @param object $account
 *   The account of the user whose Stripe Customer was created or updated.
 * @param array $params
 *   Extra parameters not available via $account.
 *
 * @return array
 *   Properties that wil be stored on the customer object.
 */
function hook_stripe_customer_info($account, $params) {
  $properties = [];
  $properties['business_vat_id'] = $account->business_vat_id;
  $properties['metadata']['Total spent'] = $account->total_spent;

  // Extra parameters not available via user_load(), for instance
  // when creating a new Customer, we might want to include the
  // user's token to \Customer:create();
  $properties['source'] = $params['stripe_token'];
  return $properties;
}
