#ifndef CHIPS_VISITORS_FWD_HPP
#define CHIPS_VISITORS_FWD_HPP

namespace chips
{
    // base class for visitor
    class tile_visitor;
    
    // each visit method does nothing. Allows for overriding of only specific ones
    class void_visitor;
    
    // implementation of visitors 
    class draw_visitor;
    class monster_update_visitor;
    class monster_intersect_visitor;
    class chip_intersect_visitor;
}                                                           // namespace chips
#endif /* CHIPS_VISITORS_FWD_HPP */