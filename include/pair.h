#ifndef PAIR_H
#define PAIR_H

namespace xx {

/* ==== */
/* Pair */
template <class _X, class _Y, bool _union = false>
class Pair
{
    _X m_x;
    _Y m_y;

public:
    inline _X& x() { return m_x; }
    inline _Y& y() { return m_y; }

    inline const _X& x() const { return m_x; }
    inline const _Y& y() const { return m_y; }
};

/* Pair constraint */
template <class _X, class _Y>
class Pair<_X, _Y, true>
{
    union U
    {
         U() {}
        ~U() {}

        _X x;
        _Y y;
    } m_;

public:
    inline _X& x() { return m_.x; }
    inline _Y& y() { return m_.y; }

    inline const _X& x() const { return m_.x; }
    inline const _Y& y() const { return m_.y; }
};

}

#endif // PAIR_H
