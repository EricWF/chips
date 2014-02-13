#ifndef CHIPS_FWD_HPP
#define CHIPS_FWD_HPP

namespace chips
{
    constexpr const char tile_image_file[] = CHIPS_RES_ROOT "/tileset.gif";
    constexpr const int texture_rows = 16;
    constexpr const int texture_cols = 13;
    constexpr const int texture_width = 416;
    constexpr const int texture_height = 512;
    constexpr const int tile_width = texture_width / texture_cols;
    constexpr const int tile_height = texture_height / texture_rows;
    constexpr const int num_tiles = texture_rows * texture_cols;
    
    static_assert(texture_width % texture_cols == 0, "Must evenly divide");
    static_assert(texture_height % texture_rows == 0, "Must evenly divide");
    
    constexpr const char level_file_root[] = CHIPS_RES_ROOT;
    constexpr const int level_width = 32;
    constexpr const int level_height = 32;
    constexpr const int level_size = level_width * level_height;
    constexpr const int level_view_width = 9;
    constexpr const int level_view_height = 9;
    
    constexpr const int level_window_xpos = 10;
    constexpr const int level_window_ypos = 10;
    constexpr const int level_window_width = level_view_width * tile_width;
    constexpr const int level_window_height = level_view_height * tile_height;
    
    constexpr const char window_name[] = "chips";
    constexpr const int window_xpos = 100;
    constexpr const int window_ypos = 100;
    constexpr const int window_width = level_window_width + 300 + 20;
    constexpr const int window_height = level_window_height + 100 + 20;
    
    
}                                                           // namespace chips
#endif /* CHIPS_FWD_HPP */