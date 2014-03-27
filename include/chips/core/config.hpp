#ifndef CHIPS_CORE_CONFIG_HPP
#define CHIPS_CORE_CONFIG_HPP

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               CONFIG
////////////////////////////////////////////////////////////////////////////////
    
    /// The directory where all resources are held
    /// CHIPS_RESOURCE_ROOT must be defined by the build.
    constexpr const char resource_root[] = CHIPS_RESOURCE_ROOT;
    
    /// Information about the textures/sprites
    constexpr const char tile_image_file[] = CHIPS_RESOURCE_ROOT "/tileset.gif";
    constexpr const unsigned texture_rows = 16;
    constexpr const unsigned texture_cols = 13;
    constexpr const unsigned texture_width = 416;
    constexpr const unsigned texture_height = 512;
    constexpr const unsigned tile_width = texture_width / texture_cols;
    constexpr const unsigned tile_height = texture_height / texture_rows;
    constexpr const unsigned num_tiles = texture_rows * texture_cols;
    
    static_assert(texture_width % texture_cols == 0, "Must evenly divide");
    static_assert(texture_height % texture_rows == 0, "Must evenly divide");
    
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
    constexpr const unsigned chip_count_ypos = 
        scoreboard_ypos + (scoreboard_height / 2) - 25;
        
    /// Information about the top level window
    constexpr const char window_name[] = "chips";
    constexpr const unsigned window_xpos = 100;
    constexpr const unsigned window_ypos = 100;
    constexpr const unsigned window_width = level_window_width + 300 + 20;
    constexpr const unsigned window_height = level_window_height + 100 + 20;
}                                                           // namespace chips
#endif /* CHIPS_CORE_CONFIG_HPP */