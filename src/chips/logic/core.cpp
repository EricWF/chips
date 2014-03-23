#include "chips/logic/core.hpp"

namespace chips { namespace logic
{
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
    
    bool reading_order_cmp::operator()(entity const & lhs, entity const & rhs) const
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
}}                                                          // namespace chips