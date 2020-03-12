#pragma once

#include <cstdint>
#include <type_traits>
#include <limits>

#include "math/utilities.h"

namespace zeus::math
{
  template<uint8_t Precision, typename T = int64_t>
  class fixed
  {
  public:
    static_assert(std::is_integral_v<T>);
    static_assert(Precision < std::numeric_limits<T>::digits10);

  public:
    using underlying_t = T;

    fixed() = default;
    ~fixed() = default;

    /**
     * Constructs a fixed object from a given integral input
     *
     * @tparam U The type of the input. It must be convertible to T.
     * @param input The integral input to be returned in fixed representation
     */
    template<typename U>
      requires std::is_convertible_v<U, T>
    constexpr explicit fixed(U input);

    /**
     * Creates a fixed object from a given underlying value
     *
     * @param underlying The underlying value of the fixed object we're creating
     * @returns The fixed object represented by \p underlying
     */
    static constexpr fixed from_underlying(underlying_t underlying);

    /**
     * @returns The maximum representable number
     */
    static constexpr fixed max();

    /**
     * @returns The maximum representable number
     */
    static constexpr fixed min();

    /***/
    constexpr fixed operator+=(fixed rhs) noexcept;

    /***/
    constexpr fixed operator-=(fixed rhs) noexcept;

    /***/
    constexpr fixed operator*(double rhs) const noexcept;

    /***/
    constexpr fixed operator/(double rhs) const noexcept;

    /***/
    constexpr bool operator==(fixed rhs) const noexcept;

    /***/
    constexpr bool operator!=(fixed rhs) const noexcept;

    /**
     * Fetch the underlying representation of this fixed object
     *
     * @returns The underlying representation
     */
    constexpr underlying_t underlying() const noexcept;

    template<typename U>
      requires std::is_arithmetic_v<U>
    constexpr operator U() const noexcept;

  private:
    underlying_t _underlying{0};
  };

  /***/
  template<uint8_t Precision, typename T>
  template<typename U>
    requires std::is_convertible_v<U, T>
  constexpr fixed<Precision, T>::fixed(U input)
  {
    _underlying = static_cast<T>(input * math::pow(10, Precision));
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::from_underlying(underlying_t underlying)
  {
    fixed<Precision, T> ret;
    ret._underlying = underlying;
    return ret;
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::operator+=(zeus::math::fixed<Precision, T> rhs) noexcept
  {
    _underlying += rhs._underlying;
    return *this;
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::operator-=(zeus::math::fixed<Precision, T> rhs) noexcept
  {
    _underlying -= rhs._underlying;
    return *this;
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::operator*(double rhs) const noexcept
  {
    return fixed<Precision, T>::from_underlying(static_cast<T>(_underlying * rhs));
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::operator/(double rhs) const noexcept
  {
    return fixed<Precision, T>::from_underlying(static_cast<T>(_underlying / rhs));
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::max()
  {
    return fixed<Precision, T>::from_underlying(std::numeric_limits<T>::max());
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> fixed<Precision, T>::min()
  {
    return fixed<Precision, T>::from_underlying(std::numeric_limits<T>::min());
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr bool fixed<Precision, T>::operator==(fixed rhs) const noexcept
  {
    return _underlying == rhs._underlying;
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr bool fixed<Precision, T>::operator!=(fixed rhs) const noexcept
  {
    return _underlying != rhs._underlying;
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr T fixed<Precision, T>::underlying() const noexcept
  {
    return _underlying;
  }

  template<uint8_t Precision, typename T>
  template<typename U>
    requires std::is_arithmetic_v<U>
  constexpr fixed<Precision, T>::operator U() const noexcept
  {
    return static_cast<U>(_underlying) / pow(10, Precision);
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> operator-(zeus::math::fixed<Precision, T> lhs, zeus::math::fixed<Precision, T> rhs)
  {
    return lhs -= rhs;
  }

  /***/
  template<uint8_t Precision, typename T>
  constexpr fixed<Precision, T> operator+(zeus::math::fixed<Precision, T> lhs, zeus::math::fixed<Precision, T> rhs)
  {
    return lhs += rhs;
  }
}