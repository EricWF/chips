#ifndef CHIPS_POSITION_HPP
# define CHIPS_POSITION_HPP

# include "chips/attribute.hpp"
# include <elib/aux.hpp>
# include <elib/lexical_cast.hpp>
# include <string>

namespace chips
{
    enum class direction : unsigned
    {
        N, W, S, E
    };
    
    inline std::string to_string(direction d)
    {
        std::string s = "direction: ";
        switch (d)
        {
            case direction::N:
                s += "N"; break;
            case direction::W:
                s += "W"; break;
            case direction::S:
                s += "S"; break;
            case direction::E:
                s += "E"; break;
        }
        return s;
    }
    
    struct position
    {
        int x, y;
    };
    
   
    inline std::string to_string(position p)
    {
        return elib::fmt("position: x=%i y=%i", p.x, p.y);
    }
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<direction> : elib::true_ {};
        
        template <>
        struct is_attribute_impl<position>  : elib::true_ {};
    }
    
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
    
# if defined(__GNUG__) && !defined(__clang__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-default"
# endif
    inline position move(position pos, direction dir, unsigned count = 1)
    {
        switch (dir)
        {
            case direction::N: pos.y += count; break;
            case direction::S: pos.y -= count; break;
            case direction::E: pos.x += count; break;
            case direction::W: pos.x -= count; break;
        }
        return pos;
    }
# if defined(__GNUG__) && !defined(__clang__)
#   pragma GCC diagnostic pop
# endif
    
    constexpr direction 
    turn_clockwise(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            ((static_cast<unsigned>(dir) + times) + 2) % 4
          );
    }
    
    constexpr direction 
    turn_counter_clockwile(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            (static_cast<unsigned>(dir) + times) % 4
          );
    }
    
    constexpr direction
    turn_around(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            (static_cast<unsigned>(dir) + (times * 2)) % 4
          );
    }
}                                                           // namespace chips
#endif /* CHIPS_POSITION_HPP */