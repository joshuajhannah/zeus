#pragma once

namespace zeus::md::itch
{
  using stock_t = char[8];

  enum class message_type : uint8_t
  {
    SYSTEM_EVENT_MESSAGE = 'S',
    STOCK_DIRECTORY_MESSAGE = 'R',
    STOCK_TRADING_ACTION_MESSAGE = 'H',
    REG_SHO_INDICATOR_MESSAGE = 'Y',
    MARKET_PARTICIPANT_POSITION_MESSAGE = 'L',
    MWCB_DECLINE_LEVEL_MESSAGE = 'V',
    MWCB_STATUS_MESSAGE = 'W',
    IPO_QUOTING_PERIOD_MESSAGE = 'K',
    LULD_AUCTION_COLLAR_MESSAGE = 'J',
    OPERATIONAL_HALT_MESSAGE = 'h',
    ADD_ORDER_NO_MPID_MESSAGE = 'A',
    ADD_ORDER_WITH_MPID_MESSAGE = 'F',
    ORDER_EXECUTED_MESSAGE = 'E',
    ORDER_EXECUTED_WITH_PRICE = 'C',
    ORDER_CANCEL_MESSAGE = 'X',
    ORDER_DELETE_MESSAGE = 'D',
    ORDER_REPLACE_MESSAGE = 'U',
    TRADE_MESSAGE = 'P',
    CROSS_TRADE_MESSAGE = 'Q',
    BROKEN_TRADE_MESSAGE = 'B',
    NET_ORDER_IMBALANCE_INDICATOR_MESSAGE = 'I'
  };

  class timestamp
  {
  public:
    operator uint64_t()
    {
      return static_cast<uint64_t>(lower) << 32 | static_cast<uint64_t>(upper) << 48;
    }

  public:
    uint16_t upper;
    uint32_t lower;
  } __attribute__((packed));

  struct message_header
  {
    message_type _type;
    uint16_t _stock_locate;
    uint16_t _tracking_number;
    timestamp _timestamp;
  } __attribute__((packed));

  struct system_event_message
  {
    enum struct event_code : uint8_t
    {
      START_OF_MESSAGES = 'O',
      START_OF_SYSTEM_HOURS = 'S',
      START_OF_MARKET_HOURS = 'Q',
      END_OF_MARKET_HOURS = 'M',
      END_OF_SYSTEM_HOURS = 'E',
      END_OF_MESSAGES = 'C'
    };

    message_header _header;
    event_code _event_code;
  } __attribute__((packed));

  static_assert(sizeof(system_event_message) == 12);

  struct stock_directory_message
  {
    using issue_subtype_t = uint8_t[2];

    message_header _header;
    stock_t _stock;
    uint8_t _market_category;
    uint8_t _financial_status_indicator;
    uint32_t _round_lot_size;
    uint8_t _round_lots_indicator;
    uint8_t _issue_structification;
    issue_subtype_t _issue_subtype;
    uint8_t _authenticity;
    uint8_t _short_sale_threshold_indicator;
    uint8_t _ipo_flag;
    uint8_t _luld_reference_price_tier;
    uint8_t _etp_flag;
    uint32_t _etp_leverage_factor;
    uint8_t _inverse_indicator;
  } __attribute__((packed));

  static_assert(sizeof(stock_directory_message) == 39);

  struct stock_trading_action_message
  {
    using reason_t = char[4];
    
    message_header _header;
    stock_t _stock;
    uint8_t _trading_state;
    uint8_t _reserved;
    reason_t _reason;
  } __attribute__((packed));

  static_assert(sizeof(stock_trading_action_message) == 25);

  struct reg_sho_indicator_message
  {
    message_header _header;
    stock_t _stock;
    uint8_t _reg_sho_action;
  } __attribute__((packed));

  static_assert(sizeof(reg_sho_indicator_message) == 20);

  struct market_participant_position_message
  {
    using mpid_t = char[4];
    
    message_header _header;
    mpid_t _mpid;
    stock_t _stock;
    uint8_t _primary_market_maker;
    uint8_t _market_maker_mode;
    uint8_t _market_participant_state;
  } __attribute__((packed));

  static_assert(sizeof(market_participant_position_message) == 26);

  struct mwcb_decline_level_message
  {
    message_header _header;
    math::fixed<8>::underlying_t _level_one;
    math::fixed<8>::underlying_t _level_two;
    math::fixed<8>::underlying_t _level_three;
  } __attribute__((packed));

  static_assert(sizeof(mwcb_decline_level_message) == 35);

  struct mwcb_status_message
  {
    message_header _header;
    uint8_t _breached_level;
  } __attribute__((packed));

  static_assert(sizeof(mwcb_status_message) == 12);

  struct ipo_quoting_period_update_message
  {
    message_header _header;
    stock_t _stock;
    uint32_t _ipo_quoting_release_time;
    uint8_t _ipo_quotation_release_qualifier;
    math::fixed<4, int32_t>::underlying_t _ipo_price;
  } __attribute__((packed));

  static_assert(sizeof(ipo_quoting_period_update_message) == 28);

  struct luld_auction_collar_message
  {
    message_header _header;
    stock_t _stock;
    math::fixed<4, int32_t>::underlying_t _auction_collar_reference_price;
    math::fixed<4, int32_t>::underlying_t _upper_auction_collar_reference_price;
    math::fixed<4, int32_t>::underlying_t _lower_auction_collar_reference_price;
    uint32_t _auction_collar_extension;
  } __attribute__((packed));

  static_assert(sizeof(luld_auction_collar_message) == 35);

  struct operational_halt_message
  {
    message_header _header;
    stock_t _stock;
    uint8_t _market_code;
    uint8_t _operational_halt_action;
  } __attribute__((packed));

  static_assert(sizeof(operational_halt_message) == 21);

  struct add_order_no_mpid_message
  {
    message_header _header;
    uint64_t _order_reference_number;
    uint8_t _buy_sell_indicator;
    uint32_t _shares;
    stock_t _stock;
    math::fixed<4, int32_t>::underlying_t _price;
  } __attribute__((packed));

  static_assert(sizeof(add_order_no_mpid_message) == 36);

  struct add_order_with_mpid_message
  {
    using attribution_t = char[4];
    
    add_order_no_mpid_message _add_order;
    attribution_t _attribution;
  } __attribute__((packed));

  static_assert(sizeof(add_order_with_mpid_message) == 40);

  struct order_executed_message
  {
    message_header _header;
    uint64_t _order_reference_number;
    uint32_t _executed_shares;
    uint64_t _match_number;
  } __attribute__((packed));

  static_assert(sizeof(order_executed_message) == 31);

  struct order_executed_with_price_message
  {
    order_executed_message _order_executed_message;
    uint8_t _printable;
    math::fixed<4, int32_t>::underlying_t _price;
  } __attribute__((packed));

  static_assert(sizeof(order_executed_with_price_message) == 36);

  struct order_cancel_message
  {
    message_header _header;
    uint64_t _order_reference_number;
    uint32_t _cancelled_shares;
  } __attribute__((packed));

  static_assert(sizeof(order_cancel_message) == 23);

  struct order_delete_message
  {
    message_header _header;
    uint64_t _order_reference_number;
  } __attribute__((packed));

  static_assert(sizeof(order_delete_message) == 19);

  struct order_replace_message
  {
    message_header _header;
    uint64_t _original_order_reference_number;
    uint64_t _new_order_reference_number;
    uint32_t _shares;
    math::fixed<4, int32_t>::underlying_t _price;
  } __attribute__((packed));

  static_assert(sizeof(order_replace_message) == 35);

  struct trade_message
  {
    message_header _header;
    uint64_t _original_order_reference_number;
    uint8_t _buy_sell_indicator;
    uint32_t _shares;
    stock_t _stock;
    math::fixed<4, int32_t>::underlying_t _price;
    uint64_t _match_number;
  } __attribute__((packed));

  static_assert(sizeof(trade_message) == 44);

  struct cross_trade_message
  {
    message_header _header;
    uint64_t _shares;
    stock_t _stock;
    math::fixed<4, int32_t>::underlying_t _cross_price;
    uint64_t _match_number;
    uint8_t _cross_type;
  } __attribute__((packed));

  static_assert(sizeof(cross_trade_message) == 40);

  struct broken_trade_message
  {
    message_header _header;
    uint64_t _match_number;
  } __attribute__((packed));

  static_assert(sizeof(broken_trade_message) == 19);

  struct net_order_imbalance_indicator_message
  {
    message_header _header;
    uint64_t _paired_shares;
    uint64_t _imbalance_shares;
    uint8_t _imbalance_direction;
    stock_t _stock;
    math::fixed<4, int32_t>::underlying_t _far_price;
    math::fixed<4, int32_t>::underlying_t _near_price;
    math::fixed<4, int32_t>::underlying_t _current_reference_price;
    uint8_t _cross_type;
    uint8_t _price_variation_indicator;
  } __attribute__((packed));

  static_assert(sizeof(net_order_imbalance_indicator_message) == 50);

  struct retail_price_improvement_indicator_message
  {
    message_header _header;
    stock_t _stock;
    uint8_t _interest_flag;
  };

  static_assert(sizeof(retail_price_improvement_indicator_message) == 20);
}