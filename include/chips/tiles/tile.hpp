#ifndef CHIPS_TILES_TILE_HPP
#define CHIPS_TILES_TILE_HPP

# include "chips/tiles/fwd.hpp"
# include "chips/position.hpp"

namespace chips
{
    class tile_visitor;
    
    class tile
    {
    public:
        explicit tile(chips::texture_id);
        tile(chips::texture_id, chips::position pos);
        
        tile(chips::tile_id id1, chips::texture_id id2)
          : id{id1}, tex_id{id2}, pos{}
        {}
        
        tile(chips::tile_id id1, chips::texture_id id2, chips::position xpos)
          : id{id1}, tex_id{id2}, pos{xpos}
        {}
        
        tile(tile const &) = default;
        tile(tile &&) = default;
        tile & operator=(tile const &) = default;
        tile & operator=(tile &&) = default;
        
        virtual void accept_visit(tile_visitor *visitor) = 0;
        
        virtual ~tile() noexcept = default;

        chips::tile_id id;
        chips::texture_id tex_id;
        chips::position pos;
    };
}                                                           // namespace chipl
#endif /* CHIPS_TILES_TILE_HPP */