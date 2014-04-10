#ifndef CHIPS_CORE_DIRECTION_HPP
#define CHIPS_CORE_DIRECTION_HPP

# include "chips/core/error.hpp"
# include "chips/entity/fwd.hpp"
# include <elib/aux.hpp>
# include <elib/fmt.hpp>
# include <string>


namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                           DIRECTION
////////////////////////////////////////////////////////////////////////////////
    
    enum class direction : unsigned
    {
        N, W, S, E
    };
    
    constexpr direction NORTH = direction::N;
    constexpr direction WEST = direction::W;
    constexpr direction SOUTH = direction::S;
    constexpr direction EAST = direction::E;

    constexpr direction 
    turn_right(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            ((static_cast<unsigned>(dir) + times) + 2) % 4
        );
    }
    
    constexpr direction 
    turn_left(direction dir, unsigned times = 1) noexcept
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
    
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-default"
# endif
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
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic pop
# endif

    inline direction to_direction(std::string const & s)
    {
        if      ("N" == s) return direction::N;
        else if ("W" == s) return direction::W;
        else if ("S" == s) return direction::S;
        else if ("E" == s) return direction::E;
        else
        {
            ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                "Unknown direction passed to to_direction: \"%s\"", s
            )));
        }
    }
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<direction> : elib::true_ {};
    }                                                       // namespace extension
}                                                           // namespace chips
#endif /* CHIPS_CORE_DIRECTION_HPP */