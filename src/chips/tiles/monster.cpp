#include "chips/tiles/monster.hpp"
#include "chips/visitors/tile_visitor.hpp"

namespace chips
{
    void monster::accept_visit(tile_visitor & v)
    {
        v.visit(*this);
    }
}                                                           // namespace chips