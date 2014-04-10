#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/sounds.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips { namespace logic
{
    ////////////////////////////////////////////////////////////////////////////
    void init_chip(entity & e, level &)
    {
        ELIB_ASSERT(is_chip(e));
        e.remove<tile_id>();
        
        e.on_death([](entity &){ chip_die_sound(); });
        
        e << inventory() 
          << method(move_, common::move_);
    }
    
    void process_chip(entity &, level &)
    {
    }
    
    void finalize_chip(entity &, level &)
    {
    }
}}                                                          // namespace chips