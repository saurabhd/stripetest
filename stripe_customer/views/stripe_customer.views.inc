<?php

/**
 * @file
 * Provides a relationship between Drupal user and Stripe customers.
 *
 * @todo: Make sure we can relate to {commerce_order.uid} etc.
 */

/**
 * Implements hook_views_data().
 */
function stripe_customer_views_data() {

  $data['stripe_customers']['table']['group']  = t('Stripe Customers');
  $data['stripe_customers']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  $data['stripe_customers']['customer_id'] = array(
    'title' => t('Customer ID'),
    'help' => t('A customer ID belonging to the user.'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Customer ID'),
      'title' => t('Relate to the Customer on Stripe'),
      'help' => t('Associate a Drupal user with a Customer on Stripe.'),
    ),
    'field' => array(
      'handler' => 'stripe_customer_viewshandler',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['stripe_customers']['status'] = array(
    'title' => t('Customer Status'),
    'help' => t('FALSE indicates a customer has been deleted'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['stripe_customers']['created'] = array(
    'title' => t('Customer created date'),
    'help' => t('The date the customer was created'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['stripe_customers']['changed'] = array(
    'title' => t('Customer updated date'),
    'help' => t('The date the customer was updated'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['stripe_customers']['created'] = array(
    'title' => t('Customer Created Date'),
    'help' => t('The date the customer was created'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}
