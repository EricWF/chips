#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include <elib/aux.hpp>

namespace chips { namespace logic
{
    namespace 
    {
        void kill_chip_on_col(entity & self, entity & other, level &)
        {
            if (is_chip(other)) {
                other.kill();
                if (self.id() == entity_id::fireball) {
                    other << chips_state::burned_smoke;
                }
            }
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
        
            e << method(update_, bug_update_)
              << regular_speed;
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
                
            e << method(update_, tank_update)
              << regular_speed;
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_fireball(entity & e, level &)
        {
            auto fireball_update =
            [](entity & self, level & l)
            {
               ELIB_ASSERT(self);
                
                if (self.has<move_lock>()) 
                    return;
                    
                position p; direction d;
                self >> p >> d;
             
                auto & el  = l.entity_list;
                bool front = ColFront(self).contains(el);
                bool back  = ColBack(self).contains(el);
                bool left  = ColLeft(self).contains(el);
                bool right = ColRight(self).contains(el);
                
                if (!front)
                    self(move_, d, l);
                else if (!right)
                    self(move_, turn_right(d), l);
                else if (!left)
                    self(move_, turn_left(d), l);
                else if (!back)
                    self(move_, turn_around(d), l);
            };
                
            e << method(update_, fireball_update)
              << regular_speed;
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_pink_ball(entity & e, level &)
        {
            auto update =
            [](entity & self, level & l)
            {
                bool front = ColFront(self).contains(l.entity_list);
                bool back = ColBack(self).contains(l.entity_list);
                if (!front)
                    self(move_, self.get<direction>(), l);
                else if (!back)
                    self(move_, turn_around(self.get<direction>()), l);
            };
            
            e << method(update_, update)
              << regular_speed;
        }
        
        ////////////////////////////////////////////////////////////////////////
        void init_teeth(entity & e, level &)
        {
            auto teeth_update =
            [](entity & self, level & l)
            {
                if (self.has<move_lock>()) return;
                
                position cp, p;
                l.chip >> cp; self >> p;
                
                direction hdir, vdir;
                hdir = cp.x < p.x ? direction::W : direction::E;
                vdir = cp.y < p.y ? direction::N : direction::S;
                unsigned hmov = cp.x < p.x ? p.x - cp.x : cp.x - p.x;
                unsigned vmov = cp.y < p.y ? p.y - cp.y : cp.y - p.y;
            
                if (hmov == 0 && vmov == 0) return;
                    
                if (vmov >= hmov)
                {
                    self(move_, vdir, l);
                    if (self.get<position>() != p)
                        return;
                }
                
                // virtical move wasnt taken
                if (hmov != 0)
                    self(move_, hdir, l);
             
            };    
            
            e << method(update_, teeth_update)
              << half_speed;
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        void init_glider(entity & e, level &)
        {
            auto glider_update =
            [](entity & self, level & l)
            {
                ELIB_ASSERT(self);
                
                if (self.has<move_lock>()) 
                    return;
                    
                position p; direction d;
                self >> p >> d;
             
                auto & el  = l.entity_list;
                bool front = ColFront(self).contains(el);
                bool back  = ColBack(self).contains(el);
                bool left  = ColLeft(self).contains(el);
                bool right = ColRight(self).contains(el);
                
                if (!front)
                    self(move_, d, l);
                else if (!left)
                    self(move_, turn_left(d), l);
                else if (!right)
                    self(move_, turn_right(d), l);
                else if (!back)
                    self(move_, turn_around(d), l);
            };
            
            e << method(update_, glider_update)
              << full_speed;
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
            
            e << method(update_, germ_update)
              << regular_speed;
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
                    init_pink_ball(e, l);
                    break;
                case entity_id::tank:
                    init_tank(e, l);
                    break;
                case entity_id::glider:
                    init_glider(e, l);
                    break;
                case entity_id::teeth:
                    init_teeth(e, l);
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
}}                                                           // namespace chips