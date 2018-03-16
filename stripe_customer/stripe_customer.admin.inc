<?php

/**
 * @file
 * Admin view for Stripe Customers.
 */

/**
 * Menu callback for Admin View Customer form.
 */
function stripe_customer_customer_view_form($form, $form_state, $customer_id) {
  $customer = stripe_get_customer($customer_id);

  if (!$customer) {
    return array(
      'error' => array(
        '#type' => 'markup',
        '#markup' => t('There was an error retrieving this customer.'),
      ),
    );
  }
  drupal_set_title(t('Viewing customer @id', array('@id' => $customer_id)));
  $livemode = empty($customer->livemode) ? t('Test') : t('Live');
  $form = array();

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => 'Description',
    '#value' => $customer->description,
    '#description' => t('<a target="_blank" href="@link">View customer in Stripe Dashboard</a>',
      array('@link' => 'https://dashboard.stripe.com/' . strtolower($livemode) . '/customers/' . $customer_id)),
  );

  $form['created'] = array(
    '#type' => 'textfield',
    '#title' => 'Customer created',
    '#value' => format_date($customer->created, 'short'),
    '#disabled' => TRUE,
  );

  $form['balance'] = array(
    '#type' => 'textfield',
    '#title' => 'Account balance',
    '#value' => $customer->account_balance,
  );

  $form['business_vat_id'] = array(
    '#type' => 'textfield',
    '#title' => 'Business VAT ID',
    '#value' => $customer->business_vat_id,
  );

  $form['delinquent'] = array(
    '#type' => 'markup',
    '#title' => 'Delinquent?',
    '#value' => empty($customer->delinquent) ? t('No') : t('Yes'),
  );

  $form['discount'] = array(
    '#type' => 'markup',
    '#title' => 'Delinquent?',
    '#value' => empty($customer->discount) ? t('None') : $customer->discount,
  );

  $form['livemode'] = array(
    '#type' => 'markup',
    '#title' => 'Live or Test mode?',
    '#value' => empty($customer->livemode) ? t('Test') : t('Live'),
  );

  $form['default_source'] = array(
    '#type' => 'textfield',
    '#title' => 'Default source',
    '#value' => !empty($customer->default_source) ? $customer->default_source : 'None',
    '#disabled' => TRUE,
  );

  $form['currency'] = array(
    '#type' => 'textfield',
    '#title' => 'Currency',
    '#value' => $customer->currency,
  );

  $form['metadata'] = array(
    '#type' => 'fieldset',
    '#title' => 'Metadata',
  );

  if (!empty($customer->metadata)) {
    foreach ($customer->metadata->__toArray() as $property => $value) {
      $form['metadata'][$property] = array(
        '#type' => 'textfield',
        '#title' => filter_xss($property),
        '#value' => filter_xss($value),
      );
    }
  }
  else {
    $form['metadata']['none'] = array(
      '#type' => 'textfield',
      '#value' => 'None',
    );
  }

  if (!empty($customer->subscriptions->total_count)) {
    $form['subscriptions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Subscriptions (@count)', array('@count' => $customer->subscriptions->total_count)),
    );

    foreach ($customer->subscriptions->data as $plan) {
      $form['subscriptions'][$plan->id] = array(
        '#type' => 'markup',
        // @todo: Change the color of the title based on active/canceled status.
        '#markup' => '<strong>' . $plan->plan->name . ' </strong><br/>Renews every ' . $plan->plan->interval_count .
        ' ' . $plan->plan->interval . '  (' . strtoupper($plan->status) . ')<br/>' .
        t('Subscribed on @date', array('@date' => format_date($plan->current_period_start))) .
        '<br />' . t('Next renewal: @next', array('@next' => format_date($plan->current_period_end))) .
        '<br/>' . t('<a href="@link">View in Stripe Dashboard</a>', array(
          '@link' => 'https://dashboard.stripe.com/' . strtolower($livemode) . '/subscriptions/' . $plan->id,
        )),
      );
    }
  }
  return $form;
}
