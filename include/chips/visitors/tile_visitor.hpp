#ifndef CHIPS_VISITORS_TILE_VISITOR_HPP
#define CHIPS_VISITORS_TILE_VISITOR_HPP

# include "chips/visitors/fwd.hpp"
# include "chips/tiles/fwd.hpp"
# include <elib/aux.hpp>

namespace chips
{
    class tile_visitor
    {
    public:
        ELIB_DEFAULT_CLASS(tile_visitor);
        virtual ~tile_visitor() noexcept = default;
        
        virtual void visit(tile &) = 0;
        virtual void visit(chip &) = 0;
        virtual void visit(floor &) = 0;
        virtual void visit(wall &) = 0;
        virtual void visit(invisible_wall &) = 0;
        virtual void visit(thin_wall &) = 0;
        virtual void visit(block &) = 0;
        virtual void visit(blue_block &) = 0;
        virtual void visit(water &) = 0;
        virtual void visit(fire &) = 0;
        virtual void visit(ice &) = 0;
        virtual void visit(force_floor &) = 0;
        virtual void visit(computer_chip &) = 0;
        virtual void visit(flippers &) = 0;
        virtual void visit(fire_boots &) = 0;
        virtual void visit(skates &) = 0;
        virtual void visit(suction_boots &) = 0;
        //TODO More tile types
    };
}                                                           // namespace chips
#endif /* CHIPS_VISITORS_TILE_VISITOR_HPP */