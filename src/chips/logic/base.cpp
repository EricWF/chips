#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>

namespace chips { namespace logic
{
    namespace 
    {
        ////////////////////////////////////////////////////////////////////////
        //                            FLOOR
        ////////////////////////////////////////////////////////////////////////

        bool elem_floor_collide(entity const & self, entity const & other)
        {
            return same_position(self, other)
                && !(is_chip(other) || other.id() == entity_id::block);
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_water(entity & e, level &)
        {
            auto water_on_collide =
            [](entity & self, entity & other, level & l)
            {
                if (is_chip(other))
                {
                    auto & inv = other.get<inventory>();
                    if (!inv.contains(entity_id::flippers))
                        other.kill();
                    return;
                }
                else if (other.id() == entity_id::block)
                {
                    other.kill();
                    clean_entity(self);
                    self.id(entity_id::dirt);
                    init(self, l);
                    return;
                }
            };
                
            e << method(collides_, elem_floor_collide)
              << method(on_collision_, water_on_collide);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_fire(entity & e,  level &)
        {
            auto fire_on_collide =
            [](entity &, entity & other, level &)
            {
                if (!is_chip(other)) return;
                auto & inv  = other.get<inventory>();
                    
                if (inv.contains(entity_id::fire_boots)) { 
                }
                else {
                    other.kill();
                }
            };
                
            e << method(collides_, elem_floor_collide)
              << method(on_collision_, fire_on_collide);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_exit(entity & e, level &)
        {
            e << method(collides_, common::collides_with_monster_);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_green_button(entity & e, level &)
        {
            auto on_col =
            [](entity & self, entity & other, level & lev)
            {
                if (!self || !other || !is_actor(other))
                    return;
                            
                auto ToggleWalls = Concept<EntityIs<entity_id::toggle_wall>>();
                        
                for (auto & wall : ToggleWalls.filter(lev.entity_list))
                {
                    tile_id curr_id;
                    wall >> curr_id;
                    if (curr_id == tile_id::toggle_wall_open)
                        wall << tile_id::toggle_wall_closed;
                    else 
                        wall << tile_id::toggle_wall_open;
                }
            };
                    
            e << method(on_collision_, on_col);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_blue_button(entity & e, level &)
        {
            auto on_col =
            [](entity & self, entity & other, level & lev)
            {
                if (!self || !other || !is_actor(other))
                    return;
                    
                auto Tanks = Concept<EntityIs<entity_id::tank>>();
                for (auto & t :  Tanks.filter(lev.entity_list))
                {
                    t << turn_around(t.get<direction>());
                }
            };
                    
            e << method(on_collision_, on_col);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_brown_button(entity &, level &)
        {
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_trap(entity & e, level &)
        {
            auto collides =
            [](entity const & self, entity const & other)
            {
                REQUIRE_CONCEPT(self, HasEntityList);
                if (! same_position(self, other)) 
                    return false;
                if (self.has<trapped_entity>())
                {
                    entity const* eptr = self.get<trapped_entity>();
                    return (eptr != &other);
                }
                
                return false;
            };
            
            auto on_collide =
            [](entity & self, entity & other, level & lv)
            {
                if (self.has<trapped_entity>()) return;
                REQUIRE_CONCEPT(self, HasEntityList);
                
                auto & el = self.get<entity_list>().get();
                for (auto loc : el)
                {
                    auto IsActive = 
                        Concept<EntityIsNot<entity_id::brown_button>>(
                            AtPosition(loc.pos)
                        );
                    if (IsActive.contains(lv.entity_list) 
                        || lv.chip.get<position>() == loc.pos
                      )
                    {
                        return;
                    }
                }
                
                log::info("trapping entity %s"
                        , to_string(other.id())
                    );
                self << trapped_entity(&other);
                other << move_lock();
            };
            
            auto on_update =
            [](entity & self, level & lv)
            {
                if (!self.has<trapped_entity>()) return;
                    
                auto & el = self.get<entity_list>().get();
                for (auto loc : el)
                {
                    auto IsActive = 
                        Concept<EntityIsNot<entity_id::brown_button>>(
                            AtPosition(loc.pos)
                        );
                    if (IsActive.contains(lv.entity_list) 
                        || lv.chip.get<position>() == loc.pos
                      )
                    {
                        entity *eptr = self.get<trapped_entity>();
                        eptr->remove<move_lock>();
                        self.remove<trapped_entity>();
                    }
                }
            };
            
            e << entity_list()
              << method(update_, on_update)
              << method(collides_, collides)
              << method(on_collision_, on_collide);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_red_button(entity & e, level &)
        {
            auto on_col =
            [](entity & self, entity & other, level & lv)
            {
                REQUIRE_CONCEPT(self, HasEntityList);
                if (!other || !is_actor(other)) return;
                    
                auto & el = self.get<entity_list>().get();
                for (auto eloc : el)
                {
                    entity & cloner = AtLocation(eloc).get(lv.entity_list);
                    cloner(clone_, lv);
                }
            };
            
            e << entity_list()
              << method(on_collision_, on_col);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_bomb(entity & e, level &)
        {
            auto on_collide =
            [](entity & self, entity & other, level &)
            {
                ELIB_ASSERT(is_actor(other));
                other.kill();
                self.clear_methods();
                self.remove<tile_id>();
                self.id(entity_id::floor);
            };
            
            e << method(on_collision_, on_collide);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_gravel(entity & e, level &)
        {
            auto collides =
            [](entity const &, entity const & other)
            {
                return is_monster(other);
            };
            e << method(collides_, collides);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_dirt(entity & e, level &)
        {
            auto collides =
            [](entity const &, entity const & other)
            {
                return other && !is_chip(other);
            };
            
            auto on_col =
            [](entity & self, entity & other, level &)
            {
                if (other && is_chip(other))
                {
                    clean_entity(self);
                    self.id(entity_id::floor);
                }
            };
            
            e << method(collides_, collides)
              << method(on_collision_, on_col);
        }
        
        
    ////////////////////////////////////////////////////////////////////////////
    //                            WALL
    ////////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        void init_lock(entity & e, level &)
        {
            auto lock_on_col =
            [](entity & self, entity & other, level & l)
            {
                if (!is_chip(other)) return;
                ELIB_ASSERT(other);
                auto & inv = other.get<inventory>();
                entity_id req_key = key_for_lock(self.id());
                if (inv.contains(req_key))
                {
                    inv.use_item(req_key);
                    clean_entity(self);
                    self.id(entity_id::floor);
                    init(self, l);
                }
            };

            e << method(collides_, common::always_collides_)
              << method(on_collision_, lock_on_col);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_socket(entity & e, level &)
        {
            
            auto socket_on_col =
            [](entity & self, entity & other, level & l)
            {
                if (!is_chip(other)) return;
                REQUIRE_CONCEPT(other, HasInventory);
                    
                auto & inv = other.get<inventory>();
                unsigned req = l.chip_count();
                    
                if (req <= inv.count(entity_id::computer_chip))
                {
                    l.chip_count(l.chip_count() - req);
                    inv.use_item(entity_id::computer_chip, req);
                            
                    clean_entity(self);
                    self.id(entity_id::floor);
                }
            };
                
            e << method(collides_, common::always_collides_)
              << method(on_collision_, socket_on_col);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_toggle_wall(entity & e, level &)
        {
            REQUIRE_CONCEPT(e, EntityHas<tile_id>);
            
            auto collides =
            [](entity const & self, entity const & other)
            {
                return self.get<tile_id>() == tile_id::toggle_wall_closed
                    && same_position(self, other);
            };
            
            e << method(collides_, collides);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_blue_wall(entity & e, level &)
        {
            auto real_on_col =
            [](entity & self, entity & other, level &)
            {
                if (other && is_chip(other))
                {
                    self.clear_methods();
                    self.remove<tile_id>();
                    self.id(entity_id::wall);
                    self << method(collides_, common::always_collides_);
                }
            };
                
            auto fake_on_col =
            [](entity & self, entity & other, level & l)
            {
                if (other && is_chip(other))
                {
                    clean_entity(self);
                    self.id(entity_id::floor);
                    init(self, l);
                }
            };
                
            if (e.get<tile_id>() == tile_id::blue_wall_fake)
            {
                e << method(on_collision_, fake_on_col)
                  << method(collides_, common::always_collides_);
            }
            else
            {
                e << method(on_collision_, real_on_col)
                  << method(collides_, common::always_collides_);
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_clone_machine(entity & e, level &)
        {
            // TODO
            auto clone =
            [](entity &, level &)
            {
                
            };
            
            e << method(collides_, common::always_collides_)
              << method(clone_, clone);
        }
        
#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
        ////////////////////////////////////////////////////////////////////////
        void init_wall(entity & e, level & l)
        {
            ELIB_ASSERT(is_wall(e));
            switch (e.id())
            {
                case entity_id::blue_lock:
                case entity_id::red_lock:
                case entity_id::green_lock:
                case entity_id::yellow_lock:
                    init_lock(e, l);
                    break;
                case entity_id::socket:
                    init_socket(e, l);
                    break;
                case entity_id::toggle_wall:
                    init_toggle_wall(e, l);
                    break;
                case entity_id::blue_wall:
                    init_blue_wall(e, l);
                    break;
                case entity_id::clone_machine:
                    init_clone_machine(e, l);
                    break;
                default:
                    e << method(collides_, common::always_collides_);
                    break;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_floor(entity & e, level & l)
        {
            ELIB_ASSERT(is_floor(e));
            switch (e.id())
            {
                case entity_id::exit:
                    init_exit(e, l);
                    break;
                case entity_id::water:
                    init_water(e, l);
                    break;
                case entity_id::fire:
                    init_fire(e, l);
                    break;
                case entity_id::green_button:
                    init_green_button(e, l);
                    break;
                case entity_id::blue_button:
                    init_blue_button(e, l);
                    break;
                case entity_id::brown_button:
                    init_brown_button(e, l);
                    break;
                case entity_id::red_button:
                    init_red_button(e, l);
                    break;
                case entity_id::trap:
                    init_trap(e, l);
                    break;
                case entity_id::bomb:
                    init_bomb(e, l);
                    break;
                case entity_id::gravel:
                    init_gravel(e, l);
                    break;
                case entity_id::dirt:
                    init_dirt(e, l);
                    break;
                default:
                    break;
            }
        }
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

    }                                                       // namespace
    
    ////////////////////////////////////////////////////////////////////////////
    void init_base(entity & e, level & l)
    {
        if      (is_floor(e)) init_floor(e, l);
        else if (is_wall(e))  init_wall(e, l);
    }
    
    void process_base(entity &, level &)
    {
    }
    
    void finalize_base(entity &, level &)
    {
    }
}}                                                          // namespace chips