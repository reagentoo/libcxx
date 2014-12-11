#ifndef RANGE_H
#define RANGE_H

#include <value_traits.h>

namespace xx {

/* ===== */
/* Range */
template <uint _index, uint _size, bool _reverse = false>
class Range
{
    using R = Range<(_index + 1), (_size - 1), false>;

public:
    template <uint... _is>
    using Expand = typename R
    ::template Expand<_is..., _index>;
};

/* Range constraint */
template <uint _index, uint _size>
class Range<_index, _size, true>
{
    using R = Range<(_index + 1), (_size - 1), true>;

public:
    template <uint... _is>
    using Expand = typename R
    ::template Expand<_index, _is...>;
};

/* Range constraint */
template <uint _index>
class Range<_index, 0, false>
{
public:
    template <uint... _is>
    using Expand = UInt<_is...>;
};

/* Range constraint */
template <uint _index>
class Range<_index, 0, true>
{
public:
    template <uint... _is>
    using Expand = UInt<_is...>;
};

/* Range::Expand alias */
template <uint _index, uint _size, bool _reverse = false>
using RangeExpand = typename Range<_index, _size, _reverse>
::template Expand<>;

}

#endif // RANGE_H
