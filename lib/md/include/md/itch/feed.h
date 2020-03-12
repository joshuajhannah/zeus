#pragma once

#include "md/book.h"
#include "md/types.h"
#include "md/itch/types.h"
#include "system/utilities.h"

#include <memory>

namespace zeus::md::itch
{
  /* A very simple implementation of an ITCH feedhandler that will build a book and return whether it has updated */
  /** TODO(jhannah): Define a concept */
  template<typename Receiver>
  class feed {
  public:
    feed(std::unique_ptr<Receiver> receiver)
    : _receiver(std::move(receiver))
    {
      /* Assume here that we are only dealing with stocks listed > 1USD */
      std::fill(_books.begin(), _books.end(), core::price_t::from_underlying(math::pow(10, 6)));
    }

    /***/
    bool poll()
    {
      constexpr size_t maximum_size = 50;
      std::byte buffer[maximum_size]{};

      /* Read the header */
      _receiver.read(buffer, sizeof(message_header));
      message_header const& header = *reinterpret_cast<message_header const*>(buffer);

      /* Could be more concise with macros/templates, but I personally prefer it to be obviously laid out */
      switch(header._type)
      {
        case message_type::ORDER_CANCEL_MESSAGE:
          _handle_order_cancel_message(_read_tail<order_cancel_message>(buffer));
          return true;
        case message_type::ORDER_DELETE_MESSAGE:
          _handle_order_delete_message(_read_tail<order_delete_message>(buffer)));
          return true;
        case message_type::ORDER_EXECUTED_MESSAGE:
          _handle_order_executed_message(_read_tail<order_executed_message>(buffer));
          return true;
        case message_type::ORDER_EXECUTED_WITH_PRICE:
          _handle_order_executed_with_price_message(_read_tail<order_executed_with_price_message>(buffer));
          return true;
        case message_type::ORDER_REPLACE_MESSAGE:
          _handle_order_replace_message(_read_tail<order_replace_message>(buffer));
          return true;
        case message_type::ADD_ORDER_NO_MPID_MESSAGE:
          _handle_add_order_no_mpid_message(_read_tail<add_order_no_mpid_message>(buffer));
          return true;
        case message_type::ADD_ORDER_WITH_MPID_MESSAGE:
          _handle_add_order_with_mpid_message(_read_tail<add_order_with_mpid_message>(buffer));
          return true;
        case message_type::BROKEN_TRADE_MESSAGE:
          _read_tail<broken_trade_message>(buffer);
          return false;
        case message_type::CROSS_TRADE_MESSAGE:
          _read_tail<cross_trade_message>(buffer);
          return false;
        case message_type::IPO_QUOTING_PERIOD_MESSAGE:
          _read_tail<ipo_quoting_period_update_message>(buffer);
          return false;
        case message_type::MARKET_PARTICIPANT_POSITION_MESSAGE:
          _read_tail<market_participant_position_message>(buffer);
          return false;
        case message_type::MWCB_DECLINE_LEVEL_MESSAGE:
          _read_tail<mwcb_decline_level_message>(buffer);
          return false;
        case message_type::MWCB_STATUS_MESSAGE:
          _read_tail<mwcb_status_message>(buffer);
          return false;
        case message_type::LULD_AUCTION_COLLAR_MESSAGE:
          _read_tail<luld_auction_collar_message>(buffer);
          return false;
        case message_type::NET_ORDER_IMBALANCE_INDICATOR_MESSAGE:
          _read_tail<net_order_imbalance_indicator_message>(buffer);
          return false;
        case message_type::OPERATIONAL_HALT_MESSAGE:
          _read_tail<operational_halt_message>(buffer);
          return false;
        case message_type::REG_SHO_INDICATOR_MESSAGE:
          _read_tail<reg_sho_indicator_message>(buffer);
          return false;
        case message_type::STOCK_DIRECTORY_MESSAGE:
          _read_tail<stock_directory_message>(buffer);
          return false;
        case message_type::STOCK_TRADING_ACTION_MESSAGE:
          _read_tail<stock_trading_action_message>(buffer);
          return false;
        case message_type::SYSTEM_EVENT_MESSAGE:
          _read_tail<system_event_message>(buffer);
          return false;
        case message_type::TRADE_MESSAGE:
          _read_tail<trade_message>(buffer);
          return false;
        default:
          /* Change to a log message once the logger is finished */
          utility::zassert_ndebug(false, "Unrecognised message type.");
      }
    }

  private:

    /***/
    [[nodiscard]] void _handle_add_order_with_mpid_message(add_order_with_mpid_message const& message)
    {
      _handle_add_order_no_mpid_message(message._add_order);
    }

    /***/
    [[nodiscard]] void _handle_add_order_no_mpid_message(add_order_no_mpid_message const& message)
    {
      md::book& book = _books[message._header._stock_locate];
      md::order_add order_add{
        ._order_id = message._order_reference_number,
        ._quantity = message._shares,
        ._price = core::price_t::from_underlying(message._price),
        ._side = message._buy_sell_indicator == 'B' ? core::order_side::BUY : core::order_side::SELL
      };

      book.add(order_add);
    }

    /***/
    [[nodiscard]] void _handle_order_cancel_message(order_cancel_message const& message)
    {
      md::book& book =_books[message._header._stock_locate];
      md::order_canceled order_cancel{
        ._order_id = message._order_reference_number,
        ._shares_cancelled = message._cancelled_shares
      };

      book.cancel(order_cancel);
    }

    /***/
    [[nodiscard]] void _handle_order_delete_message(order_delete_message const& message)
    {
      md::book& book =_books[message._header._stock_locate];
      md::order_removed order_remove{
        ._order_id = message._order_reference_number
      };

      book.remove(order_remove);
    }

    /***/
    [[nodiscard]] void _handle_order_executed_message(order_executed_message const& message)
    {
      md::book& book =_books[message._header._stock_locate];
      md::order_executed order_execute{
        ._order_id = message._order_reference_number,
        ._shares_executed = message._executed_shares
      };

      book.execute(order_execute);
    }

    /***/
    [[nodiscard]] void _handle_order_executed_with_price_message(order_executed_with_price_message const& message)
    {
      md::book& book =_books[message._order_executed_message._header._stock_locate];
      md::order_executed order_execute{
        ._order_id = message._order_executed_message._order_reference_number,
        ._shares_executed = message._order_executed_message._executed_shares
      };
      md::order_executed_with_price order_execute_with_price{
        ._order_executed = order_execute,
        ._price = core::price_t::from_underlying(message._price)
      };

      book.execute_with_price(order_execute_with_price);
    };

    /***/
    [[nodiscard]] void _handle_order_replace_message(order_replace_message const& message)
    {
      md::book& book =_books[message._header._stock_locate];
      md::order_replaced order_replace{
        ._original_order_id = message._original_order_reference_number,
        ._new_order_id = message._new_order_reference_number,
        ._price = core::price_t::from_underlying(message._price),
        ._quantity = message._shares
      };

      book.replace(order_replace);
    };

    /***/
    template<typename T>
    T const& _read_tail(std::byte* buffer)
    {
      _receiver.read(buffer + sizeof(message_header), sizeof(T) - sizeof(message_header));
      return *reinterpret_cast<T const*>(buffer);
    }

  private:
    /* Explicitly use decltype to make the context of the value obvious */
    static constexpr size_t _num_books = std::numeric_limits<decltype(message_header::_stock_locate)>::max() + 1;
    std::array<md::book, _num_books> _books;

    /* This is where we will read our data stream from */
    std::unique_ptr<Receiver> _receiver;
  };
}