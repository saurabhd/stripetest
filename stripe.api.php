<?php

/**
 * @file
 * Describe hooks provided by the Stripe module.
 */

/**
 * Handle incoming Stripe webhook.
 *
 * Stripe sends requests to a URL defined in the Stripe webhooks manager.
 * When a single request is received from Stripe, this hook is invoked.
 *
 * You can reference https://stripe.com/docs/webhooks to learn more
 * about webhooks.
 *
 * Event types and descriptions are listed here: https://stripe.com/docs/api#event_types
 *
 * @param object $event
 *   An object of event data sent in from Stripe.
 */
function hook_stripe_webhook($event) {
  switch ($event->type) {
    case 'invoice.payment_succeeded':
      // You could send a custom invoice paid email here.
      break;

    case 'charge.refunded':
      // You could send an email with details about the refund here.
      break;
  }
}

/**
 * Add metadata to a StripeObject.
 *
 * Modules implementing this hook should inspect $object to make sure
 *   it is of the correct type (to avoid adding metadata to a Customer
 *   instead of a Subscription Plan, for example).
 *
 * @param string $type
 *   The type of object being saved or created.
 *
 * @param array $values
 *   Key-value pairs of metadata values to add to our object.
 *
 * @return $metadata
 */
function hook_stripe_metadata($type, $values) {
  $metadata = array();
  if ($type == 'customer') {
    $metadata['Customer Field'] = $values['custom_field1'];
  }
  return $metadata;
}
