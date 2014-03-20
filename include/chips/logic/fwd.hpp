#ifndef CHIPS_LOGIC_FWD_HPP
#define CHIPS_LOGIC_FWD_HPP

# include "chips/core.hpp"
# include "chips/entity.hpp"
# include "chips/game.hpp"
# include <elib/aux.hpp>

namespace chips { namespace logic
{
    void init(entity & e, level & l);
    void process(entity & e, level & l);
    void finalize(entity & e, level & l);
    
    void init_chip(entity &, level &);
    void init_actor(entity &, level &);
    void init_item(entity &, level &);
    void init_base(entity &, level &);
    
    void process_chip(entity &, level &);
    void process_actor(entity &, level &);
    void process_item(entity &, level &);
    void process_base(entity &, level &);
    
    void finalize_chip(entity &, level &);
    void finalize_actor(entity &, level &);
    void finalize_item(entity &, level &);
    void finalize_base(entity &, level &);
}}                                                          // namespace chips
#endif /* CHIPS_LOGIC_FWD_HPP */