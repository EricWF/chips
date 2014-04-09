#ifndef CHIPS_LOGIC_TICK_HPP
#define CHIPS_LOGIC_TICK_HPP

# include "chips/logic/attributes.hpp"
# include "chips/logic/concepts.hpp"
# include "chips/entity.hpp"
# include <chrono>
# include <string>
# include <ratio>
# include <cstddef>

namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    /// Types used for measuring time
    using tick_clock = std::chrono::high_resolution_clock;
    using time_point = tick_clock::time_point;
    
    ////////////////////////////////////////////////////////////////////////////
    /// Ten ticks per second
    using resolution = std::ratio<1, 1000>;
    using tick_dur = std::chrono::duration<std::intmax_t, std::ratio<1, 100>>;
    
    ////////////////////////////////////////////////////////////////////////////
    /// An attribute used for speed
    struct updates_per_tick : attribute_base
    {
        constexpr updates_per_tick()
          : updates_per_tick(1)
        {}
        constexpr explicit 
        updates_per_tick(std::intmax_t xnum, std::intmax_t xden = 1)
          : num(xnum), den(xden)
        {}
        
        std::intmax_t num;
        std::intmax_t den;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    constexpr bool operator==(
        updates_per_tick const & lhs
      , updates_per_tick const & rhs
      ) noexcept
    {
        return (lhs.num * rhs.den == rhs.num * lhs.den);
    }
        
    ////////////////////////////////////////////////////////////////////////////
    constexpr bool operator!=(
        updates_per_tick const & lhs
      , updates_per_tick const & rhs
      ) noexcept
    {
        return !(lhs == rhs);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    /// The common speeds
    using speed = updates_per_tick;
    
    constexpr speed half_speed(1, 50);
    constexpr speed regular_speed(1, 20);
    constexpr speed full_speed(1, 10);
    
    ////////////////////////////////////////////////////////////////////////////
    inline speed speed_from_string(std::string const & s)
    {
        if (s == "half_speed")
            return half_speed;
        else if (s == "regular_speed")
            return regular_speed;
        else if (s == "full_speed")
            return full_speed;
        else
            throw "TODO";
    }
    
    ////////////////////////////////////////////////////////////////////////////
    struct UpdateOnTick : concept_base<UpdateOnTick>
    {
        UpdateOnTick(std::intmax_t xtick) noexcept
          : m_tick(xtick)
        {}
        
        UpdateOnTick(UpdateOnTick const &) = default;
        UpdateOnTick & operator=(UpdateOnTick &&) = default;
        
        bool test(entity const & e) const
        {
            if (not e)return false;
            speed const *sptr = e.get_raw<speed>();
            if (not sptr) return false;
            ELIB_ASSERT(sptr->num == 1);
            return (m_tick % sptr->den) == 0 && (not e.has<move_lock>());
        }
        
    private:
        std::intmax_t m_tick;
    };
    
}                                                           // namespace chips
#endif /* CHIPS_LOGIC_TICK_HPP */