#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>

namespace chips { namespace logic
{
    namespace 
    {
        void kill_chip_on_col(entity &, entity & other, level &)
        {
            if (is_chip(other)) other.kill();
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_block(entity & e, level &)
        {
            auto block_on_collide =
            [](entity & self, entity & other, level & lev)
            {
                if (!is_chip(other) || self.has<move_lock>())
                    return;
                self(move_, other.get<direction>(), lev);
                self.remove<direction>();
            };
            
            e << method(move_, common::move_)
              << method(on_collision_, block_on_collide)
              << method(collides_, common::always_collides_);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void init_bug(entity & e, level &)
        {
            auto bug_update_ =
            [](entity & self, level & lv)
            {
                ELIB_ASSERT(self);
                
                if (self.has<move_lock>()) 
                    return;
                    
                position p; direction d;
                self >> p >> d;
             
                auto & el  = lv.entity_list;
                bool front = ColFront(self).contains(el);
                bool back  = ColBack(self).contains(el);
                bool left  = ColLeft(self).contains(el);
                bool right = ColRight(self).contains(el);
                
                if (!left)       self(move_, turn_left(d), lv);
                else if (!front) self(move_, d, lv);
                else if (!right) self(move_, turn_right(d), lv);
                else if (!back)  self(move_, turn_around(d), lv);
            };
        
            e << method(update_, bug_update_);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_tank(entity & e, level &)
        {
            auto tank_update =
            [](entity & self, level & l)
            {
                if (self.has<move_lock>()) return;
                self(move_, self.get<direction>(), l);
            };
                
            e << method(update_, tank_update);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_fireball(entity & e, level &)
        {
            auto fireball_update =
            [](entity & self, level & l)
            {
                if (self.has<move_lock>()) return;
                position p; direction d;
                self >> p >> d;
                self(move_, d, l);
                if (self.get<position>() == p)
                {
                    self(move_, turn_right(d), l);
                }
            };
                
            e << method(update_, fireball_update);
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_glider(entity & e, level &)
        {
            auto glider_update =
            [](entity & self, level & l)
            {
                ELIB_ASSERT(self);
                if (self.has<move_lock>()) return;
                position p; direction d;
                self >> p >> d;
                self(move_, d, l);
                if (self.get<position>() != p) return;
                self(move_, turn_left(d), l);
                if (self.get<position>() != p) return;
                self(move_, turn_right(d), l);
                if (self.get<position>() != p) return;
                self(move_, turn_around(d), l);
            };
            e << method(update_, glider_update);
        }
        
        void init_germ(entity & e, level &)
        {
            auto germ_update =
            [](entity & self, level & lv)
            {
                if (self.has<move_lock>())
                    return;
                position p; direction d;
                self >> p >> d;

                auto & el = lv.entity_list;
                bool front  = ColFront(self).contains(el);
                bool behind = ColBack(self).contains(el);
                bool left   = ColLeft(self).contains(el);
                bool right  = ColRight(self).contains(el);
                
                if (!right)       self(move_, turn_right(d), lv);
                else if (!front)  self(move_, d, lv);
                else if (!left)   self(move_, turn_left(d), lv);
                else if (!behind) self(move_, turn_around(d), lv);
            };
            
            e << method(update_, germ_update);
        }
        
#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
        void init_monster(entity & e, level & l)
        {
            ELIB_ASSERT(is_monster(e));
            e.remove<tile_id>();
            
            e << method(move_, common::move_)
              << texture_type::cutout
              << method(collides_, common::always_collides_)
              << method(on_collision_, kill_chip_on_col);
              
            switch (e.id())
            {
                case entity_id::bug:
                    init_bug(e, l); 
                    break;
                case entity_id::fireball:
                    init_fireball(e, l);
                    break;
                case entity_id::pink_ball:
                    // TODO
                    break;
                case entity_id::tank:
                    init_tank(e, l);
                    break;
                case entity_id::glider:
                    init_glider(e, l);
                    break;
                case entity_id::teeth:
                    // TODO
                    break;
                case entity_id::walker:
                    // TODO
                    break;
                case entity_id::blob:
                    // TODO
                    break;
                case entity_id::germ:
                    init_germ(e, l);
                    break;
                default:
                    ELIB_ASSERT(false && "In default case");
            }
        }
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

    }                                                       // namespace
    
    void init_actor(entity & e, level & l)
    {
        if (is_monster(e))
            init_monster(e, l);
        else if (e.id() == entity_id::block)
            init_block(e, l);
        else
            log::warn("In default case for init_actor");
    }
    
    void process_actor(entity &, level &)
    {
    }
    
    void finalize_actor(entity &, level &)
    {
    }
}}                                                          // namespace chips