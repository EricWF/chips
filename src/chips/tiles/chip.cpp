#include "chips/tiles/chip.hpp"
#include "chips/visitors/tile_visitor.hpp"

namespace chips
{
    void chip::accept_visit(tile_visitor & v)
    {
        v.visit(*this);
    }
}                                                           // namespace chips