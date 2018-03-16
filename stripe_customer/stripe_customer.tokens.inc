<?php

/**
 * @file
 * Contains token integration.
 */

/**
 * Implements hook_token_info().
 */
function stripe_customer_token_info() {
  $info['types']['stripe_customer'] = array(
    'name' => t('Stripe'),
    'description' => t('Tokens related to Stripe.'),
  );
  $info['tokens']['stripe_customer']['customer_id'] = array(
    'name' => t('Customer ID'),
    'description' => t('The Customer Id'),
  );
  $info['tokens']['stripe_customer']['default_source'] = array(
    'name' => t('Default source'),
    'description' => t('The token of the default source for this Customer.'),
  );
  $info['tokens']['stripe_customer']['owner'] = array(
    'name' => t('Drupal account'),
    'description' => t('The Drupal User associated with this Customer.'),
    'type' => 'user',
  );
  $info['tokens']['stripe_customer']['email'] = array(
    'name' => t('Email'),
    'description' => t('The Customer email address on the account.'),
  );
  $info['tokens']['stripe_customer']['metadata'] = array(
    'name' => t('Customer Metadata'),
    'description' => t('The Metadata currently on the Customer.'),
  );
  $info['tokens']['stripe_customer']['currency'] = array(
    'name' => t('Currency'),
    'description' => t('The Customer preferred currency.'),
  );
  $info['tokens']['stripe_customer']['created'] = array(
    'name' => t('Created Date'),
    'description' => t('The date the Customer was created on Stripe.'),
    'type' => 'date',
  );
  $info['tokens']['stripe_customer']['account_balance'] = array(
    'name' => t('Account balance'),
    'description' => t('The balance on this customer.'),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function stripe_customer_tokens($type, $tokens, $data = array(), $options = array()) {
  if (isset($options['language'])) {
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  if ($type == 'stripe_customer') {
    $customer = (isset($data['stripe_customer']) && ($data['stripe_customer'] instanceof \Stripe\Customer)) ? $data['stripe_customer'] : NULL;
    $account = isset($data['account']) ? $data['account'] : NULL;

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'customer_id':
          $replacements[$original] = $customer->id;
          break;

        case 'owner':
          $replacements[$original] = $sanitize ? filter_xss($account->name) : $account->name;
          break;

        case 'email':
          $replacements[$original] = $customer->email;
          break;

        case 'metadata':
          $replacements[$original] = $customer->metadata;
          break;

        case 'currency':
          $replacements[$original] = $customer->currency;
          break;

        case 'created':
          $replacements[$original] = format_date($customer->created, 'medium', '', NULL, $language_code);
          break;

        case 'account_balance':
          $replacements[$original] = $customer->account_balance;
          break;

        case 'description':
          $replacements[$original] = $sanitize ? check_plain($customer->description) : $customer->description;
          break;

        case 'default_source':
          $replacements[$original] = $customer->default_source;
          break;
      }
    }
  }
  return $replacements;
}
