#include <string>
#include <iostream>

#define ENABLE_PLAIN_WRAPPERS
#define ZERO_VALUE

#include <backend.h>
#include <global.h>
#include <same.h>
#include <tuple.h>
#include <type_traits.h>
#include <value_traits.h>
#include <whole.h>

using namespace xx;

struct Foo {};
struct Bar : Foo {};

using S = SameType<Bar, Bar>;

struct MoveTest
{
    MoveTest() {}
    MoveTest(MoveTest&)  { std::cout << "copy" << std::endl; }
    MoveTest(MoveTest&&) { std::cout << "move" << std::endl; int i = move(i); }
};

class WholeTest
{
public:
    WholeTest()
    { std::cout << "W ctor" << std::endl; }
    ~WholeTest()
    { std::cout << "W dtor" << std::endl; }

    WholeTest(const WholeTest&)
    { std::cout << "W ctor copy" << std::endl; }
    WholeTest(WholeTest&&) noexcept
    { std::cout << "W ctor move" << std::endl; }

    bool operator ==(const WholeTest&) const
    {
        std::cout << "W eq" << std::endl;
        return true;
    }

    WholeTest& operator =(const WholeTest&)
    {
        std::cout << "W copy" << std::endl;
        return *this;
    }

    WholeTest& operator =(WholeTest&&)
    {
        std::cout << "W move" << std::endl;
        return *this;
    }
};

class WholeTestDeleted
{
public:
    WholeTestDeleted() = delete;
    ~WholeTestDeleted() = delete;

    WholeTestDeleted(const WholeTestDeleted&) = delete;
    WholeTestDeleted(WholeTestDeleted&&) noexcept = delete;

    bool operator ==(const WholeTestDeleted&) const = delete;
    WholeTestDeleted& operator =(const WholeTestDeleted&) = delete;
};

class WholeTestWrong
{
public:
    WholeTestWrong(int) {}

    int operator ==(const WholeTestWrong&) const { return 1; }
    bool operator ==(const int&) const { return true; }

    int operator =(const WholeTestWrong&) { return 1; }
    WholeTestWrong& operator =(const int&) { return *this; }
};

void whole_test()
{
    std::cout << "Whole scalar test:" << std::endl;
    Whole<int> i;
    Whole<int> ii(i);

    std::cout << "WholeTest:" << std::endl;
    Whole<WholeTest> w;
    Whole<WholeTest> w1(w);
    Whole<WholeTest> w2(std::move(w));

    std::cout << "WholeTestDeleted:" << std::endl;
    Whole<WholeTestDeleted> d;
    Whole<WholeTestDeleted> d1(d);
    Whole<WholeTestDeleted> d2(std::move(d));

    std::cout << "WholeTestWrong:" << std::endl;
    Whole<WholeTestWrong> wr;
    Whole<WholeTestWrong> wr1(wr);
    Whole<WholeTestWrong> wr2(std::move(wr));

    std::cout << "All dtors:" << std::endl;
}

int main(int argc, char *argv[])
{
    unused(argc, argv);

    //whole_test();
    try { whole_test(); } catch (...) {}

    MoveTest mt(mt);
    MoveTest mt2(xx::move(mt));
    unused(mt2);

    int i;
    char c;
    std::string s;
    double d;

    Struct<int, char, std::string, double> t1;
    Struct<int, char, std::string, double> t2(t1);
    Struct<int, char, std::string, std::string> t22;
    Struct<int, char, std::string> t222;
    Struct<double, std::string, char, int> t3;
    Struct<std::string, char, int, double> t4;
    Struct<char, int, std::string, double> t44;
    Struct<char, int, std::string, double> t5(c, i, s, d);

    Union<int, char, std::string, double> u1;

    t1.get(t2);
    t1.set(t2);
    t1.get(i, c, s, d);
    t1.set(1, 2, "str", 3.0);

    t2 = t1;

    t1.merge(UInt<4>(), Bool<false>(), t2);
    t1.merge(UInt<2>(), Bool<false>(), t2);
    t1.merge(UInt<4>(), Bool<true>(), t3);
    t1.merge(UInt<3>(), Bool<true>(), t4);

    t1.merge(UInt<2>(), Bool<false>(), t2);
    t1.merge(UInt<3>(), Bool<false>(), t2);
    t1.merge(UInt<3>(), Bool<false>(), t22);
    t1.merge(UInt<3>(), Bool<false>(), t222);

    t1.merge(UInt<3>(), Bool<true>(), t4);
    t1.merge(UInt<2>(), Bool<true>(), t44);

    s = t1.value<2>();
    t1.get<2>(s);
    t1.set<2>("str");

    t1.get(UInt<0,1,2,3>(), t2);
    t1.get(UInt<0,1,2,3>(), t3.value<3>(), t3.value<2>(), t3.value<1>(), t3.value<0>());
    t1.set(UInt<0,1,2,3>(), t2);
    t1.set(UInt<0,1,2,3>(), t2.value<0>(), t2.value<1>(), t2.value<2>(), t2.value<3>());

    auto f = [] (int&, char&, std::string&, double&)
    {
        return 1;
    };

    t1.invoke(UInt<0,1,2,3>(), f);
    t1.invokeRange(UInt<0,4>(), Bool<false>(), f);
    t1.invoke(f);

    t2.set(11, 22, "ssttrr", 33.0);
    const Struct<int, char, std::string, double> t11(t2);

    auto ff = [] (const int& ii, const char& cc, const std::string& ss, const double& dd)
    {
        std::cout << ii << std::endl
                  << (int)cc << std::endl
                  << ss << std::endl
                  << dd << std::endl;
        return;
    };

    t11.invoke(UInt<0,1,2,3>(), ff);
    t11.invokeRange(UInt<0,4>(), Bool<false>(), ff);
    t11.invoke(ff);

    std::string ss = t11.invoke(UInt<0,1,2,3>(), [] (const int&, const char&, const std::string& s, const double&) { return s; });

    std::cout << "t1 size: " << sizeof(t1) << std::endl;

    return 0;
}
