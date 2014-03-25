#include "chips/logic/fwd.hpp"
#include "chips/logic/common.hpp"
#include "chips/core.hpp"

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
    
    void move_on_ice(entity & e, entity const & ice, level & l)
    {
        e << move_lock();
        tile_id tid = ice.get<tile_id>();
        direction d = e.get<direction>();

        if (tid == tile_id::ice_NW)
        {
            if (d == direction::N)
                e(move_, direction::E, l);
            else
                e(move_, direction::S, l);
        }
        else if (tid == tile_id::ice_NE)
        {
            if (d == direction::N) 
                e(move_, direction::W, l);
            else
                e(move_, direction::S, l);
        }
        else if (tid == tile_id::ice_SW)
        {
            if (d == direction::S)
                e(move_, direction::E, l);
            else
                e(move_, direction::N, l);
        }
        else if (tid == tile_id::ice_SE)
        {
            if (d == direction::S)
                e(move_, direction::W, l);
            else
                e(move_, direction::N, l);
        }
        else
        {
            e(move_, e.get<direction>(), l);
        }
        
        if (!OnIce(e).contains(l.entity_list))
            e.remove<move_lock>();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void move_on_force_floor(entity & e, entity const & ff, level & l)
    {
        tile_id id = ff.get<tile_id>();
        if (id == tile_id::force_floor_N)
            e(move_, direction::N, l);
        else if (id == tile_id::force_floor_S)
            e(move_, direction::S, l);
        else if (id == tile_id::force_floor_E)
            e(move_, direction::E, l);
        else if (id == tile_id::force_floor_W)
            e(move_, direction::W, l); 
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
        
        if (OnWater(chip).contains(el))
        {
            if (chip) return chips_state::swimming;
            else      return chips_state::drowned;
        }
        
        if (OnFire(chip).contains(el))
        {
            if (chip) return chips_state::normal;
            else      return chips_state::burned_fire;
        }
        
        if (OnEntity<entity_id::fake_exit>(chip).contains(el))
        {
            return chips_state::fake_exit;
        }
      
        return chips_state::normal;
    }
}}                                                           // namespace chips