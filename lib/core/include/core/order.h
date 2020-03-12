#pragma once

#include <cstdint>

#include "core/types.h"
#include "time/types.h"

namespace zeus::core
{
  /* We use -1/1 as sell/buy, so we can multiply by the enum to embed side into quantity */
  enum class order_side : int32_t
  {
    SELL = -1,
    INVALID = 0,
    BUY = 1
  };

  enum class order_type : int32_t
  {
    MARKET = 0,
    LIMIT,
    STOP,
    STOP_LIMIT,
    MARKET_IF_TOUCHED,
    INVALID
  };

  enum class order_tif : int32_t
  {
    DAY = 0,
    /* Good Till Cancel*/
    GTC,
    /* At the Opening */
    OPG,
    /* Immediate or Cancel */
    IOC,
    /* Fill or Kill */
    FOK,
    /* Good Till Crossing */
    GTX,
    /* Good Till Date */
    GTD,
    /* At the Close */
    ATC,
    INVALID
  };

  enum class order_status : int32_t
  {
    NEW = 0,
    PARTIALLY_FILLED,
    FILLED,
    DONE_FOR_DAY,
    CANCELED,
    REPLACED,
    PENDING_CANCEL,
    STOPPED,
    REJECTED,
    SUSPENDED,
    PENDING_NEW,
    CALCULATED,
    EXPIRED,
    ACCEPTED_FOR_BIDDING,
    PENDING_REPLACE,
    INVALID
  };

  /** A class to represent a generic order */
  /* TODO(jhannah): Optimise the data layout */
  class order {
  public:
    order() = default;
    ~order() = default;

    /** \brief The client order ID associated with this order */
    core::clordid_t _client_order_id{core::invalid_clordid};

    /** \brief The transaction time of this order */
    time::timestamp_t _transact_time{time::invalid_time};

    /** \brief Whether this order is a BUY or a SELL order */
    core::order_side _side{order_side::INVALID};

    /** \brief The time-in-force for this order */
    core::order_tif _tif{order_tif::INVALID};

    /** \brief The type of this order */
    core::order_type _type{order_type::INVALID};

    /** \brief The price of this order */
    core::price_t _price{core::invalid_price};

    /** \brief The total quantity for this order */
    core::quantity_t _total_qty{core::invalid_quantity};

    /** \brief The remaining quantity to be executed for this order */
    core::quantity_t _leaves_qty{core::invalid_quantity};

    /** \brief The last executed quantity of the order */
    core::quantity_t _last_qty;

    /** \brief The status of this order */
    core::order_status _status{order_status::INVALID};
  };
}