<?php

/**
 * @file
 *
 */

/**
 * Implements hook_token_values(). (token.module)().
 */
function uc_recurring_token_values($type, $object = NULL) {
  $values = array();
  switch ($type) {
    case 'recurring_fee':
      $fee = $object;

      $values['recurring-fee-id'] = $fee->rfid;
      $values['next-charge'] = format_date($fee->next_charge);
      $values['fee-amount'] = $fee->fee_amount;
      $values['fee-title'] = $fee->fee_title;
      $values['charged-intervals'] = $fee->charged_intervals;
      $values['remaining-intervals'] = $fee->remaining_intervals < 0 ? t('Until cancelled') : $fee->remaining_intervals;
      $values['renewal-attempts'] = $fee->attempts;
      $values['recurring-link'] = url('user/' . $fee->uid . '/recurring-fees', array('absolute' => TRUE));
      break;
  }

  return $values;
}

/**
 * Implements hook_token_list(). (token.module)().
 */
function uc_recurring_token_list($type = 'all') {
  $tokens = array();
  if ($type == 'recurring_fee' || $type == 'ubercart' || $type == 'all') {
    $tokens['recurring_fee']['recurring-fee-id'] = t('The recurring fee ID.');
    $tokens['recurring_fee']['next-charge'] = t('The date and time when the next charge is due to be processed.');
    $tokens['recurring_fee']['fee-amount'] = t('The recurring fee due on the next charge.');
    $tokens['recurring_fee']['fee-title'] = t('The product title used as orders for this recurring fee.');
    $tokens['recurring_fee']['charged-intervals'] = t('The number of recurring intervals due left in subscription.');
    $tokens['recurring_fee']['remaining-intervals'] = t('The number of recurring intervals due left in subscription.');
    $tokens['recurring_fee']['renewal-attempts'] = t('The number of attempts to try and renew this fee. ');
  }

  return $tokens;
}


