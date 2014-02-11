#ifndef CHIPS_TILES_MONSTER_HPP
#define CHIPS_TILES_MONSTER_HPP

# include "chips/tiles/fwd.hpp"
# include "chips/tiles/tile.hpp"
# include "chips/visitors/tile_visitor.hpp"

namespace chips
{
    class monster : public tile
    {
    public:
        using tile::tile;
        virtual ~monster() noexcept = default;
        
        virtual void accept_visit(tile_visitor &);
    };
}                                                           // namespace chips
#endif /* CHIPS_TILES_MONSTER_HPP */