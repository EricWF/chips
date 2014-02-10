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
        
        tile(chips::tile_id, chips::texture_id)
          : m_tile_id{id1}, m_texture_id{id2}, m_position{}
        {}
        
        tile(chips::tile_id id1, chips::texture_id id2, chips::position pos)
          : m_tile_id{id1}, m_texture_id{id2}, m_position{pos}
        {}
        
        tile(tile const &) = default;
        tile(tile &&) = default;
        tile & operator=(tile const &) = default;
        tile & operator=(tile &&) = default;
        
        chips::tile_id tile_id() const noexcept { return m_tile_id; }
        chips::tile_id & tile_id() noexcept { return m_tile_id; }
        
        chips::texture_id texture_id() const noexcept { return m_texture_id; }
        chips::texture_id & texture_id() noexcept { return m_texture_id; }
        
        chips::position position() const noexcept { return m_position; }
        chips::position & position() noexcept { return m_position; }
        
        virtual void accept_visit(tile_visitor *visitor) = 0;
        
        virtual ~tile() noexcept = default;
    private:
        chips::tile_id m_tile_id;
        chips::texture_id m_texture_id;
        chips::position m_position;
    };
}                                                           // namespace chipl
#endif /* CHIPS_TILES_TILE_HPP */