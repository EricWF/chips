#ifndef CHIPS_CORE_CONFIG_HPP
#define CHIPS_CORE_CONFIG_HPP

# include <vector>
# include <string>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               CONFIG
////////////////////////////////////////////////////////////////////////////////
    
    /// The directory where all resources are held
    /// CHIPS_RESOURCE_ROOT must be defined by the build.
    constexpr const char resource_root[] = CHIPS_RESOURCE_ROOT;
    
    /// Information about the textures/sprites
    constexpr const char default_tileset_file[] = "pirate_tileset.png";
    constexpr const char tileset_info_file[] = CHIPS_RESOURCE_ROOT "/default_tileset.tsx";
    constexpr const unsigned tile_width = 32;
    constexpr const unsigned tile_height = 32;
    constexpr const unsigned texture_rows = 16;
    constexpr const unsigned texture_cols = 8;
    constexpr const unsigned texture_width = tile_width * texture_cols;
    constexpr const unsigned texture_height = tile_height * texture_rows;
    constexpr const unsigned num_tiles = texture_rows * texture_cols;
    
    /// Information about the levels
    constexpr const char level_file_root[] = CHIPS_RESOURCE_ROOT "/levels";
    constexpr const unsigned level_width = 32;
    constexpr const unsigned level_height = 32;
    constexpr const unsigned level_size = level_width * level_height;
    constexpr const unsigned level_view_width = 9;
    constexpr const unsigned level_view_height = 9;
    
    /// Information about the part of the window the game is displayed in
    constexpr const unsigned level_window_xpos = 25;
    constexpr const unsigned level_window_ypos = 25;
    constexpr const unsigned level_window_width = level_view_width * tile_width;
    constexpr const unsigned level_window_height = level_view_height * tile_height;
    
    constexpr const unsigned scoreboard_xpos = level_window_width + level_window_xpos + 50;
    constexpr const unsigned scoreboard_ypos = level_window_ypos;
    constexpr const unsigned scoreboard_width = (25 * 2) + (4 * tile_width);
    constexpr const unsigned scoreboard_height = level_window_height;
    
    constexpr const unsigned helptext_xpos = level_window_xpos;
    constexpr const unsigned helptext_ypos = level_window_ypos
                                           + level_window_height + 30;
    
    constexpr const unsigned inventory_xpos = scoreboard_xpos + 25;
    constexpr const unsigned inventory_ypos = 
        (scoreboard_ypos + scoreboard_height) - (2 * tile_height) - 25;
        
    constexpr const unsigned chip_count_xpos = scoreboard_xpos + 25;
    constexpr const unsigned chip_count_ypos = scoreboard_ypos + 10;
        
    /// Information about the top level window
    constexpr const char window_name[] = "chips";
    constexpr const unsigned window_xpos = 100;
    constexpr const unsigned window_ypos = 100;
    constexpr const unsigned window_width = level_window_width + 300 + 20;
    constexpr const unsigned window_height = level_window_height + 100 + 20;
    
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wglobal-constructors"
#   pragma clang diagnostic ignored "-Wexit-time-destructors"
# endif
    const std::vector<std::string> level_list = 
        {
            "1", "2", "3", "4", "5", "6", "7", "8"
        };
#if defined(__clang__)
#   pragma clang diagnostic pop
# endif
}                                                           // namespace chips
#endif /* CHIPS_CORE_CONFIG_HPP */
