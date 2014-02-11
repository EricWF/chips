#ifndef CHIPS_VISITORS_VOID_VISITOR_HPP
#define CHIPS_VISITORS_VOID_VISITOR_HPP

# include "chips/visitors/fwd.hpp"
# include "chips/visitors/tile_visitor.hpp"
# include "chips/tiles/fwd.hpp"

namespace chips
{
    class void_visitor
      : public tile_visitor
    {
    public:
        ELIB_DEFAULT_CLASS(void_visitor);
        virtual ~void_visitor() noexcept = default;
        
        virtual void visit(chip &);
        virtual void visit(floor &);
        virtual void visit(wall &);
        virtual void visit(invisible_wall &);
        virtual void visit(thin_wall &);
        virtual void visit(block &);
        virtual void visit(dirt &);
        virtual void visit(blue_block &);
        virtual void visit(water &);
        virtual void visit(fire &);
        virtual void visit(ice &);
        virtual void visit(force_floor &);
        virtual void visit(computer_chip &);
        virtual void visit(flippers &);
        virtual void visit(fire_boots &);
        virtual void visit(skates &);
        virtual void visit(suction_boots &);
        virtual void visit(key &);
        virtual void visit(lock &);
        virtual void visit(exit &);
        virtual void visit(socket &);
        virtual void visit(green_button &);
        virtual void visit(red_button &);
        virtual void visit(brown_button &);
        virtual void visit(blue_button &);
        virtual void visit(teleport &);
        virtual void visit(bomb &);
        virtual void visit(trap &);
        virtual void visit(gravel &);
        virtual void visit(pop_up_wall &);
        virtual void visit(hint &);
        virtual void visit(clone_machine &);
        virtual void visit(fake_exit &);
        virtual void visit(monster &);
        virtual void visit(bug &);
        virtual void visit(fireball &);
        virtual void visit(pink_ball &);
        virtual void visit(tank &);
        virtual void visit(glider &);
        virtual void visit(teeth &);
        virtual void visit(walker &);
        virtual void visit(blob &);
        virtual void visit(germ &);
    };
}                                                           // namespace chips
#endif /* CHIPS_VISITORS_VOID_VISITOR_HPP */