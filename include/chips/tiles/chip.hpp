#ifndef CHIPS_TILES_CHIP_HPP
#define CHIPS_TILES_CHIP_HPP

# include "chips/tiles/fwd.hpp"
# include "chips/tiles/tile.hpp"
# include "chips/visitors/fwd.hpp"

namespace chips
{
    class chip 
      : public tile
    {
    public:
        using tile::tile;
        
        virtual void accept_visit(tile_visitor & v);
        
        virtual ~chip() noexcept = default;
        
        chip_state state{chip_state::normal};
    };
}                                                           // namespace chips
#endif /* CHIPS_TILES_CHIP_HPP */