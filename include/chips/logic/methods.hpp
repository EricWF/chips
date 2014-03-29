#ifndef CHIPS_LOGIC_METHODS_HPP
#define CHIPS_LOGIC_METHODS_HPP

# include "chips/logic/fwd.hpp"
# include "chips/core.hpp"
# include "chips/entity.hpp"

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                                 METHODS
////////////////////////////////////////////////////////////////////////////////
    
    // Every entity that has the possibility to change should implement this method.
    /// things like walls and floors can never change, so no overload is needed.
    /// No default provided
    constexpr struct 
    update_m : method_base<update_m, void(level &)> 
    {} update_ {};
    
    /// Move an object in its direction by its velocity.
    /// Default requires: position, direction, velocity
    constexpr struct 
    move_m : method_base<move_m, void(direction, level &)> 
    {} move_ {};
    
    /// Check if a given entity colides with you.
    constexpr struct 
    collides_m : method_base<collides_m, bool(entity const &) const>
    {} collides_ {};
    
    /// Apply results of a collision with a given entity.
    constexpr struct 
    on_collision_m : method_base<on_collision_m, void(entity &, level &)> 
    {} on_collision_ {};
    
    /// Used by clone machines to clone an object
    constexpr struct
    clone_m : method_base<clone_m, void(level &)>
    {} clone_ {};
   
////////////////////////////////////////////////////////////////////////////////
//                              COMMON METHODS
////////////////////////////////////////////////////////////////////////////////

// declared in fwd.hpp
# if defined(CHIPS_EXPOSITION)
    /// Default implementations are provided for some functions.
    namespace logic { namespace common
    {
        void move_(entity & self, direction d, level & l);
        bool always_collides_(entity const & self, entity const & other);
        bool collides_with_monster_(entity const & self, entity const & other);
    }}                                                      // namespace logic
# endif
}                                                           // namespace chips
#endif /* CHIPS_LOGIC_METHODS_HPP */