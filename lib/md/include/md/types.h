#pragma once

#include "core/types.h"

namespace zeus::md
{
  /** This header houses the structures that are used to communicate order state changes to the book */
  struct order_add
  {
    core::ordid_t _order_id;
    core::quantity_t _quantity;
    core::price_t _price;
    core::order_side _side;
  };

  struct order_executed
  {
    core::ordid_t _order_id;
    core::quantity_t _shares_executed;
  };

  struct order_executed_with_price
  {
    struct order_executed _order_executed;
    core::price_t _price;
  };

  struct order_canceled
  {
    core::ordid_t _order_id;
    core::quantity_t _shares_cancelled;
  };

  struct order_removed
  {
    core::ordid_t _order_id;
  };

  struct order_replaced
  {
    core::ordid_t _original_order_id;
    core::ordid_t _new_order_id;
    core::quantity_t _quantity;
    core::price_t _price;
  };
}