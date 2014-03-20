#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>

namespace chips { namespace logic
{
    namespace 
    {
        void init_monster(entity & e, level &)
        {
            ELIB_ASSERT(is_monster(e));
            REQUIRE_CONCEPT(e, Directional);
            e.remove<tile_id>();
            // TODO remove this
            // add a default velocity if none is present.
            if (!e.has<velocity>())
                e << velocity(1);
                
            auto kill_chip_on_col =
            [](entity &, entity & other, level &)
            {
                if (is_chip(other))
                    other.kill();
            };
                
            e << method(move_, common::move_)
              << texture_type::cutout
              << method(collides_, common::always_collides_)
              << method(on_collision_, kill_chip_on_col);
              
            if (e.id() == entity_id::bug)
            {
                auto bug_update_ =
                [](entity & self, level & lv)
                {
                  ELIB_ASSERT(self);
                  position p; direction d;
                  self >> p >> d;
                  self(move_, d, lv);
                  for (;;)
                  {
                    if (self.get<position>() != p) break;
                
                    self(move_, turn_counter_clockwise(d), lv);
                    if (self.get<position>() != p) break;
                    self(move_, turn_around(d), lv);
                    if (self.get<position>() != p) break;
                    self(move_, turn_clockwise(d), lv);
                    break;
                  }
                
                  auto HasWallLeft = Concept<EntityMatches<&is_wall>>(
                    AtPosition(move(
                        self.get<position>()
                      , turn_counter_clockwise(self.get<direction>())
                      , 1
                   )));
                
                  if (!HasWallLeft.contains(lv.entity_list))
                    self << turn_counter_clockwise(self.get<direction>());
                };
                
                e << method(update_, bug_update_);
            }
            else if (e.id() == entity_id::tank)
            {
                auto tank_update =
                [](entity & self, level & l)
                {
                    if (self.get<toggle_state>())
                    {
                        self << turn_around(self.get<direction>());
                        self << toggle_state(false);
                    }
                    
                    self(move_, self.get<direction>(), l);
                };
                
                e << method(update_, tank_update);
            }
            else if (e.id() == entity_id::fireball)
            {
                auto fireball_update =
                [](entity & self, level & l)
                {
                    position p;
                    direction d;
                    self >> p >> d;
                    self(move_, d, l);
                    if (self.get<position>() == p)
                    {
                      self(move_, turn_clockwise(d), l);
                    }
                };
                
                e << method(update_, fireball_update);
            }
            else if (e.id() == entity_id::glider)
            {
              auto glider_update =
              [](entity & self, level & l)
              {
                ELIB_ASSERT(self);
                position p; direction d;
                self >> p >> d;
                self(move_, d, l);
                if (self.get<position>() != p) return;
                self(move_, turn_counter_clockwise(d), l);
                if (self.get<position>() != p) return;
                self(move_, turn_clockwise(d), l);
                if (self.get<position>() != p) return;
                self(move_, turn_around(d), l);
              };
              
              e << method(update_, glider_update);
            }
            
        }
        
        void init_block(entity & e, level &)
        {
            ELIB_ASSERT(e && e.id() == entity_id::block);
            
            auto block_on_collide =
            [](entity & self, entity & other, level & lev)
            {
                if (!is_chip(other)) return;
                self(move_, other.get<direction>(), lev);
                self.remove<direction>();
            };
            
            e << method(move_, common::move_)
              << method(on_collision_, block_on_collide)
              << method(collides_, common::always_collides_);
        }
    }                                                       // namespace
    
    void init_actor(entity & e, level & l)
    {
        if (is_monster(e))
            init_monster(e, l);
        else if (e.id() == entity_id::block)
            init_block(e, l);
    }
    
    void process_actor(entity &, level &)
    {
    }
    
    void finalize_actor(entity &, level &)
    {
    }
}}                                                          // namespace chips