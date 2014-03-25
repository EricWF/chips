#ifndef CHIPS_CORE_POSITION_HPP
#define CHIPS_CORE_POSITION_HPP

# include "chips/core/direction.hpp"
# include "chips/entity/fwd.hpp"
# include <elib/aux.hpp>
# include <elib/fmt.hpp>
# include <string>

namespace chips
{
    struct position
    {
        constexpr position() : x(0), y(0) {}
        constexpr position(unsigned x_, unsigned y_) : x(x_), y(y_) {}
        
        unsigned x, y;
    };
    
    constexpr bool 
    operator==(position const & lhs, position const & rhs) noexcept
    { 
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    
    constexpr bool
    operator !=(position const & lhs, position const & rhs) noexcept
    {
        return lhs.x != rhs.x || lhs.y != rhs.y;
    }
    
    constexpr bool 
    operator<(position const & lhs, position const & rhs) noexcept
    {
        return lhs.x < rhs.x || 
                (lhs.x == rhs.x && lhs.y < rhs.y);
    }
    
    constexpr bool
    operator>(position const & lhs, position const & rhs) noexcept
    {
        return rhs < lhs; 
    }
    
    constexpr bool
    operator <=(position const & lhs, position const & rhs) noexcept
    {
        return !(rhs < lhs);
    }
    
    constexpr bool
    operator >=(position const & lhs, position const & rhs) noexcept
    {
        return !(lhs < rhs); 
    }
    
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-default"
# endif
    // TODO
    inline position move(position pos, direction dir, int count = 1)
    {
        switch (dir)
        {
            case direction::N: pos.y -= (unsigned)count; break;
            case direction::S: pos.y += (unsigned)count; break;
            case direction::E: pos.x += (unsigned)count; break;
            case direction::W: pos.x -= (unsigned)count; break;
        }
        return pos;
    }
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic pop
# endif
    
    
    inline std::string to_string(position p)
    {
        return elib::fmt("position: x=%i y=%i", p.x, p.y);
    }
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<position>  : elib::true_ {};
    }
}                                                           // namespace chips
#endif /* CHIPS_CORE_POSITION_HPP */