#pragma once

#include <array>
#include <utility>
#include <unordered_map>

#include "core/order.h"
#include "math/fastmod.h"
#include "md/level.h"
#include "md/types.h"

/** A limit order book. As a performance experiment, it has a maximum of 64 levels.
 *
 *  This book is only performant for liquid instruments. Sparse books will suffer may penalty when the spread moves. */
namespace zeus::md
{
  /* Since the book is currently stateless (L2), we do not need any more order information. */
  struct order_info
  {
    order_info() = default;

    order_info(md::level *level, core::quantity_t qty) : _level(level), _qty(qty)
    {
    }

    md::level *_level;
    core::quantity_t _qty;
  };

  class book
  {
  private:
    /* We will represent the bid/ask side of a book separately */
    static constexpr std::size_t _max_levels{64};
    using book_side_t = std::pair<std::size_t, std::array<md::level, _max_levels>>;

  public:
    book() = default;

    ~book() = default;

    /***/
    book(core::price_t tick_size);

    /**
     * Add an order into the LOB
     *
     * @param order The order to add to the book
     */
    void add(order_add const &order);

    /**
     * Cancel an order in the LOB
     *
     * @param order The order to cancel from the book
     */
    void cancel(order_canceled const &order);

    /**
     * Remove an order from the LOB
     *
     * @param order The order to cancel from the book
     */
    void remove(order_removed const &order);

    /**
     * Amend one order to another in the LOB. This implementation is not thread safe.
     *
     * @param order The order that has been amended
     */
    void replace(order_replaced const &order);

    /**
     * Execute an order in the LOB
     *
     * @param order The order that has been executed
     */
    void execute(order_executed const &order);

    /**
     * Execute an order in the LOB at a specific price
     */
    void execute_with_price(order_executed_with_price const &order);

    /**
     * Get the best bid price/qty
     */
    std::pair<core::price_t, core::quantity_t> best_bid() const;

    /**
     * Get the best bid price/qty
     */
    std::pair<core::price_t, core::quantity_t> best_ask() const;

  private:
    /** If quantity is executed or removed, we need to check if the spread price has moved */
    void _resolve_book_side(core::order_side side, order_info const &info);

    /** Determine which side a level belongs to */
    core::order_side _level_to_side(md::level const &level) const;

  private:
    /** \brief Both side of the book glued together in an array */
    std::array<book_side_t, 2> _book;

    /** \brief To use the price as an index into the book, we store the tick size optimised for modulo
     *
     *  TODO(jhannah): Need to support variable tick sizes.. Annoying, but I have a plan. */
    math::lemire_fastmod _tick_size{std::numeric_limits<int64_t>::max()};

    /** \brief A mapping from client order ID to the level it belongs to.
     *  TODO(jhannah): Write your own hash map, optimised for this use case.
     *  TODO(jhannah): For the sake of performance, we do not delete from the map. Is this scalable? */
    std::unordered_map<core::clordid_t, md::order_info> _order_level_mapping{};
  };
}