#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

namespace xx {

/* ================ */
/* Base type traits */
using uchar  = unsigned char;  //  8 bit unsigned
using ushort = unsigned short; // 16 bit unsigned
using uint   = unsigned int;   // 32 bit unsigned
using ulong  = unsigned long;  // 32 bit unsigned

using longlong  = long long;          // 64 bit signed
using ulonglong = unsigned long long; // 64 bit unsigned

/* ==== */
/* Type */
struct Type {
    /* _________________ */
    /* Types enumeration */
    enum E {
        Any,

        Number,
        Signed,
        Unsigned,

        AnyChar,
        AnyShort,
        AnyInt,
        AnyLong,
        AnyLongLong,

        Boolean,

        Char,
        Short,
        Int,
        Long,
        LongLong,

        UChar,
        UShort,
        UInt,
        ULong,
        ULongLong
    };

    /* _________ */
    /* Validator */
    template <class _X, E _e = E::Any>
    struct Validator;
};

/* Type::Validator alias */
template <class _X, Type::E _e = Type::Any>
using TypeValidator = typename Type
::template Validator<_X, _e>::Type;

#define DEFINE_TYPE_INFO(_X_, _XX_) \
    template <> \
    struct Type::Validator<_X_, Type::_XX_> \
    { using Type = _X_; };

/* Validator constraints */
DEFINE_TYPE_INFO(bool,      Any)
DEFINE_TYPE_INFO(char,      Any)
DEFINE_TYPE_INFO(uchar,     Any)
DEFINE_TYPE_INFO(short,     Any)
DEFINE_TYPE_INFO(ushort,    Any)
DEFINE_TYPE_INFO(int,       Any)
DEFINE_TYPE_INFO(uint,      Any)
DEFINE_TYPE_INFO(long,      Any)
DEFINE_TYPE_INFO(ulong,     Any)
DEFINE_TYPE_INFO(longlong,  Any)
DEFINE_TYPE_INFO(ulonglong, Any)

DEFINE_TYPE_INFO(char,      Number)
DEFINE_TYPE_INFO(uchar,     Number)
DEFINE_TYPE_INFO(short,     Number)
DEFINE_TYPE_INFO(ushort,    Number)
DEFINE_TYPE_INFO(int,       Number)
DEFINE_TYPE_INFO(uint,      Number)
DEFINE_TYPE_INFO(long,      Number)
DEFINE_TYPE_INFO(ulong,     Number)
DEFINE_TYPE_INFO(longlong,  Number)
DEFINE_TYPE_INFO(ulonglong, Number)

DEFINE_TYPE_INFO(char,     Signed)
DEFINE_TYPE_INFO(short,    Signed)
DEFINE_TYPE_INFO(int,      Signed)
DEFINE_TYPE_INFO(long,     Signed)
DEFINE_TYPE_INFO(longlong, Signed)

DEFINE_TYPE_INFO(uchar,     Unsigned)
DEFINE_TYPE_INFO(ushort,    Unsigned)
DEFINE_TYPE_INFO(uint,      Unsigned)
DEFINE_TYPE_INFO(ulong,     Unsigned)
DEFINE_TYPE_INFO(ulonglong, Unsigned)

DEFINE_TYPE_INFO(char,      AnyChar)
DEFINE_TYPE_INFO(uchar,     AnyChar)
DEFINE_TYPE_INFO(short,     AnyShort)
DEFINE_TYPE_INFO(ushort,    AnyShort)
DEFINE_TYPE_INFO(int,       AnyInt)
DEFINE_TYPE_INFO(uint,      AnyInt)
DEFINE_TYPE_INFO(long,      AnyLong)
DEFINE_TYPE_INFO(ulong,     AnyLong)
DEFINE_TYPE_INFO(longlong,  AnyLongLong)
DEFINE_TYPE_INFO(ulonglong, AnyLongLong)

DEFINE_TYPE_INFO(bool, Boolean)

DEFINE_TYPE_INFO(char,     Char)
DEFINE_TYPE_INFO(short,    Short)
DEFINE_TYPE_INFO(int,      Int)
DEFINE_TYPE_INFO(long,     Long)
DEFINE_TYPE_INFO(longlong, LongLong)

DEFINE_TYPE_INFO(uchar,     UChar)
DEFINE_TYPE_INFO(ushort,    UShort)
DEFINE_TYPE_INFO(uint,      UInt)
DEFINE_TYPE_INFO(ulong,     ULong)
DEFINE_TYPE_INFO(ulonglong, ULongLong)

}

#endif // TYPE_TRAITS_H
