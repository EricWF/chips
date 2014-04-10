#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"

#include <random>

namespace chips
{
    void clean_entity(entity & e)
    {
        if (e.has<position>())
        {
            position p = e.get<position>();
            e.clear();
            e <<  p;
        }
    }

    bool same_position(entity const & lhs, entity const & rhs)
    {
        if (!lhs || !rhs || &lhs == &rhs)
            return false;
        return lhs.get<position>() == rhs.get<position>();
    }
    
    bool at_location(entity_locator loc, entity const & e)
    {
        return e 
          && e.id() == loc.id
          && e.get<position>() == loc.pos;
    }
    
}                                                           // namespace chips
namespace chips { namespace logic
{
    
    void init(entity & e, level & l)
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
    
    void process(entity & e, level & l)
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

    void finalize(entity & e, level & l)
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
    
    direction move_on_ice(entity & e, entity const & ice, level & l)
    {
        tile_id tid = ice.get<tile_id>();
        direction d = e.get<direction>();

        direction move_dir = direction::N;
        
        if (tid == tile_id::ice_NW) {
            if (d == direction::N) {
                move_dir = direction::E;
            } else {
                move_dir = direction::S;
            }
        }
        else if (tid == tile_id::ice_NE) {
            if (d == direction::N) {
                move_dir = direction::W;
            } else {
                move_dir = direction::S;
            }
        }
        else if (tid == tile_id::ice_SW) {
            if (d == direction::S) {
                move_dir = direction::E;
            } else {
                move_dir = direction::N;
            }
        }
        else if (tid == tile_id::ice_SE) {
            if (d == direction::S) {
                move_dir = direction::W;
            } else {
                move_dir = direction::N;
            }
        }
        else {
            move_dir = e.get<direction>();
        }
        
        e(move_, move_dir, l);
        return move_dir;
    }
    
    namespace 
    {
        /// The engine of choice for randomness in llama
        using random_engine = std::mt19937;
        
        /// Access to a statically allocated random_engine.
        /// TODO: this is not thread safe
        inline random_engine & get_rand_engine()
        {
            static random_engine eng((std::random_device()()));
            return eng;
        }
    }                                                       // namespace 
    
    ////////////////////////////////////////////////////////////////////////////
    direction move_on_force_floor(entity & e, entity const & ff, level & l)
    {
        tile_id id = ff.get<tile_id>();
        if (id == tile_id::force_floor_N) {
            e(move_, direction::N, l);
            return direction::N;
        }
        else if (id == tile_id::force_floor_S) {
            e(move_, direction::S, l);
            return direction::S;
        } 
        else if (id == tile_id::force_floor_E) {
            e(move_, direction::E, l);
            return direction::E;
        }
        else if (id == tile_id::force_floor_W) {
            e(move_, direction::W, l); 
            return direction::W;
        }
        else if (id == tile_id::force_floor_random) {
            std::uniform_int_distribution<int> rand_dist(0, 3);
            int r = rand_dist(get_rand_engine());
            direction d = static_cast<direction>(r);
            e(move_, d, l);
            return d;
        } else {
            ELIB_ASSERT(false);
            throw "TODO";
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool reading_order_cmp::operator()(
        entity const & lhs, entity const & rhs
      ) const
    {
        if (!rhs) return false;
        if (!lhs) return true;

        position lp, rp;
        lhs >> lp;
        rhs >> rp;

        if (lp.y > rp.y)
            return true;
        if (lp.y < rp.y)
            return false;
        if (lp.x > rp.x)
            return true;
        if (lp.x < rp.x)
            return false;
        return false;
    }
    
    bool reverse_reading_order_cmp::operator()(entity const & lhs, entity const & rhs) const
    {
        if (!rhs) return false;
        if (!lhs) return true;

        position lp, rp;
        lhs >> lp;
        rhs >> rp;

        if (lp.y < rp.y)
            return true;
        if (lp.y > rp.y)
            return false;
        if (lp.x < rp.x)
            return true;
        if (lp.x > rp.x)
            return false;
        return false;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    chips_state get_chips_state(level const & l)
    {
        entity const & chip = l.chip;
        auto const & el = l.entity_list;
        
        if (not chip) {
            chips_state const *opt_st = chip.get_raw<chips_state>();
            if (opt_st) return *opt_st;
            return chips_state::normal;
        }
        
        ELIB_ASSERT(!chip.has<chips_state>());
        
        if (OnWater(chip).contains(el)) {
            return chips_state::swimming;
        }
      
        return chips_state::normal;
    }
}}                                                           // namespace chips