#pragma once

namespace zeus::md
{
  /**
   * A stateless representation of a level in a LOB. It may seem a little silly now, but the interface will be useful
   * for when L3 support is added.
   *
   * TODO(jhannah): Add a structure to store the orders.
   *   - Design a solution that is copy-free from receiving from the handler to moving into the book. Only reclaim these
   *     buffers when the order is executed or canceled.
   */
  class level {
  public:
    level() = default;
    ~level() = default;

    /***/
    void add_order(core::quantity_t qty) {
      _quantity += qty;
    }

    /***/
    void remove_order(core::quantity_t const& qty) {
      _quantity -= qty;
    }

    void cancel_order(core::quantity_t const& qty)
    {
      _quantity -= qty;
    }

    /***/
    void execute_order(core::quantity_t const& qty) {
      _quantity -= qty;
    }

    /** I'm really torn whether to just expose the data member instead of a trivial getter. */
    [[nodiscard]] core::quantity_t quantity() const noexcept { return _quantity; }

  private:
    /* Here, we store the current open quantity of the level. While the design is so unsophisticated, this abstraction
     * is quite useless, but it will be handy later on. */
    core::quantity_t _quantity{0};
  };
}