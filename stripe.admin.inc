<?php

/**
 * @file
 * Stripe administration and module settings UI.
 */

/**
 * Menu callback: configure Stripe API Keys.
 */
function stripe_admin_keys() {
  $secret = variable_get('stripe_secret');
  stripe_load_library();

  if (!empty($secret) && substr($secret, 0, 1) != 'p') {
    try {
      $info = \Stripe\Account::retrieve();

      // Currencies supported by Stripe will vary depending
      // on which country the Stripe Account was created in.
      $country_spec = \Stripe\CountrySpec::retrieve($info->country);
      $supported_currencies = $country_spec->supported_payment_currencies;

      $account_information = "<label>" . t('Business name:') . "</label> " . $info->business_name;
      $account_information .= "<br /><label>" . t('Business E-mail:') . "</label> " . $info->email;
      $account_information .= "<br /><label>" . t('Statement Descriptor:') . "</label> " . $info->statement_descriptor;
      $account_information .= "<br /><label>" . t('Details Submitted:') . "</label> " . $info->details_submitted;
      $account_information .= "<br /><label>" . t('Charges Enabled:') . "</label> " . $info->charges_enabled;
//      $account_information .= "<br /><label>" . t('Transfers Enabled:') .  "</label> " . $info->transfers_enabled;
      $account_information .= "<br /><label>" . t('Currencies Supported:') . "</label> " . implode(", ", $supported_currencies);
      $account_information .= "<br /><label>" . t('Default currency:') . "</label> " . $info->default_currency;
    }
    catch (Exception $e) {
      _stripe_error(t('Could not authenticate to Stripe. :msg', array(':msg' => $e->getMessage())));
      $account_information = t("Error connecting to Stripe.");
    }
  }
  else {
    $account_information = t("Not connected to Stripe.");
  }

  $form['stripe_account_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Information'),
    '#collapsible' => FALSE,
  );
  $form['stripe_account_information']['info'] = array(
    '#markup' => $account_information,
  );
  $form['api_keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Keys'),
    '#collapsible' => FALSE,
  );
  $form['api_keys']['stripe_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#size' => 35,
    '#default_value' => variable_get('stripe_secret', ''),
    '#required' => TRUE,
  );
  $form['api_keys']['stripe_publishable'] = array(
    '#type' => 'textfield',
    '#title' => t('Publishable key'),
    '#size' => 35,
    '#default_value' => variable_get('stripe_publishable', ''),
    '#required' => TRUE,
  );

  $form['api_keys']['stripe_api_version'] = array(
    '#type' => 'select',
    '#title' => t('Stripe API Version'),
    '#options' => array(
      STRIPE_API_LATEST_TESTED => 'Latest Tested (2017-06-05)',
      STRIPE_API_ACCOUNT_DEFAULT => 'Account Default',
      STRIPE_API_VERSION_CUSTOM => 'Custom',
    ),
    '#empty_option' => STRIPE_API_ACCOUNT_DEFAULT,
    '#empty_value' => 'Account Default',
    '#default_value' => variable_get('stripe_api_version', STRIPE_API_ACCOUNT_DEFAULT),
    '#description' => t('Specify the API version to use for requests.
    Defaults to the version configured in your <a href="@dash">Stripe Account</a>.',
      array('@dash' => 'http://dashboard.stripe.com/account/apikeys')),
  );

  $form['api_keys']['stripe_api_version_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Specify an API Version'),
    '#description' => t('Useful for testing API Versioning before committing to an upgrade.'),
    '#default_value' => variable_get('stripe_api_version_custom', ''),
    '#size' => 12,
    '#states' => array(
      'visible' => array(
        ':input[name="stripe_api_version"]' => array('value' => STRIPE_API_VERSION_CUSTOM),
      ),
    ),
  );

  $form['stripe_webhooks'] = array('#type' => 'fieldset', '#title' => t('Webhooks'));
  $form['stripe_webhooks']['stripe_log_webhooks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log Webhooks'),
    '#description' => t('This will log all valid webhook requests with their event data to Watchdog.'),
    '#default_value' => variable_get('stripe_log_webhooks', FALSE),
  );
  $form['stripe_webhooks']['stripe_log_webhooks_expire_days'] = array(
    '#type' => 'select',
    '#options' => range(1, 30, 1),
    '#title' => t('Webhook logs expiration (days)'),
    '#description' => t('The number of days before received webhook data is deleted. Max 30.'),
    '#default_value' => variable_get('stripe_log_webhooks_expire_days', 30),
    '#states' => array(
      'visible' => array(
        ':input[name="stripe_log_webhooks"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['stripe_webhooks']['stripe_webhooks_idempotent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for webhook idempotency'),
    '#description' => t('This will log basic Webhook data (id, type, and created timestamps) to ensure they are only processed once.'),
    '#default_value' => variable_get('stripe_webhooks_idempotent', FALSE),
  );

  return system_settings_form($form);
}
