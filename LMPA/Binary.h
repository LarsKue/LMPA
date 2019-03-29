
#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check


#include <vector>
#include <iostream>
#include <algorithm>

#ifdef DEBUG
#include <bitset>
#endif



class LMPA;


namespace Binary_Types {
    typedef             unsigned char                   value_type;
    typedef typename    std::vector<value_type>         base_type;
    typedef typename    base_type::size_type            size_type;
}


class Binary : Binary_Types::base_type {
    
    friend class LMPA;
    
public:
    typedef             Binary_Types::value_type        value_type;
    typedef typename    Binary_Types::base_type         base_type;
    typedef typename    Binary_Types::size_type         size_type;
    
    // Zero-Initialization Constructor, token bool for signature distinction
    explicit Binary(size_type precision, bool) noexcept;

    // conversion constructor
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value, bool>::type = true>
    Binary(const T& initializer) noexcept {
        for (size_type i = 0; i < sizeof(initializer) * 8; ++i) {
            emplace_back(initializer & 1 << i);
        }
        precision = sizeof(initializer) * 8;
        std::reverse(this->begin(), this->end());
    }


    // inherit base class constructors
    using base_type::base_type;

    // inherit iterators
    using base_type::iterator;
    using base_type::reverse_iterator;

    /// Operators ///

    // shift assignment
    Binary& operator<<=(size_type n);
    Binary& operator>>=(size_type n);

    // shift
    Binary operator<<(size_type n) const;
    Binary operator>>(size_type n) const;

    // arithmetic
    Binary& operator+=(const Binary& other);
    Binary& operator-=(const Binary& other);
    Binary& operator*=(const Binary& other);
    Binary& operator/=(const Binary& other);

    bool get_bit(size_type index) const;
    void set_bit(size_type index, bool val);
    bool toggle_bit(size_type index);
    void flip();

#ifdef DEBUG
    friend std::ostream& operator<<(std::ostream& stream, const Binary& b) {
        stream << "0b";
        for (const value_type v : b) {
            stream << std::bitset<sizeof(value_type) * 8>(v);
        }
        return stream;
    }
#endif

private:
    size_type precision = 0;
    
};