#pragma once

#if defined(_MSC_VER)
#define STRONGLY_TYPED_ALIAS_EMPTY_BASES __declspec(empty_bases)
#else
#define STRONGLY_TYPED_ALIAS_EMPTY_BASES
#endif

#if defined(STRONGLY_TYPED_ALIAS_DISABLE)

#define STRONGLY_TYPED_ALIAS(_name, _type, ...) using _name = _type

namespace strongly_typed_alias
{
template <class TAlias>
class underlying_type
{
public:
    using type = TAlias;
};

template <class TAlias>
using underlying_type_t = typename underlying_type<TAlias>::type;

template <class TAlias>
constexpr TAlias construct(TAlias value) noexcept
{
    return value;
}

template <class TAlias>
constexpr TAlias get(TAlias alias) noexcept
{
    return alias;
}

template <class T>
class enable_construct
{
public:
    template <class TAlias>
    constexpr static TAlias strongly_typed_alias_construct(TAlias value) noexcept
    {
        return value;
    }
};

template <class T>
class enable_get
{
public:
    template <class TAlias>
    constexpr static TAlias strongly_typed_alias_get(TAlias alias) noexcept
    {
        return alias;
    }
};
}

#else

namespace strongly_typed_alias
{
template <class TAlias, class TValueType, class TAliasOperator>
class alias_operator
{
protected:
    constexpr TAlias & to_alias() noexcept
    {
        return static_cast<TAlias &>(*this);
    }

    constexpr TAlias const & to_alias() const noexcept
    {
        return static_cast<TAlias const &>(*this);
    }

    constexpr static TValueType get_underlying_value(TAlias alias) noexcept
    {
        return alias.m_value;
    }

    constexpr static void set_underlying_value(TAlias & alias, TValueType value) noexcept
    {
        alias.m_value = value;
    }
};

template <class TUnique, class TValueType, template <class, class> class ... TAliasOperators>
class STRONGLY_TYPED_ALIAS_EMPTY_BASES alias : public TAliasOperators<alias<TUnique, TValueType, TAliasOperators...>, TValueType>...
{
public:
    using value_type = TValueType;

    template <class, class, class>
    friend class alias_operator;

protected:
    value_type m_value;
};

template <class TAlias>
class underlying_type
{
public:
    using type = typename TAlias::value_type;
};

template <class TAlias>
using underlying_type_t = typename underlying_type<TAlias>::type;

template <class TAlias>
constexpr TAlias construct(underlying_type_t<TAlias> value) noexcept
{
    return TAlias::do_construct(value);
}

template <class T>
class enable_construct
{
private:
    friend T;

    template <class TAlias>
    constexpr static TAlias strongly_typed_alias_construct(underlying_type_t<TAlias> value) noexcept
    {
        return TAlias::do_construct(value);
    }
};

template <class TAlias>
constexpr underlying_type_t<TAlias> get(TAlias alias) noexcept
{
    return TAlias::do_get(alias);
}

template <class T>
class enable_get
{
private:
    friend T;

    template <class TAlias>
    constexpr static underlying_type_t<TAlias> strongly_typed_alias_get(TAlias alias) noexcept
    {
        return TAlias::do_get(alias);
    }
};

namespace op
{
template <class TAlias, class TValueType>
class addition : public alias_operator<TAlias, TValueType, addition<TAlias, TValueType>>
{
public:
    constexpr TAlias operator+(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) + this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class bitwise_and : public alias_operator<TAlias, TValueType, bitwise_and<TAlias, TValueType>>
{
public:
    constexpr TAlias operator&(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) & this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class bitwise_left_shift : public alias_operator<TAlias, TValueType, bitwise_left_shift<TAlias, TValueType>>
{
public:
    constexpr TAlias operator<<(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) << this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class bitwise_not : public alias_operator<TAlias, TValueType, bitwise_not<TAlias, TValueType>>
{
public:
    constexpr TAlias operator~() const noexcept
    {
        TAlias other {};
        this->set_underlying_value(other, ~this->get_underlying_value(this->to_alias()));
        return other;
    }
};

template <class TAlias, class TValueType>
class bitwise_or : public alias_operator<TAlias, TValueType, bitwise_or<TAlias, TValueType>>
{
public:
    constexpr TAlias operator|(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) | this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class bitwise_right_shift : public alias_operator<TAlias, TValueType, bitwise_right_shift<TAlias, TValueType>>
{
public:
    constexpr TAlias operator>>(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) >> this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class bitwise_xor : public alias_operator<TAlias, TValueType, bitwise_xor<TAlias, TValueType>>
{
public:
    constexpr TAlias operator^(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) ^ this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class bool_conversion : public alias_operator<TAlias, TValueType, bool_conversion<TAlias, TValueType>>
{
public:
    constexpr explicit operator bool() const noexcept
    {
        return this->get_underlying_value(this->to_alias()) != 0;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_addition : public alias_operator<TAlias, TValueType, compound_assignment_addition<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator+=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) + this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_bitwise_and : public alias_operator<TAlias, TValueType, compound_assignment_bitwise_and<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator&=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) & this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_bitwise_left_shift : public alias_operator<TAlias, TValueType, compound_assignment_bitwise_left_shift<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator<<=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) << this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_bitwise_or : public alias_operator<TAlias, TValueType, compound_assignment_bitwise_or<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator|=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) | this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_bitwise_right_shift : public alias_operator<TAlias, TValueType, compound_assignment_bitwise_right_shift<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator>>=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) >> this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_bitwise_xor : public alias_operator<TAlias, TValueType, compound_assignment_bitwise_xor<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator^=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) ^ this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_division : public alias_operator<TAlias, TValueType, compound_assignment_division<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator/=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) / this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_multiplication : public alias_operator<TAlias, TValueType, compound_assignment_multiplication<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator*=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) * this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_remainder : public alias_operator<TAlias, TValueType, compound_assignment_remainder<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator%=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) % this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class compound_assignment_subtraction : public alias_operator<TAlias, TValueType, compound_assignment_subtraction<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator-=(TAlias other) noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(
            alias,
            this->get_underlying_value(alias) - this->get_underlying_value(other)
        );
        return alias;
    }
};

template <class TAlias, class TValueType>
class construct : public alias_operator<TAlias, TValueType, construct<TAlias, TValueType>>
{
public:
    constexpr static TAlias do_construct(TValueType value) noexcept
    {
        TAlias result {};
        construct::set_underlying_value(result, value);
        return result;
    }
};

template <class TAlias, class TValueType>
class division : public alias_operator<TAlias, TValueType, division<TAlias, TValueType>>
{
public:
    constexpr TAlias operator/(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) / this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class equal : public alias_operator<TAlias, TValueType, equal<TAlias, TValueType>>
{
public:
    constexpr bool operator==(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) == this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class get : public alias_operator<TAlias, TValueType, get<TAlias, TValueType>>
{
public:
    constexpr static TValueType do_get(TAlias value) noexcept
    {
        return get::get_underlying_value(value);
    }
};

template <class TAlias, class TValueType>
class greater : public alias_operator<TAlias, TValueType, greater<TAlias, TValueType>>
{
public:
    constexpr bool operator>(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) > this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class greater_or_equal : public alias_operator<TAlias, TValueType, greater_or_equal<TAlias, TValueType>>
{
public:
    constexpr bool operator>=(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) >= this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class less : public alias_operator<TAlias, TValueType, less<TAlias, TValueType>>
{
public:
    constexpr bool operator<(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) < this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class less_or_equal : public alias_operator<TAlias, TValueType, less_or_equal<TAlias, TValueType>>
{
public:
    constexpr bool operator<=(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) <= this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class logical_and : public alias_operator<TAlias, TValueType, logical_and<TAlias, TValueType>>
{
public:
    constexpr bool operator&&(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) && this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class logical_not : public alias_operator<TAlias, TValueType, logical_not<TAlias, TValueType>>
{
public:
    constexpr bool operator!() const noexcept
    {
        return !this->get_underlying_value(this->to_alias());
    }
};

template <class TAlias, class TValueType>
class logical_or : public alias_operator<TAlias, TValueType, logical_or<TAlias, TValueType>>
{
public:
    constexpr bool operator||(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) || this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class multiplication : public alias_operator<TAlias, TValueType, multiplication<TAlias, TValueType>>
{
public:
    constexpr TAlias operator*(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) * this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class nothing
{
};

template <class TAlias, class TValueType>
class not_equal : public alias_operator<TAlias, TValueType, not_equal<TAlias, TValueType>>
{
public:
    constexpr bool operator!=(TAlias other) const noexcept
    {
        return this->get_underlying_value(this->to_alias()) != this->get_underlying_value(other);
    }
};

template <class TAlias, class TValueType>
class postfix_decrement : public alias_operator<TAlias, TValueType, postfix_decrement<TAlias, TValueType>>
{
public:
    constexpr TAlias operator--(int) noexcept
    {
        TAlias & alias = this->to_alias();
        TAlias copy = alias;
        this->set_underlying_value(alias, this->get_underlying_value(alias) - static_cast<TValueType>(1));
        return copy;
    }
};

template <class TAlias, class TValueType>
class postfix_increment : public alias_operator<TAlias, TValueType, postfix_increment<TAlias, TValueType>>
{
public:
    constexpr TAlias operator++(int) noexcept
    {
        TAlias & alias = this->to_alias();
        TAlias copy = alias;
        this->set_underlying_value(alias, this->get_underlying_value(alias) + static_cast<TValueType>(1));
        return copy;
    }
};

template <class TAlias, class TValueType>
class prefix_and_postfix_decrement : public alias_operator<TAlias, TValueType, prefix_and_postfix_decrement<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator--() noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(alias, this->get_underlying_value(alias) - static_cast<TValueType>(1));
        return alias;
    }

    constexpr TAlias operator--(int) noexcept
    {
        TAlias & alias = this->to_alias();
        TAlias copy = alias;
        this->set_underlying_value(alias, this->get_underlying_value(alias) - static_cast<TValueType>(1));
        return copy;
    }
};

template <class TAlias, class TValueType>
class prefix_and_postfix_increment : public alias_operator<TAlias, TValueType, prefix_and_postfix_increment<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator++() noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(alias, this->get_underlying_value(alias) + static_cast<TValueType>(1));
        return alias;
    }

    constexpr TAlias operator++(int) noexcept
    {
        TAlias & alias = this->to_alias();
        TAlias copy = alias;
        this->set_underlying_value(alias, this->get_underlying_value(alias) + static_cast<TValueType>(1));
        return copy;
    }
};

template <class TAlias, class TValueType>
class prefix_decrement : public alias_operator<TAlias, TValueType, prefix_decrement<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator--() noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(alias, this->get_underlying_value(alias) - static_cast<TValueType>(1));
        return alias;
    }
};

template <class TAlias, class TValueType>
class prefix_increment : public alias_operator<TAlias, TValueType, prefix_increment<TAlias, TValueType>>
{
public:
    constexpr TAlias & operator++() noexcept
    {
        TAlias & alias = this->to_alias();
        this->set_underlying_value(alias, this->get_underlying_value(alias) + static_cast<TValueType>(1));
        return alias;
    }
};

template <class TAlias, class TValueType>
class remainder : public alias_operator<TAlias, TValueType, remainder<TAlias, TValueType>>
{
public:
    constexpr TAlias operator%(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) % this->get_underlying_value(other));
        return copy;
    }
};

template <template <class, class> class TAliasOperator, class TFriend>
class restrict
{
public:
    template <class TAlias, class TValueType>
    class type : protected TAliasOperator<TAlias, TValueType>
    {
        friend TFriend;
    };
};

template <class TFriend>
class restrict<construct, TFriend>
{
public:
    template <class TAlias, class TValueType>
    class type : protected construct<TAlias, TValueType>
    {
        friend enable_construct<TFriend>;
    };
};

template <class TFriend>
class restrict<get, TFriend>
{
public:
    template <class TAlias, class TValueType>
    class type : protected get<TAlias, TValueType>
    {
        friend enable_get<TFriend>;
    };
};

template <class TAlias, class TValueType>
class subtraction : public alias_operator<TAlias, TValueType, subtraction<TAlias, TValueType>>
{
public:
    constexpr TAlias operator-(TAlias other) const noexcept
    {
        TAlias copy = this->to_alias();
        this->set_underlying_value(copy, this->get_underlying_value(copy) - this->get_underlying_value(other));
        return copy;
    }
};

template <class TAlias, class TValueType>
class unary_minus : public alias_operator<TAlias, TValueType, unary_minus<TAlias, TValueType>>
{
public:
    constexpr TAlias operator-() const noexcept
    {
        TAlias other {};
        unary_minus::set_underlying_value(other, -unary_minus::get_underlying_value(this->to_alias()));
        return other;
    }
};

template <class TAlias, class TValueType>
class unary_plus : public alias_operator<TAlias, TValueType, unary_plus<TAlias, TValueType>>
{
public:
    constexpr friend TAlias operator+(TAlias alias) noexcept
    {
        return alias;
    }
};

template <class TAlias, class TValueType>
class STRONGLY_TYPED_ALIAS_EMPTY_BASES arithmetic
    : public addition<TAlias, TValueType>
    , public compound_assignment_addition<TAlias, TValueType>
    , public compound_assignment_division<TAlias, TValueType>
    , public compound_assignment_multiplication<TAlias, TValueType>
    , public compound_assignment_remainder<TAlias, TValueType>
    , public compound_assignment_subtraction<TAlias, TValueType>
    , public division<TAlias, TValueType>
    , public multiplication<TAlias, TValueType>
    , public prefix_and_postfix_decrement<TAlias, TValueType>
    , public prefix_and_postfix_increment<TAlias, TValueType>
    , public remainder<TAlias, TValueType>
    , public subtraction<TAlias, TValueType>
    , public unary_minus<TAlias, TValueType>
    , public unary_plus<TAlias, TValueType>
{
};

template <class TAlias, class TValueType>
class STRONGLY_TYPED_ALIAS_EMPTY_BASES bitwise
    : public bitwise_and<TAlias, TValueType>
    , public bitwise_left_shift<TAlias, TValueType>
    , public bitwise_not<TAlias, TValueType>
    , public bitwise_or<TAlias, TValueType>
    , public bitwise_right_shift<TAlias, TValueType>
    , public bitwise_xor<TAlias, TValueType>
    , public compound_assignment_bitwise_and<TAlias, TValueType>
    , public compound_assignment_bitwise_left_shift<TAlias, TValueType>
    , public compound_assignment_bitwise_or<TAlias, TValueType>
    , public compound_assignment_bitwise_right_shift<TAlias, TValueType>
    , public compound_assignment_bitwise_xor<TAlias, TValueType>
{
};

template <class TAlias, class TValueType>
class STRONGLY_TYPED_ALIAS_EMPTY_BASES comparable
    : public equal<TAlias, TValueType>
    , public greater<TAlias, TValueType>
    , public greater_or_equal<TAlias, TValueType>
    , public less<TAlias, TValueType>
    , public less_or_equal<TAlias, TValueType>
    , public not_equal<TAlias, TValueType>
{
};

template <class TAlias, class TValueType>
class STRONGLY_TYPED_ALIAS_EMPTY_BASES logical
    : public bool_conversion<TAlias, TValueType>
    , public logical_and<TAlias, TValueType>
    , public logical_not<TAlias, TValueType>
    , public logical_or<TAlias, TValueType>
{
};

template <class TAlias, class TValueType>
class STRONGLY_TYPED_ALIAS_EMPTY_BASES everything
    : public addition<TAlias, TValueType>
    , public bitwise_and<TAlias, TValueType>
    , public bitwise_left_shift<TAlias, TValueType>
    , public bitwise_not<TAlias, TValueType>
    , public bitwise_or<TAlias, TValueType>
    , public bitwise_right_shift<TAlias, TValueType>
    , public bitwise_xor<TAlias, TValueType>
    , public bool_conversion<TAlias, TValueType>
    , public compound_assignment_addition<TAlias, TValueType>
    , public compound_assignment_bitwise_and<TAlias, TValueType>
    , public compound_assignment_bitwise_left_shift<TAlias, TValueType>
    , public compound_assignment_bitwise_or<TAlias, TValueType>
    , public compound_assignment_bitwise_right_shift<TAlias, TValueType>
    , public compound_assignment_bitwise_xor<TAlias, TValueType>
    , public compound_assignment_division<TAlias, TValueType>
    , public compound_assignment_multiplication<TAlias, TValueType>
    , public compound_assignment_remainder<TAlias, TValueType>
    , public compound_assignment_subtraction<TAlias, TValueType>
    , public construct<TAlias, TValueType>
    , public division<TAlias, TValueType>
    , public equal<TAlias, TValueType>
    , public get<TAlias, TValueType>
    , public greater<TAlias, TValueType>
    , public greater_or_equal<TAlias, TValueType>
    , public less<TAlias, TValueType>
    , public less_or_equal<TAlias, TValueType>
    , public logical_and<TAlias, TValueType>
    , public logical_not<TAlias, TValueType>
    , public logical_or<TAlias, TValueType>
    , public multiplication<TAlias, TValueType>
    , public not_equal<TAlias, TValueType>
    , public prefix_and_postfix_decrement<TAlias, TValueType>
    , public prefix_and_postfix_increment<TAlias, TValueType>
    , public remainder<TAlias, TValueType>
    , public subtraction<TAlias, TValueType>
    , public unary_minus<TAlias, TValueType>
    , public unary_plus<TAlias, TValueType>
{
};
}
}

#define STRONGLY_TYPED_ALIAS(_name, _type, ...) \
    using _name = strongly_typed_alias::alias<struct strongly_typed_alias_anonymous_struct_ ## _name, _type, __VA_ARGS__>; \
    static_assert(sizeof(_name) == sizeof(_type), "sizeof mismatch"); \
    static_assert(alignof(_name) == alignof(_type), "alignof mismatch")

#endif
