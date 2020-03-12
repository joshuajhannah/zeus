#include <gtest/gtest.h>
#include "md/book.h"

using namespace zeus;

TEST(MD_BOOK, bid_ask_spread)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  /* Check the bid */
  {
    auto const& [price, quantity] = book.best_bid();
    EXPECT_EQ(price, core::price_t{2});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }
}

TEST(MD_BOOK, partial_execution)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  md::order_executed order_execute{
    ._order_id = 2,
    ._shares_executed = core::quantity_t{50}
  };
  book.execute(order_execute);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }
}

TEST(MD_BOOK, execute_with_price)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  md::order_executed order_execute{
    ._order_id = 2,
    ._shares_executed = core::quantity_t{50}
  };
  md::order_executed_with_price order_executed_with_price{
    ._order_executed = order_execute,
    ._price = core::price_t{4}
  };
  book.execute_with_price(order_executed_with_price);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }
}

TEST(MD_BOOK, top_level_wipe)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  md::order_add order_sell_behind{
    ._order_id = 3,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{4},
    ._side = core::order_side::SELL
  };
  book.add(order_sell_behind);

  md::order_executed order_execute{
    ._order_id = 2,
    ._shares_executed = core::quantity_t{50}
  };
  book.execute(order_execute);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }

  /* Fully wipe the level */
  book.execute(order_execute);

  /* Check that the level has fallen to the order behind */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{4});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }
}

TEST(MD_BOOK, behind_level_wipe)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  md::order_add order_sell_behind{
    ._order_id = 3,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{4},
    ._side = core::order_side::SELL
  };
  book.add(order_sell_behind);

  md::order_executed order_execute{
    ._order_id = 2,
    ._shares_executed = core::quantity_t{50}
  };
  book.execute(order_execute);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }

  md::order_executed order_execute_behind{
    ._order_id = 3,
    ._shares_executed = core::quantity_t{100}
  };
  book.execute(order_execute_behind);

  /* Check that the level has fallen to the order behind */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }
}

TEST(MD_BOOK, tighten_spread)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{4},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{4});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }

  md::order_add order_sell_front{
    ._order_id = 3,
    ._quantity = core::quantity_t{50},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell_front);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }
}

TEST(MD_BOOK, empty_level)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  /* Check the ask */
  {
    auto const& [_, quantity] = book.best_ask();
    EXPECT_EQ(quantity, core::quantity_t{0});
  }

  /* Check the bid */
  {
    auto const& [_, quantity] = book.best_bid();
    EXPECT_EQ(quantity, core::quantity_t{0});
  }

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  /* Check the ask */
  {
    auto const& [_, quantity] = book.best_ask();
    EXPECT_EQ(quantity, core::quantity_t{0});
  }

  /* Check the bid */
  {
    auto const& [price, quantity] = book.best_bid();
    EXPECT_EQ(price, core::price_t{2});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }

  /* Take out the level */
  md::order_executed order_execute{
    ._order_id = 1,
    ._shares_executed = 100
  };
  book.execute(order_execute);

  /* Check the ask */
  {
    auto const& [_, quantity] = book.best_ask();
    EXPECT_EQ(quantity, core::quantity_t{0});
  }

  /* Check the bid */
  {
    auto const& [_, quantity] = book.best_bid();
    EXPECT_EQ(quantity, core::quantity_t{0});
  }
}

TEST(MD_BOOK, replace_order)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy);

  md::order_add order_sell{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{3},
    ._side = core::order_side::SELL
  };
  book.add(order_sell);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{3});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }

  md::order_replaced order_replace{
    ._original_order_id = 2,
    ._new_order_id = 3,
    ._quantity = 25,
    ._price = core::price_t{4}
  };
  book.replace(order_replace);

  /* Check the ask */
  {
    auto const& [price, quantity] = book.best_ask();
    EXPECT_EQ(price, core::price_t{4});
    EXPECT_EQ(quantity, core::quantity_t{25});
  }
}

TEST(MD_BOOK, partial_then_remove)
{
  core::price_t tick_size{1};
  md::book book{tick_size};

  md::order_add order_buy_front{
    ._order_id = 1,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{2},
    ._side = core::order_side::BUY
  };
  book.add(order_buy_front);

  md::order_add order_buy_back{
    ._order_id = 2,
    ._quantity = core::quantity_t{100},
    ._price = core::price_t{1},
    ._side = core::order_side::BUY
  };
  book.add(order_buy_back);

  /* Check the bid */
  {
    auto const& [price, quantity] = book.best_bid();
    EXPECT_EQ(price, core::price_t{2});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }

  /* Partially execute */
  md::order_executed order_executed{
    ._order_id = 1,
    ._shares_executed = 50
  };
  book.execute(order_executed);

  /* Check the bid */
  {
    auto const& [price, quantity] = book.best_bid();
    EXPECT_EQ(price, core::price_t{2});
    EXPECT_EQ(quantity, core::quantity_t{50});
  }

  /* Remove the order */
  md::order_removed order_removed{
    ._order_id = 1
  };
  book.remove(order_removed);

  /* Check the bid */
  {
    auto const& [price, quantity] = book.best_bid();
    EXPECT_EQ(price, core::price_t{1});
    EXPECT_EQ(quantity, core::quantity_t{100});
  }
}