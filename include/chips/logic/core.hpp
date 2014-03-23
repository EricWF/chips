#ifndef CHIPS_LOGIC_CORE_HPP
#define CHIPS_LOGIC_CORE_HPP

# include "chips/logic/fwd.hpp"
# include "chips/core.hpp"
# include "chips/entity.hpp"
# include "chips/game.hpp"
# include <elib/aux.hpp>

namespace chips { namespace logic
{
    inline void init(entity & e, level & l)
    {
        if      (is_chip(e))  init_chip(e, l);
        else if (is_actor(e)) init_actor(e, l);
        else if (is_item(e))  init_item(e, l);
        else if (is_base(e))  init_base(e, l);
        else if (e.id() == entity_id::BAD_ID) {}
        else
        {
            log::err("In default case for init with entity %s"
                    , to_string(e.id()));
        }
    }
    
    inline void process(entity & e, level & l)
    {
        if      (is_chip(e))  process_chip(e, l);
        else if (is_actor(e)) process_actor(e, l);
        else if (is_item(e))  process_item(e, l);
        else if (is_base(e))  process_base(e, l);
        else
        {
            ELIB_ASSERT(false, "I should not be here!");
        }
    }

    inline void finalize(entity & e, level & l)
    {
        if      (is_chip(e))   finalize_chip(e, l);
        else if (is_actor(e))  finalize_actor(e, l);
        else if (is_item(e))   finalize_item(e, l);
        else if (is_base(e))   finalize_base(e, l);
        else
        {
            ELIB_ASSERT(false, "I should not be here!");
        }
    }
    
    void move_on_ice(entity & e, entity const & ice, level &);
    void move_on_force_floor(entity & e, entity const & ff, level &);
    
    struct reading_order_cmp
    {
        bool operator()(entity const &, entity const &) const;
    };
    
    struct reverse_reading_order_cmp
    {
        bool operator()(entity const &, entity const &) const;
    };
}}                                                          // namespace chips

namespace chips
{
///////////////////////////////////////////////////////////////////////////////
//                              Helpers
////////////////////////////////////////////////////////////////////////////////
    
    inline void clean_entity(entity & e)
    {
        if (e.has<position>())
        {
            position p = e.get<position>();
            e.clear();
            e <<  p;
        }
    }

    inline bool same_position(entity const & lhs, entity const & rhs)
    {
        if (!lhs || !rhs || &lhs == &rhs)
            return false;
        return lhs.get<position>() == rhs.get<position>();
    }
    
    inline bool at_location(entity_locator loc, entity const & e)
    {
        return e 
          && e.id() == loc.id
          && e.get<position>() == loc.pos;
    }
    
////////////////////////////////////////////////////////////////////////////////
//                               MISC ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////
    
    /// A list of entities to be called after a given event
    namespace detail { struct entity_list_tag {}; } 
    using entity_list = any_attribute<std::vector<entity_locator>, detail::entity_list_tag>;
    
    /// Used by socket to count the number of chips it needs
    namespace detail { struct chip_lock_tag {}; }
    using chip_count_lock = any_attribute<unsigned, detail::chip_lock_tag>;
    
    /// For use with traps. 
    namespace detail { struct trapped_entity_tag {}; }
    using trapped_entity = any_attribute<entity*, detail::trapped_entity_tag>;
    
    /// For use with traps and other things
    struct move_lock : attribute_base {};
    
////////////////////////////////////////////////////////////////////////////////
//                                 METHODS
////////////////////////////////////////////////////////////////////////////////
  
    /// Every entity that has the possibility to change should implement this method.
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
//                               CONCEPTS
////////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <unsigned Times, bool DoColl = false>
        struct EntityAtPosImpl : concept_base<EntityAtPosImpl<Times, DoColl>>
        {
            EntityAtPosImpl(entity const & e)
            {
                REQUIRE_CONCEPT(e, EntityHas<position, direction>);
                
                m_pos = move(
                    e.get<position>()
                  , turn_left(e.get<direction>(), Times)
                  , 1
                );
                
                m_ent = entity(e.id(), m_pos);
            }

            bool test(entity const & e) const
            {
                if (!e || !e.has<position>()) return false;

                return e.get<position>() == m_pos
                    && m_ent.id() != e.id()
                    && (!DoColl || (e.has(collides_) && e(collides_, m_ent)));
            }
            
        private:
            position m_pos;
            entity m_ent;
        };

    }                                                       // namespace detail
  
    ////////////////////////////////////////////////////////////////////////////
    struct CollidesWith : concept_base<CollidesWith>
    {
        CollidesWith(entity const & e)
          : m_eptr(&e)
        {}
        
        bool test(entity const & e) const
        {
            return &e != m_eptr
                  && e.has(collides_) && e(collides_, *m_eptr);
        }

    private:
        entity const* m_eptr;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    struct SamePosition : concept_base<SamePosition>
    {
        SamePosition(entity const & e)
          : m_eptr(&e)
        {}
        
        bool test(entity const & e) const
        {
            return same_position(e, *m_eptr);
        }

    private:
        entity const* m_eptr;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    struct AtLocation : concept_base<AtLocation>
    {
        AtLocation(entity_locator loc)
          : m_loc(loc)
        {}
        
        bool test(entity const & e) const
        {
            return at_location(m_loc, e);
        }
          
    private:
        entity_locator m_loc;
    };
  
    ////////////////////////////////////////////////////////////////////////////
    template <entity_id ID>
    struct OnEntity : concept_base<OnEntity<ID>>
    {
        OnEntity(entity const & e)
        {
            m_ent = entity(e.id(), e.get<position>());
        }
        
        bool test(entity const & e) const
        {
            return same_position(e, m_ent) && e.id() == ID;
        }

    private:
        entity m_ent;
    };

  
    /// Concepts about basic functionality
    using CanUpdate  = EntityHas<update_m>;
    using CanMove    = Concept<EntityHas<move_m>, EntityHasNone<move_lock>>;
    using CanCollide = EntityHasAny<collides_m, on_collision_m>;
  
    using InFront = detail::EntityAtPosImpl<0>;
    using Behind  = detail::EntityAtPosImpl<2>;
    using ToRight = detail::EntityAtPosImpl<3>;
    using ToLeft  = detail::EntityAtPosImpl<1>;
    
    using ColFront = detail::EntityAtPosImpl<0, true>;
    using ColBack  = detail::EntityAtPosImpl<2, true>;
    using ColRight = detail::EntityAtPosImpl<3, true>;
    using ColLeft  = detail::EntityAtPosImpl<1, true>;

    using HasPos        = EntityHas<position>;
    using HasDir        = EntityHas<direction>;
    using HasEntityList = EntityHas<entity_list>;
    
    using IsExit       = Concept<EntityIs<entity_id::exit>>;
    using IsWater      = Concept<EntityIs<entity_id::water>>;
    using IsFire       = Concept<EntityIs<entity_id::fire>>;
    using IsIce        = Concept<EntityIs<entity_id::ice>>;
    using IsForceFloor = Concept<EntityIs<entity_id::force_floor>>;
    using IsTeleport   = Concept<EntityIs<entity_id::teleport>>;
    
    using OnExit       = OnEntity<entity_id::exit>;
    using OnWater      = OnEntity<entity_id::water>;
    using OnFire       = OnEntity<entity_id::fire>;
    using OnIce        = OnEntity<entity_id::ice>;
    using OnForceFloor = OnEntity<entity_id::force_floor>;
    
    using IsChip        = EntityIs<entity_id::chip>;
    
    using IsGreenButton = EntityIs<entity_id::green_button>;
    using IsToggleWall  = EntityIs<entity_id::toggle_wall>;
    
    using IsBlueButton  = EntityIs<entity_id::blue_button>;
    using IsTank        = EntityIs<entity_id::tank>;
    
    using IsBrownButton = EntityIs<entity_id::brown_button>;
    using IsTrap        = EntityIs<entity_id::trap>;
    
    using IsRedButton    = EntityIs<entity_id::red_button>;
    using IsCloneMachine = EntityIs<entity_id::clone_machine>;
    
    
////////////////////////////////////////////////////////////////////////////////
//                              COMMON METHODS
////////////////////////////////////////////////////////////////////////////////

    /// Default implementations are provided for some functions.
    namespace common
    {
        
        inline void 
        move_(entity & self, direction d, level & l)
        {
            ELIB_ASSERT(self);
            position old_pos = self.get<position>();
            
            self << d << move(old_pos, d, 1);
            
            for (auto & e : SamePosition(self).filter(l.entity_list))
            {
                e.call_if(on_collision_, self, l);
                if (!self) return;
                if (e.has(collides_) && e(collides_, self))
                {
                    self << old_pos;
                    return;
                }
            }
        }
        
        inline bool 
        always_collides_(entity const & self, entity const & other)
        {
            return same_position(self, other);
        }
        
        inline bool 
        collides_with_monster_(entity const & self, entity const & other)
        {
            return same_position(self, other)
                 && is_monster(other);
        }
        
    }                                                       // namespace common
}                                                           // namespace chips
#endif /* CHIPS_LOGIC_CORE_HPP */