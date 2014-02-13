#ifndef CHIPS_TILES_TILE_HPP
#define CHIPS_TILES_TILE_HPP

# include "chips/tiles/fwd.hpp"
# include "chips/visitors/fwd.hpp"
# include "chips/position.hpp"
# include <cstddef>

namespace chips
{    
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wweak-vtables"
# endif
    class tile
    {
    public:
        explicit tile(texture_id);
        tile(texture_id, position pos);
        
        tile(tile_id id1, texture_id id2)
          : id{id1}, tex_id{id2}, pos()
        {}
        
        tile(tile_id id1, texture_id id2, position xpos)
          : id{id1}, tex_id{id2}, pos(xpos)
        {}
        
        tile(tile const &) = default;
        tile(tile &&) = default;
        tile & operator=(tile const &) = default;
        tile & operator=(tile &&) = default;
        
        virtual void accept_visit(tile_visitor & visitor) = 0;
        
        virtual ~tile() noexcept = default;

        tile_id id;
        texture_id tex_id;
        position pos;
    };
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif
}                                                           // namespace chipl
#endif /* CHIPS_TILES_TILE_HPP */