#include <cassert>
#include <type_traits>

//#define STRONGLY_TYPED_ALIAS_DISABLE
#include "strongly_typed_alias.hpp"

namespace sta = strongly_typed_alias;

int main()
{
    STRONGLY_TYPED_ALIAS(int_alias_t, int, sta::op::everything);

    int_alias_t value = sta::construct<int_alias_t>(0);
    assert(sta::get(value++) == 0);
    assert(sta::get(value) == 1);
    assert(sta::get(++value) == 2);
    assert(sta::get(value--) == 2);
    assert(sta::get(value) == 1);
    assert(sta::get(--value) == 0);
    assert(sta::get(value = sta::construct<int_alias_t>(42)) == 42);
    assert(sta::get(value) == 42);
    assert(sta::get(+value) == 42);
    assert(sta::get(-value) == -42);
    assert(sta::get(value) == 42);
    assert(!value == false);
    assert(sta::get(~value) == -43);
    assert(sta::get(value << sta::construct<int_alias_t>(1)) == 84);
    assert(sta::get(value >> sta::construct<int_alias_t>(1)) == 21);
    assert((value > sta::construct<int_alias_t>(0)) == true);
    assert((value > sta::construct<int_alias_t>(42)) == false);
    assert((value >= sta::construct<int_alias_t>(42)) == true);
    assert((value >= sta::construct<int_alias_t>(43)) == false);
    assert((value < sta::construct<int_alias_t>(0)) == false);
    assert((value < sta::construct<int_alias_t>(42)) == false);
    assert((value <= sta::construct<int_alias_t>(42)) == true);
    assert((value <= sta::construct<int_alias_t>(43)) == true);
    assert((value == sta::construct<int_alias_t>(42)) == true);
    assert((value == sta::construct<int_alias_t>(43)) == false);
    assert((value != sta::construct<int_alias_t>(42)) == false);
    assert((value != sta::construct<int_alias_t>(43)) == true);
    assert(sta::get(value & sta::construct<int_alias_t>(3)) == 2);
    assert(sta::get(value ^ sta::construct<int_alias_t>(3)) == 41);
    assert(sta::get(value | sta::construct<int_alias_t>(1)) == 43);
    assert(static_cast<int>(value && sta::construct<int_alias_t>(0)) == 0);
    assert(static_cast<int>(value && sta::construct<int_alias_t>(1)) == 1);
    assert(static_cast<int>(value || sta::construct<int_alias_t>(0)) == 1);
    assert(static_cast<int>(value || sta::construct<int_alias_t>(1)) == 1);
    assert(static_cast<int>(sta::construct<int_alias_t>(0) || sta::construct<int_alias_t>(0)) == 0);
    assert(value);
    assert(value ? true : false);
    if (value) assert(true); else assert(false);
    assert(sta::get(value += sta::construct<int_alias_t>(100)) == 142);
    assert(sta::get(value -= sta::construct<int_alias_t>(10)) == 132);
    assert(sta::get(value *= sta::construct<int_alias_t>(2)) == 264);
    assert(sta::get(value /= sta::construct<int_alias_t>(4)) == 66);
    assert(sta::get(value %= sta::construct<int_alias_t>(30)) == 6);
    assert(sta::get(value <<= sta::construct<int_alias_t>(2)) == 24);
    assert(sta::get(value >>= sta::construct<int_alias_t>(1)) == 12);
    assert(sta::get(value &= sta::construct<int_alias_t>(7)) == 4);
    assert(sta::get(value ^= sta::construct<int_alias_t>(7)) == 3);
    assert(sta::get(value |= sta::construct<int_alias_t>(10)) == 11);

    static_assert(std::is_trivially_constructible<int_alias_t>::value, "");
    static_assert(std::is_trivially_copyable<int_alias_t>::value, "");
    static_assert(std::is_trivially_copy_assignable<int_alias_t>::value, "");
    static_assert(std::is_default_constructible<int_alias_t>::value, "");
    static_assert(std::is_move_constructible<int_alias_t>::value, "");
    static_assert(std::is_pod<int_alias_t>::value, "");
    static_assert(std::is_literal_type<int_alias_t>::value, "");
    static_assert(std::is_standard_layout<int_alias_t>::value, "");
    static_assert(std::is_trivial<int_alias_t>::value, "");

    return 0;
}
