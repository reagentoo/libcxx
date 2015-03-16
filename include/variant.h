#ifndef VARIANT_H
#define VARIANT_H

#include <backend.h>
#include <global.h>
#include <type_traits.h>

namespace xx {

template <class... _Xs>
class Variant
{
    using This = Variant<_Xs...>;

    template <uint _i>
    using At = MuxType<_i, _Xs...>;
    using Xs = Union<_Xs...>;

    static constexpr uint m_size = sizeof...(_Xs);

    template <uint... _is>
    struct Handlers {
        inline auto& operator [](int i)
        { return d[i]; }
        inline const auto& operator [](int i) const
        { return d[i]; }

    private:
        static const struct Data {
            void (&ctor)(Xs&);
            void (&dtor)(Xs&);

            void (&ctor_copy)(Xs&, const Xs&);
            void (&ctor_move)(Xs&, Xs&);

            bool (&eq)(const Xs&, const Xs&);
            bool (&neq)(const Xs&, const Xs&);

            void (&copy)(Xs&, const Xs&);
            void (&move)(Xs&, Xs&);
        } d[m_size];
    };

    using Hs = RangeExpand<0, m_size, false, Handlers>;

    template <uint _i>
    static void ctor(Xs& xs)
    {
        auto i = UInt<_i>();
        new (&xs[i]) At<_i>;
    }
    template <uint _i>
    static void dtor(Xs& xs)
    {
        // TODO: try to remove
        // this alias with clang 3.7
        using A = At<_i>;

        auto i = UInt<_i>();
        xs[i].~A();
    }

    template <uint _i>
    static void ctor_copy(Xs& xs, const Xs& src)
    {
        auto i = UInt<_i>();
        new (&xs[i]) At<_i> (src[i]);
    }
    template <uint _i>
    static void ctor_move(Xs& xs, Xs& src)
    {
        auto i = UInt<_i>();
        new (&xs[i]) At<_i> (xx::move(src[i]));
    }

    template <uint _i>
    static bool eq(const Xs& xs, const Xs& src)
    {
        auto i = UInt<_i>();
        return xs[i] == src[i];
    }

    template <uint _i>
    static bool neq(const Xs& xs, const Xs& src)
    {
        auto i = UInt<_i>();
        return xs[i] != src[i];
    }

    template <uint _i>
    static void copy(Xs& xs, const Xs& src)
    {
        auto i = UInt<_i>();
        xs[i] = src[i];
    }
    template <uint _i>
    static void move(Xs& xs, Xs& src)
    {
        auto i = UInt<_i>();
        xs[i] = xx::move(src[i]);
    }

public:
    inline Variant()
    {
        m_index = -1;
    }
    inline ~Variant()
    {
        if (m_index < m_size)
            m_hs[m_index].dtor(m_xs);
    }

    template <uint _i>
    inline Variant(const UInt<_i>)
    {
        m_index = _i;
        ctor<_i>(m_xs);
    }

    inline Variant(uint i)
    {
        m_index = i;
        m_hs[i].ctor(m_xs);
    }

    /* _________ */
    /* Operators */
    template <uint _i>
    inline auto& operator [](const UInt<_i> i)
    { return value(i); }
    template <uint _i>
    inline const auto& operator [](const UInt<_i> i) const
    { return value(i); }

    /* ____________________ */
    /* Simple value getters */
    template <uint _i>
    inline auto& value(const UInt<_i> i)
    { return m_xs[i]; }
    template <uint _i>
    inline const auto& value(const UInt<_i> i) const
    { return m_xs[i]; }


    inline void clear()
    {
        if (m_index < m_size) {
            m_hs[m_index].dtor(m_xs);
            m_index = -1;
        }
    }

    inline bool empty() const
    { return m_index >= m_size; }

    /* ____________________ */
    /* Base getters/setters */
    inline void get(This& t) const
    { t.set(*this); }
    inline void get(This&& t)
    { t.set(xx::move(*this)); }

    inline void set(const This& t)
    {
        uint i = t.m_index;
        unused(i);
    }
    inline void set(This&& t)
    {
        uint i = t.m_index;
        unused(i);
    }

    /* _______________ */
    /* getters/setters */
    template <uint _i>
    inline void set(const UInt<_i>, const MuxType<_i, _Xs...>&)
    { }

private:
    uint m_index;

    Hs m_hs;
    Xs m_xs;
};

template <class... _Xs>
template <uint... _is>
const typename Variant<_Xs...>
::template Handlers<_is...>::Data
Variant<_Xs...>::Handlers<_is...>::d[m_size] = {
    {
        ctor<_is>,
        dtor<_is>,

        ctor_copy<_is>,
        ctor_move<_is>,

        eq<_is>,
        neq<_is>,

        copy<_is>,
        move<_is>,
    }...
};

}

#endif // VARIANT_H
