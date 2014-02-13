#ifndef CHIPS_LEVEL_HPP
#define CHIPS_LEVEL_HPP

# include "chips/fwd.hpp"
# include "chips/position.hpp"
# include "chips/tiles/fwd.hpp"
# include <elib/CXX14/memory.hpp>
# include <array>
# include <string>

// TODO Make files plaintext readable 
namespace chips
{
    // fwd
    class game_grid;
    
    struct level_tile
    {
        tile_id id;
        texture_id tex_id;
        position pos;
        direction dir;
    };
    
    // Representation of a level on disk;
    struct level
    {
        unsigned level_id;
        level_tile chip_tile;
        std::array<level_tile, level_size> base;
        std::array<level_tile, level_size> entities;
    };
    
    level read_level(std::string const & filename);
    void write_level(std::string const & filename, level const & l);
    
    std::unique_ptr<game_grid> load_level(level const &);
    std::unique_ptr<tile>      load_tile(level_tile const &);
    std::unique_ptr<chip>      load_chip(level_tile const &);
    
}                                                           // namespace chips
#endif /* CHIPS_LEVEL_HPP */