#ifndef CHIPS_POSITION_HPP
#define CHIPS_POSITION_HPP

namespace chips
{
    enum class direction : unsigned
    {
        N, W, S, E
    };
    
    struct position
    {
        int x, y;
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
    
    inline position & move(position & pos, direction dir, unsigned count = 1)
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