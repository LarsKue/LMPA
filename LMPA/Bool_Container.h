//
// Created by Lars on 24/01/2019.
//

#ifndef LMPA_LIBRARY_BOOL_CONTAINER_H
#define LMPA_LIBRARY_BOOL_CONTAINER_H

#if __cplusplus <= 199711L
#error This library requires at least C++11.
#endif // C++11 check

#include <vector>
#include <iostream> // debug printing
#include <bitset> // debug printing

// change types here
namespace Bool_Container_Types {
    typedef unsigned long long                      value_type;
    typedef typename std::vector<value_type>        base_type;
    typedef typename base_type::size_type           size_type;
}

/* Consider treating these as though they had a special bit size
 *  And only showing that part to the user
 *  E.g. a 250-bit binary would really have 256 bits,
 *  but the user can only access the last 250 of those
 *  support this with two "reserve" functions, the regular safe reserve
 *  and an "unsafe" expand, which would leave values standing
 */

/**
 * \brief This class is a memory-contiguous bit-packing vector.
 * Note this class does not generally roll over the entire value
 * of the binary it represents upon overflow.
 */
class Bool_Container : protected Bool_Container_Types::base_type {
public:
    typedef Bool_Container_Types::value_type                value_type;
    typedef typename Bool_Container_Types::base_type        base_type;
    typedef typename Bool_Container_Types::size_type        size_type;

    // The token boolean is necessary to differ the function signature from the Conversion Constructor.
    explicit Bool_Container(size_type precision, bool) noexcept;

    // inherit base class constructors
    using base_type::base_type;

    // container bit-shifting
    Bool_Container& operator<<=(size_type n);
    Bool_Container& operator>>=(size_type n);

    Bool_Container operator<<(size_type n) const;
    Bool_Container operator>>(size_type n) const;


    Bool_Container& operator+=(const Bool_Container& b);

    Bool_Container& operator*=(const Bool_Container& b);

    bool get_bit(size_type n) const;
    void set_bit(size_type n, bool val);
    bool toggle_bit(size_type n);

    friend std::ostream& operator<<(std::ostream& stream, const Bool_Container& b) {
        stream << "0b";
        for (const value_type v : b) {
            stream << std::bitset<sizeof(value_type) * 8>(v);
        }
        return stream;
    }

};


#endif //LMPA_LIBRARY_BOOL_CONTAINER_H
