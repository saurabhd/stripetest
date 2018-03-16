<?php
/**
 * @file
 * Page callbacks for the stripe module.
 */

/**
 * Handler for incoming Stripe Webhook requests.
 */
function stripe_webhooks_callback() {
  if ($_SERVER['REQUEST_METHOD'] != 'POST') {
    drupal_not_found();
  }

  if (stripe_load_library()) {
    // Read in and decode the event data.
    $body = @file_get_contents('php://input');

    watchdog('stripe', 'Received a webhook');
    // @todo. See: https://stripe.com/docs/webhooks#signatures

//    // Make sure the webhook request includes a signing signature.
//    $received_signature = $_SERVER["HTTP_STRIPE_SIGNATURE"];
//    $signature_parts = explode(',', $received_signature);
//    $saved_secrets = variable_get('stripe_webhooks_secrets', '');
//
//    // If we are checking secrets, verify against our saved secret headers.
//    if (!empty($saved_secrets) && empty($received_signature)) {
//      $signatures = explode("\r\n", $saved_secrets);
//
//      if (!in_array($received_signature, $signatures)) {
//        drupal_access_denied();
//      }
//    }

    $event = json_decode($body);
    // Log event in watchdog.
    if (variable_get('stripe_log_webhooks', 0) == 1) {
      watchdog('stripe', 'Stripe Webhook %type: <pre>@event</pre>', array(
        '%type' => (is_object($event) && isset($event->type)) ? $event->type : t('<undefined>'),
        '@event' => print_r($event, TRUE),
      ), WATCHDOG_DEBUG);
    }

    // For additional security - retrieve event data from ID.
    if (is_object($event) && isset($event->id)) {
      try {
        $event = \Stripe\Event::retrieve($event->id);
      }
      catch (Exception $e) {
        watchdog('stripe_webhook', 'Could not retrieve event @id. Message: @msg', array(
          '@id' => $event->id,
          '@msg' => $e->getMessage(),
        ));
      }

      if ($event instanceof \Stripe\Event && isset($event->id)) {
        // Print our status early to let Webhooks see a response quickly.
        $processed = db_query("SELECT processed FROM {stripe_webhook_events} WHERE event_id = :id", array(':id' => $event->id))->fetchField();

        // If configured for idempotent requests, make sure
        // we have not already processed this event.
        if (variable_get('stripe_webhooks_idempotent', FALSE)) {
          if (!empty($processed)) {
            watchdog('Stripe', 'Attempted to process previously-sent webhook event :id', array(':id' => $event->id));
            drupal_add_http_header('Status', '200 OK');
            return 'OK';
          }

          // Insert our initial event prior to processing.
          db_merge('stripe_webhook_events')
            ->insertFields(array(
              'event_id' => $event->id,
              'idempotency_key' => isset($event->idempotency_key) ? $event->idempotency_key : '',
              'type' => $event->type,
              'api_version' => $event->api_version,
              'created' => $event->created,
              'received' => REQUEST_TIME,
              'livemode' => ($event->livemode === 'true' ? 1 : 0),
            ))
            ->updateFields(array(
              'received' => REQUEST_TIME,
            ))
            ->key(array('event_id' => $event->id))
            ->execute();

          // Invoke webhook events.
          module_invoke_all('stripe_webhook', $event);

          // If we successfully processed, update with current timestamp.
          db_update('stripe_webhook_events')
            ->fields(array('processed' => REQUEST_TIME))
            ->condition('event_id', $event->id)
            ->execute();
        }
        // Otherwise we just invoke the webhook response hooks.
        else {
          module_invoke_all('stripe_webhook', $event);
        }
      }
    }
    else {
      drupal_add_http_header('Status', '404');
    }
  }
  // @todo: Return an error if we failed.
  drupal_add_http_header('Status', '200');
  return 'OK';
}
