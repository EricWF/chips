#ifndef CHIPS_LOGIC_CONCEPTS_HPP
#define CHIPS_LOGIC_CONCEPTS_HPP

# include "chips/logic/fwd.hpp"
# include "chips/logic/attributes.hpp"
# include "chips/logic/methods.hpp"
# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <elib/aux.hpp>


namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    struct Alive : public concept_base<Alive>
    {
        bool test(entity const & e) const
        {
            return bool(e);
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    struct Dead : concept_base<Dead>
    {
        bool test(entity const & e) const
        {
            return !bool(e);
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <entity_id ...Ids>
    struct EntityIs : concept_base<EntityIs<Ids...>>
    {
        bool test(entity const & e) const
        {
            return concept_or(
                (e.id() == Ids)...
            );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <entity_id ...Ids>
    struct EntityIsNot : concept_base<EntityIsNot<Ids...>>
    {
        bool test(entity const & e) const
        {
            return concept_and(
                (e.id() != Ids)...
            );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <class ...AttrOrMeth>
    struct EntityHas : concept_base<EntityHas<AttrOrMeth...>>
    {
        bool test(entity const & e) const
        {
            return concept_and(
                concept_check<AttrOrMeth>(e)...
              );
        }
    };
    
    template <class ...AttrOrMeth>
    struct EntityHasAny : concept_base<EntityHasAny<AttrOrMeth...>>
    {
        bool test(entity const & e) const
        {
            return concept_or(
                concept_check<AttrOrMeth>(e)...
              );
        }
    };
    
    template <class ...AttrOrMeth>
    struct EntityHasNone : concept_base<EntityHasNone<AttrOrMeth...>>
    {
        bool test(entity const & e) const
        {
            return !concept_or(
                concept_check<AttrOrMeth>(e)...
              );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        using query_fn_ptr = bool(*)(entity_id);
    }

    template <detail::query_fn_ptr ...Querys>
    struct EntityMatches : concept_base<EntityMatches<Querys...>>
    {
        bool test(entity const & e) const
        {
            return concept_and(
                Querys(e.id())...
            );
        }
    };
    
    template <detail::query_fn_ptr ...Querys>
    struct EntityMatchesAny : concept_base<EntityMatchesAny<Querys...>>
    {
        bool test(entity const & e) const
        {
            return concept_or(
                Querys(e.id())...
            );
        }
    };

    template <detail::query_fn_ptr ...Querys>
    struct EntityMatchesNone : concept_base<EntityMatchesNone<Querys...>>
    {
        bool test(entity const & e) const
        {
            return !concept_or(
                Querys(e.id())...
            );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        using pred_type = bool(*)(entity const &);
    }
    
    struct Predicate : concept_base<Predicate>
    {
        Predicate(std::function<bool(entity const &)> fn)
          : m_fn(fn)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(Predicate);
        
        bool test(entity const & e) const
        {
            return m_fn(e);
        }

    private:
        std::function<bool(entity const &)> m_fn;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    struct AtPosition : concept_base<AtPosition>
    {
        AtPosition(position p) : m_pos(p) {}
        
        ELIB_DEFAULT_COPY_MOVE(AtPosition);
        
        bool test(entity const & e) const
        {
            return (bool(e) && e.has<position>() 
                  && e.get<position>() == m_pos);
        }

    private:
        position m_pos;
    };
    
    struct AtEntity : concept_base<AtEntity>
    {
        AtEntity(entity const & e)
        {
            ELIB_ASSERT(e.has<position>());
            m_pos = e.get<position>();
        }

        bool test(entity const & e) const
        {
            if (!e || !e.has<position>()) return false;
            return e.get<position>() == m_pos;
        }
        
    private:
        position m_pos;
    };
    
    struct HasID : concept_base<HasID>
    {
        HasID(entity_id id) : m_id(id) {}
        
        ELIB_DEFAULT_COPY_MOVE(HasID);
        
        bool test(entity const & e) const
        {
            return e.id() == m_id;
        }

    private:
        entity_id m_id;
    };
    
////////////////////////////////////////////////////////////////////////////////
//                               CONCEPTS
////////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <int Times, bool DoColl = false>
        struct EntityAtPosImpl : concept_base<EntityAtPosImpl<Times, DoColl>>
        {
            EntityAtPosImpl(entity const & e)
            {
                if (Times != -1) {
                    REQUIRE_CONCEPT(e, EntityHas<position, direction>);
                    m_pos = move(
                        e.get<position>()
                    , turn_left(e.get<direction>(), static_cast<unsigned>(Times))
                    , 1
                    );
                } else {
                    REQUIRE_CONCEPT(e, EntityHas<position>);
                    m_pos = e.get<position>();
                }
                
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
          : m_eptr(e)
        {}
        
        bool test(entity const & e) const
        {
            return same_position(e, m_eptr);
        }

    private:
        entity m_eptr;
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
    using HasUpdateLock = EntityHas<update_lock>;
    using CanUpdate  = Concept<EntityHas<update_m>>;
    using CanMove    = Concept<EntityHas<move_m>, EntityHasNone<move_lock>>;
    using CanCollide = EntityHasAny<collides_m, on_collision_m>;
  
    using InFront = detail::EntityAtPosImpl<0>;
    using Behind  = detail::EntityAtPosImpl<2>;
    using ToRight = detail::EntityAtPosImpl<3>;
    using ToLeft  = detail::EntityAtPosImpl<1>;
    
    using ColAt = detail::EntityAtPosImpl<-1, true>;
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
    
    using HasInventory = EntityHas<inventory>;
}                                                           // namespace chips
#endif /* CHIPS_LOGIC_CONCEPTS_HPP */