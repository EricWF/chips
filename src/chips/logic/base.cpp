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
        void init_element_floor(entity & e, level &)
        {
            
            auto elem_floor_collide =
            [](entity const &, entity const & other)
            {
                    return !(is_chip(other) || other.id() == entity_id::block);
            };
            
            if (e.id() == entity_id::water)
            {
                auto water_on_collide =
                [](entity & self, entity & other, level &)
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
                        
                        auto dirt_on_collide =
                        [](entity & sf, entity & o, level &)
                        {
                            if (o && is_chip(o))
                            {
                                sf.clear_methods();
                                sf.id(entity_id::floor);
                                sf << tile_id::floor;
                            }
                        };
                        
                        other.kill();
                        position tmp_pos = self.get<position>();
                        self.clear();
                        self.id(entity_id::dirt);
                        self << tile_id::dirt << tmp_pos
                             << method(on_collision_, dirt_on_collide)
                             << method(collides_, common::collides_with_monster_);
                            
                        return;
                    }
                };
                
                e << method(collides_, elem_floor_collide)
                  << method(on_collision_, water_on_collide);
            }
            else if (e.id() == entity_id::fire)
            {
                auto fire_on_collide =
                [](entity &, entity & other, level &)
                {
                    if (!is_chip(other)) return;
                    auto & inv  = other.get<inventory>();
                    
                    if (inv.contains(entity_id::fire_boots)) { 
                        other << chips_state::normal;
                    }
                    else {
                        other << chips_state::burned_fire;
                        other.kill();
                    }
                };
                
                e << method(collides_, elem_floor_collide)
                  << method(on_collision_, fire_on_collide);
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_floor(entity & e, level & l)
        {
             ELIB_ASSERT(is_floor(e));
            
            if (entity_id::exit == e.id())
            {
                auto exit_on_col =
                [](entity &, entity & other, level &)
                {
                    if (!is_chip(other)) return;
                    REQUIRE_CONCEPT(other, EntityHas<chip_at_exit>);
                    other.get<chip_at_exit>() = true;
                };
                
                e << method(collides_, common::collides_with_monster_)
                  << method(on_collision_, exit_on_col);
            }
            else if (is_element_floor(e.id()))
            {
                init_element_floor(e, l);
            }
            else if (is_button(e.id()))
            {
                e << method(collides_, common::never_collides_);
                if (e.id() == entity_id::green_button)
                {
                    auto on_col =
                    [](entity & self, entity & other, level & lev)
                    {
                        if (!self || !other || !is_actor(other))
                            return;
                            
                        auto ToggleWalls = Concept<EntityIs<entity_id::toggle_wall>>();
                        
                        for (auto & wall : ToggleWalls.filter(lev.entity_list))
                        {
                            wall << toggle_state(!wall.get<toggle_state>());
                            wall(update_, lev);
                        }
                    };
                    
                    e << method(on_collision_, on_col);
                }
                else if (e.id() == entity_id::blue_button)
                {
                    auto on_col =
                    [](entity & self, entity & other, level & lev)
                    {
                        if (!self || !other || !is_actor(other))
                            return;
                        
                        auto Tanks = Concept<EntityIs<entity_id::tank>>();
                        for (auto & t :  Tanks.filter(lev.entity_list))
                        {
                            t << toggle_state(!t.get<toggle_state>());
                        }
                    };
                    
                    e << method(on_collision_, on_col);
                }
            }
            else if (e.id() == entity_id::gravel)
            {
              auto collides =
              [](entity const &, entity const & other)
              {
                return is_monster(other);
              };
              e << method(collides_, collides);
            }
            else if (e.id() == entity_id::bomb)
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
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_wall(entity & e, level &)
        {
            ELIB_ASSERT(is_wall(e));
           
            if (is_lock(e))
            {
                auto lock_on_col =
                [](entity & self, entity & other, level &)
                {
                    if (!is_chip(other)) return;
                    ELIB_ASSERT(other);
                    auto & inv = other.get<inventory>();
                    entity_id req_key = key_for_lock(self.id());
                    if (inv.contains(req_key))
                    {
                        inv.use_item(req_key);
                        self.clear_methods();
                        self.id(entity_id::floor);
                        self << tile_id::floor
                             << method(collides_, common::never_collides_);
                    }
                };
                
                e.on_death([](entity & self) { self.clear(); });
                e << method(collides_, common::always_collides_)
                  << method(on_collision_, lock_on_col);
            }
            else if (e.id() == entity_id::socket)
            {
                auto socket_on_col =
                [](entity & self, entity & other, level & l)
                {
                    if (!is_chip(other)) return;
                    REQUIRE_CONCEPT(self, EntityHas<sock_chip_count>);
                    REQUIRE_CONCEPT(other, EntityHas<inventory>);
                    
                    auto & inv = other.get<inventory>();
                    unsigned req = self.get<sock_chip_count>();
                    
                    if (req <= inv.count(entity_id::computer_chip))
                    {
                        l.chip_count(l.chip_count() - req);
                        inv.use_item(entity_id::computer_chip, req);
                            
                        self.clear_methods();
                        self.id(entity_id::floor);
                        self << tile_id::floor
                             << method(collides_, common::never_collides_);
                    }
                };
                
                e << method(collides_, common::always_collides_)
                  << method(on_collision_, socket_on_col);
            }
            else if (e.id() == entity_id::toggle_wall)
            {
                auto toggle_wall_update =
                [](entity & self, level &)
                {
                    if (self.get<toggle_state>())
                    {
                        self << tile_id::toggle_wall_open;
                        self << method(collides_, common::never_collides_);
                    }
                    else
                    {
                        self << tile_id::toggle_wall_closed;
                        self << method(collides_, common::always_collides_);
                    }
                };
                
                e << method(update_, toggle_wall_update);
            }
            else if (e.id() == entity_id::blue_wall)
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
                [](entity & self, entity & other, level &)
                {
                    if (other && is_chip(other))
                    {
                      self.clear_methods();
                      self.remove<tile_id>();
                      self.id(entity_id::floor);
                      self << method(collides_, common::never_collides_);
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
            else if (!is_acting_wall(e))
            {
              e << method(collides_, common::always_collides_);
            }
        }
    }                                                       // namespace
    
    ////////////////////////////////////////////////////////////////////////////
    void init_base(entity & e, level & l)
    {
        if (is_floor(e))
            init_floor(e, l);
        else if (is_wall(e)) 
            init_wall(e, l);
    }
    
    void process_base(entity &, level &)
    {
    }
    
    void finalize_base(entity &, level &)
    {
    }
}}                                                          // namespace chips