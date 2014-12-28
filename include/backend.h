#ifndef BACKEND_H
#define BACKEND_H

#include <type_traits>
#include <utility>

namespace xx {

template <class _X>
using ResultOf = typename std::result_of_t<_X>;

template <typename _X>
constexpr auto move(_X&& x) noexcept
{ return std::move(x); }

template <typename _X>
constexpr auto move_if_noexcept(_X&& x) noexcept
{ return std::move_if_noexcept(x); }

}

#endif // BACKEND_H
