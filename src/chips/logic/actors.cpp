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
                [](entity & self, level & lev)
                {
                    ELIB_ASSERT(self);
                    
                    position old_pos;
                    direction old_dir;
                    self >> old_pos >> old_dir;
                
                    self(move_, old_dir, lev);
                    
                    if (self.get<position>() == old_pos)
                        self(move_, turn_clockwise(old_dir), lev);
                        
                    auto AgainstWallLeft = Concept<Alive, EntityMatches<&is_wall>>(
                        AtPosition(move(
                            self.get<position>()
                          , turn_counter_clockwise(self.get<direction>())
                          , 1
                          ))
                      );
                    
                    bool has_wall_left = false;
                    for (auto & fe : AgainstWallLeft.filter(lev.entity_list))
                    {
                        ((void)fe);
                        has_wall_left = true;
                    }
                
                    if (!has_wall_left)
                    {
                        self << turn_counter_clockwise(self.get<direction>());
                    }
                    
                };
                
                e << method(update_, bug_update_);
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