
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

    // inherit base class constructors
    using base_type::base_type;

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

#ifdef DEBUG
    friend std::ostream& operator<<(std::ostream& stream, const Binary& b) {
        stream << "0b";
        for (const value_type v : b) {
            stream << std::bitset<sizeof(value_type) * 8>(v);
        }
        return stream;
    }
#endif
    
};