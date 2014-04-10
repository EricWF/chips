#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/sounds.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips { namespace logic
{
    ////////////////////////////////////////////////////////////////////////////
    void chips_move_(entity & self, direction d, level & l) 
    {
        /// Check to see if the last move was a FF move. If so, only allow
        /// perpindicular movement
        force_floor_move const* opt_move = self.get_raw<force_floor_move>();
        if (opt_move) {
            direction in = *opt_move;
            if ((in == NORTH || in == SOUTH) && (d == NORTH || d == SOUTH)) {
                return;
            }
            else if ((in == EAST || in == WEST) && (d == EAST || d == WEST)) {
                return;
            }
        }
        
        common::move_(self, d, l);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void init_chip(entity & e, level &)
    {
        ELIB_ASSERT(is_chip(e));
        e.remove<tile_id>();
        
        e.on_death([](entity &){ chip_die_sound(); });
        
        e << inventory() 
          << method(move_, chips_move_);
          
    }
    
    void process_chip(entity &, level &)
    {
    }
    
    void finalize_chip(entity &, level &)
    {
    }
}}                                                          // namespace chips