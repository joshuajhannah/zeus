#include "md/book.h"
#include "md/itch/types.h"

namespace zeus::md
{
  /***/
  book::book(core::price_t tick_size) : _tick_size(tick_size.underlying())
  {
    /* We want an empty sell price to be represented by the numerical max index/price */
    _book[1].first = std::numeric_limits<size_t>::max();
  }

  /***/
  void book::add(order_add const &order)
  {
    /* TODO(jhannah): Is this better than a branch? Should we load by value here, then again by reference _only_ in
     *   the case that we need to actually move the spread prices? */
    auto&[top_index, levels] = _book[math::signof(static_cast<int64_t>(order._side))];

    /* Calculate our index into this half of the book */
    const std::size_t ticks_in_price = order._price.underlying() / _tick_size;
    md::level &level = levels[ticks_in_price % _max_levels];

    /* Add this order information to the map */
    _order_level_mapping.try_emplace(order._order_id, &level, order._quantity);

    /* Add the order */
    level.add_order(order._quantity);

    /* Update the spread information */
    top_index = order._side == core::order_side::BUY ? std::max(top_index, ticks_in_price)
                                                     : std::min(top_index, ticks_in_price);
  }

  /***/
  void book::cancel(order_canceled const &order)
  {
    /* Find the order in the level and remove it */
    md::order_info& info = _order_level_mapping[order._order_id];
    utility::zassert(info._level != nullptr, "Level is nullptr.");

    /* Remove the order */
    info._level->cancel_order(order._shares_cancelled);

    /* We need to adjust the working quantity of this order, or we will double-account when it gets filled/removed */
    info._qty -= order._shares_cancelled;

    /* Check if the top of book has changed */
    _resolve_book_side(_level_to_side(*info._level), info);
  }

  /***/
  void book::remove(order_removed const &order)
  {
    /* Find the order in the level and remove it */
    md::order_info const &info = _order_level_mapping[order._order_id];
    utility::zassert(info._level != nullptr, "Level is nullptr.");

    /* Remove the order */
    info._level->remove_order(info._qty);

    /* Check if the top of book has changed */
    _resolve_book_side(_level_to_side(*info._level), info);
  }

  void book::replace(order_replaced const &order)
  {
    md::order_info const &info = _order_level_mapping[order._original_order_id];
    utility::zassert(info._level != nullptr, "Level is nullptr.");

    core::order_side side = _level_to_side(*info._level);

    /* Add the new order */
    order_add order_add{._order_id = order._new_order_id, ._quantity = order._quantity, ._price = order._price, ._side = side};
    add(order_add);

    /* Remove the order */
    order_removed order_remove{._order_id = order._original_order_id};
    remove(order_remove);
  }

  /***/
  void book::execute(order_executed const &order)
  {
    /* Find the order in the level and remove it */
    md::order_info &info = _order_level_mapping[order._order_id];
    utility::zassert(info._level != nullptr, "Level is nullptr.");

    /* Execute the order */
    info._level->execute_order(order._shares_executed);

    /* We need to adjust the working quantity of this order, or we will double-account when it gets filled/removed */
    info._qty -= order._shares_executed;

    /* We may have executed the total quantity. Check if the spread has moved. */
    _resolve_book_side(_level_to_side(*info._level), info);
  }

  /***/
  void book::execute_with_price(order_executed_with_price const &order)
  {
    /* Right now, we do not care about the executed price, we just need to change the level where the order lives */
    execute(order._order_executed);
  }

  /***/
  std::pair<core::price_t, core::quantity_t> book::best_bid() const
  {
    auto&[top_index, levels] = _book[0];
    if (__unlikely(top_index == std::numeric_limits<size_t>::min()))
    {
      return {core::invalid_price, core::quantity_t{0}};
    }

    return {core::price_t::from_underlying(top_index * _tick_size.denominator()),
            core::quantity_t{levels[top_index % _max_levels].quantity()}};
  }

  /***/
  std::pair<core::price_t, core::quantity_t> book::best_ask() const
  {
    auto&[top_index, levels] = _book[1];
    if (__unlikely(top_index == std::numeric_limits<size_t>::max()))
    {
      return {core::invalid_price, core::quantity_t{0}};
    }

    return {core::price_t::from_underlying(top_index * _tick_size.denominator()),
            core::quantity_t{levels[top_index % _max_levels].quantity()}};
  }

  /** If quantity is executed or removed, we need to check if the spread price has moved */
  void book::_resolve_book_side(core::order_side side, order_info const &info)
  {
    auto side_multiplier = static_cast<int64_t>(side);
    auto&[top_index, levels] = _book[math::signof(side_multiplier)];

    /* TODO(jhannah): Curious how much impact the short-circuit branching would have here? */
    bool empty_level = info._level->quantity() == 0;
    bool top_of_book = info._level == &levels[top_index];
    if (empty_level & top_of_book)
    {
      /* In a liquid market, it's very likely that there will be volume in the level behind */
      std::size_t next_index = (top_index -= side_multiplier) % _max_levels;
      if (__likely(levels[next_index].quantity() != 0))
      {
        return;
      }

      /* There's no liquidity in the level behind. Perform a linear search to look for it. */
      for (std::size_t count = 1; count < _max_levels; ++count)
      {
        std::size_t running_index = (top_index -= side_multiplier) % _max_levels;
        if (levels[running_index].quantity() != 0)
        {
          return;
        }
      }

      /* The book is empty on this side */
      top_index = side == core::order_side::BUY ? std::numeric_limits<std::size_t>::min()
                                                : std::numeric_limits<std::size_t>::max();
    }
  }

  /***/
  core::order_side book::_level_to_side(md::level const &level) const
  {
    /* This is a hacky way of converting a level to a side, but it beats storing it in a map/level/order_info */
    bool is_sell_side = std::addressof(level) >= std::addressof(_book[1].second.front());
    return is_sell_side ? core::order_side::SELL : core::order_side::BUY;
  }

}