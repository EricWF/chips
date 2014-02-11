#ifndef CHIPS_VISITORS_TILE_VISITOR_HPP
#define CHIPS_VISITORS_TILE_VISITOR_HPP

# include "chips/visitors/fwd.hpp"
# include "chips/tiles/fwd.hpp"
# include <elib/aux.hpp>

namespace chips
{
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wweak-vtables"
# endif
    class tile_visitor
    {
    public:
        ELIB_DEFAULT_CLASS(tile_visitor);
        virtual ~tile_visitor() noexcept = default;
        
        virtual void visit(chip &) = 0;
        virtual void visit(floor &) = 0;
        virtual void visit(wall &) = 0;
        virtual void visit(invisible_wall &) = 0;
        virtual void visit(thin_wall &) = 0;
        virtual void visit(block &) = 0;
        virtual void visit(dirt &) = 0;
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
        virtual void visit(key &) = 0;
        virtual void visit(lock &) = 0;
        virtual void visit(exit &) = 0;
        virtual void visit(socket &) = 0;
        virtual void visit(green_button &) = 0;
        virtual void visit(red_button &) = 0;
        virtual void visit(brown_button &) = 0;
        virtual void visit(blue_button &) = 0;
        virtual void visit(teleport &) = 0;
        virtual void visit(bomb &) = 0;
        virtual void visit(trap &) = 0;
        virtual void visit(gravel &) = 0;
        virtual void visit(pop_up_wall &) = 0;
        virtual void visit(hint &) = 0;
        virtual void visit(clone_machine &) = 0;
        virtual void visit(fake_exit &) = 0;
        virtual void visit(monster &) = 0;
        virtual void visit(bug &) = 0;
        virtual void visit(fireball &) = 0;
        virtual void visit(pink_ball &) = 0;
        virtual void visit(tank &) = 0;
        virtual void visit(glider &) = 0;
        virtual void visit(teeth &) = 0;
        virtual void visit(walker &) = 0;
        virtual void visit(blob &) = 0;
        virtual void visit(germ &) = 0;
    };
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif 
}                                                           // namespace chips
#endif /* CHIPS_VISITORS_TILE_VISITOR_HPP */