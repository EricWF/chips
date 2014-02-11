#include "chips/visitors/void_visitor.hpp"
#include "chips/tiles.hpp"

namespace chips
{
    void void_visitor::visit(chip &) {}
    void void_visitor::visit(floor &) {}
    void void_visitor::visit(wall &) {}
    void void_visitor::visit(invisible_wall &) {}
    void void_visitor::visit(thin_wall &) {}
    void void_visitor::visit(block &) {}
    void void_visitor::visit(dirt &) {}
    void void_visitor::visit(blue_block &) {}
    void void_visitor::visit(water &) {}
    void void_visitor::visit(fire &) {}
    void void_visitor::visit(ice &) {}
    void void_visitor::visit(force_floor &) {}
    void void_visitor::visit(computer_chip &) {}
    void void_visitor::visit(flippers &) {}
    void void_visitor::visit(fire_boots &) {}
    void void_visitor::visit(skates &) {}
    void void_visitor::visit(suction_boots &) {}
    void void_visitor::visit(key &) {}
    void void_visitor::visit(lock &) {}
    void void_visitor::visit(exit &) {}
    void void_visitor::visit(socket &) {}
    void void_visitor::visit(green_button &) {}
    void void_visitor::visit(red_button &) {}
    void void_visitor::visit(brown_button &) {}
    void void_visitor::visit(blue_button &) {}
    void void_visitor::visit(teleport &) {}
    void void_visitor::visit(bomb &) {}
    void void_visitor::visit(trap &) {}
    void void_visitor::visit(gravel &) {}
    void void_visitor::visit(pop_up_wall &) {}
    void void_visitor::visit(hint &) {}
    void void_visitor::visit(clone_machine &) {}
    void void_visitor::visit(fake_exit &) {}
    void void_visitor::visit(monster &) {}
    void void_visitor::visit(bug &) {}
    void void_visitor::visit(fireball &) {}
    void void_visitor::visit(pink_ball &) {}
    void void_visitor::visit(tank &) {}
    void void_visitor::visit(glider &) {}
    void void_visitor::visit(teeth &) {}
    void void_visitor::visit(walker &) {}
    void void_visitor::visit(blob &) {}
    void void_visitor::visit(germ &) {}
}