#ifndef CHIPS_LOGIC_CORE_HPP
# define CHIPS_LOGIC_CORE_HPP

# include "chips/logic/fwd.hpp"
# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <elib/aux.hpp>
# include <elib/lexical_cast.hpp>

namespace chips { namespace logic
{
    inline void init(entity & e, level & l)
    {
        if (is_chip(e))
            init_chip(e, l);
        else if (is_actor(e))
            init_actor(e, l);
        else if (is_item(e))
            init_item(e, l);
        else if (is_base(e))
            init_base(e, l);
        else
        {
            ELIB_ASSERT(false, "I should not be here!");
        }
    }
    
    inline void process(entity & e, level & l)
    {
        if (is_chip(e))
            process_chip(e, l);
        else if (is_actor(e))
            process_actor(e, l);
        else if (is_item(e))
            process_item(e, l);
        else if (is_base(e))
            process_base(e, l);
        else
        {
            ELIB_ASSERT(false, "I should not be here!");
        }
    }

    inline void finalize(entity & e, level & l)
    {
        if (is_chip(e))
            finalize_chip(e, l);
        else if (is_actor(e))
            finalize_actor(e, l);
        else if (is_item(e))
            finalize_item(e, l);
        else if (is_base(e))
            finalize_base(e, l);
        else
        {
            ELIB_ASSERT(false, "I should not be here!");
        }
    }
}}                                                          // namespace chips


namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               MISC ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct velocity_tag {}; }

    /// A attribute that stores velocity as an unsigned int
    using velocity = any_attribute<unsigned, detail::velocity_tag>;
    
    inline std::string to_string(velocity const & v)
    {
        return "velocity = " + elib::lexical_cast<std::string>(v.value());
    }

    inline velocity to_velocity(std::string const & s)
    {
        return velocity( elib::lexical_cast<unsigned>(s) );;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct toggle_state_tag {}; }
    
    /// A boolean meant for use as a toggle switch via toggle_ notify_ binding
    using toggle_state = any_attribute<bool, detail::toggle_state_tag>;
    
    inline std::string to_string(toggle_state const & s)
    {
        return "toggle_state = " + elib::lexical_cast<std::string>(s.value());
    }
    
    inline toggle_state to_toggle_state(std::string const & s)
    {
        return toggle_state( elib::lexical_cast<bool>(s) );
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct bindings_tag {}; } 
    
    /// A list of pointers no entities that should be notified via notify_ call.
    using bindings = any_attribute<std::vector<entity*>, detail::bindings_tag>;
    
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct chip_count_tag {}; }
    
    using sock_chip_count = any_attribute<unsigned, detail::chip_count_tag>;
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct chip_at_exit_tag {}; }
    
    using chip_at_exit = any_attribute<bool, detail::chip_at_exit_tag>;
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct moveable_tag {}; }
    
    using moveable = any_attribute<bool, detail::moveable_tag>;
    
    
////////////////////////////////////////////////////////////////////////////////
//                                 METHODS
////////////////////////////////////////////////////////////////////////////////
  
    /// Every entity that has the possibility to change should implement this method.
    /// things like walls and floors can never change, so no overload is needed.
    /// No default provided
    struct update_m : method_base<update_m, void(level &)> {};
    
    /// for use with the toggle_switch attribute. This method sets toggle_switch
    /// Default Requires: toggle_switch
    struct toggle_m : method_base<toggle_m, void()> {};
    
    /// Call toggle_ on all the attributes in the bindings attribute.
    /// Default Requires: bindings, (bound entities require toggle_)
    struct notify_m : method_base<notify_m, void() const> {};
    
    /// Move an object in its direction by its velocity.
    /// Default requires: position, direction, velocity
    struct move_m : method_base<move_m, void(direction, level &)>{};
    
    /// Move an object in a given direction, N times. and turn the entity to
    /// face that direction.
    struct move_in_m : method_base<move_in_m, void(direction, int)> {};
    
    /// Check if a given entity colides with you.
    struct collides_m : method_base<collides_m, bool(entity const &) const> {};
    
    /// Apply results of a collision with a given entity.
    struct on_collision_m : method_base<on_collision_m, void(entity &, level & l)> {};
    
    
    constexpr update_m       update_      {};
    constexpr toggle_m       toggle_      {};
    constexpr notify_m       notify_      {};
    constexpr move_m         move_        {};
    constexpr move_in_m      move_in_     {};
    constexpr collides_m     collides_    {};
    constexpr on_collision_m on_collision_{};

    
////////////////////////////////////////////////////////////////////////////////
//                               CONCEPTS
////////////////////////////////////////////////////////////////////////////////
    /// Concepts about basic functionality
    using Updateable = EntityHas<update_m>;
    using Moveable   = EntityHas<direction, velocity, position>;
    
    using Directional = 
        Concept<EntityHas<direction>, EntityMatches<&is_directional>>;
    
    using Toggleable = EntityHas<toggle_state, toggle_m>;
    using Bindable   = EntityHas<bindings, notify_m>;
    
    /// Concepts for certain types
    using Chip = EntityIs<entity_id::chip>;
    using Monster = EntityMatches<&is_monster>;
    using ActingBase = EntityMatches<&is_acting_base>; 
    
    using IsGreenButton = EntityIs<entity_id::green_button>;
    using IsToggleWall  = EntityIs<entity_id::toggle_wall>;
    
    using IsBlueButton  = EntityIs<entity_id::blue_button>;
    using IsTank        = EntityIs<entity_id::tank>;
    
////////////////////////////////////////////////////////////////////////////////
//                              COMMON METHODS
////////////////////////////////////////////////////////////////////////////////
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wglobal-constructors"
# endif
    /// Default implementations are provided for some functions.
    namespace common
    {
        auto toggle_ =
        [](entity & e)
        {
            REQUIRE_CONCEPT(e, EntityHas<toggle_state>);
            e << toggle_state(!e.get<toggle_state>());
        };
        
        auto notify_ =
        [](entity const & e)
        {
            REQUIRE_CONCEPT(e, EntityHas<bindings>);
            for (entity * other_ptr : *e.get<bindings>())
            {
                ELIB_ASSERT(other_ptr);
                other_ptr->call_if(chips::toggle_);
            }
        };
        
        auto notify_on_collision_ =
        [](entity & self, entity &, level &)
        {
            REQUIRE_CONCEPT(self, Bindable);
            ELIB_ASSERT(self);
            self.call(chips::notify_);
        };
        
        auto move_ = 
        [](entity & self, direction d, level & l)
        {
            ELIB_ASSERT(self);
            position old_pos = self.get<position>();
            
            self << d << move(old_pos, d, 1);
            
            auto Collisions = Predicate(
                [&](entity const & other)
                {
                    return other 
                        && other.get<position>() == self.get<position>()
                        && &self != &other;
                }
              );
            
            for (auto & e : Collisions.filter(l.entity_list))
            {
                if (!e || &e == &self) continue;
                if (e && e.has(chips::on_collision_))
                    e(chips::on_collision_, self, l);
                if (self && e 
                    && e.has<position>() && e.get<position>() == self.get<position>()
                    && e.has(chips::collides_) && e(chips::collides_, self))
                {
                    self << old_pos;
                    break;
                }
                if (!self) return;
            }
        };
        
        auto move_in_ =
        [](entity & e, direction d, int n)
        {
            REQUIRE_CONCEPT(e, Concept<Directional, EntityHas<position>>);
            position p;
            e >> p;
            e << d << move(p, d, n);
        };
        
        auto always_collides_ =
        [](entity const &, entity const &)
        {
            return true;
        };
        
        auto never_collides_ =
        [](entity const &, entity const &)
        {
            return false;
        };
        
        auto collides_with_monster_ =
        [](entity const &, entity const & e)
        {
            return e && is_monster(e);
        };
        
        auto null_on_collision_ = [](entity &, entity &) {};
        
    }                                                       // namespace common
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif
    
}                                                           // namespace chips
#endif /* CHIPS_LOGIC_CORE_HPP */