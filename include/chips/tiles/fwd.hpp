#ifndef CHIPS_TILES_FWD_HPP
#define CHIPS_TILES_FWD_HPP

# include "chips/position.hpp"

namespace chips
{
    constexpr const char tile_image_file[] = CHIPS_RES_ROOT "/tileset.gif";
    constexpr const int texture_rows = 16;
    constexpr const int texture_cols = 13;
    constexpr const int texture_width = 416;
    constexpr const int texture_height = 512;
    constexpr const int tile_width = texture_width / texture_cols;
    constexpr const int tile_height = texture_height / texture_rows;
   
   /* Concepts:
     *    Object: A block that can move
     *      
     */
    enum class tile_id
    {
        floor = 0, 
        wall = 1,
        computer_chip = 2,
        
        water = 3,
        fire = 4, 
        
        invisible_wall = 5,
        thin_wall = 6, 
        
        block = 10,
        dirt = 11, 
        ice = 12, 
        force_flow = 13,
        
        exit = 21, 
        lock = 22, 
        
        blue_wall = 30, 
        
        thief = 33, 
        socket = 34, 
        
        green_button = 35, 
        red_button = 36, 
        toggle_wall = 37, 
        brown_button = 39, 
        blue_button = 40, 
        
        teleport = 41, 
        bomb = 42,
        trap = 43, 
        gravel = 45, 
        pop_up_wall = 46, 
        hint = 47, 
        clone_machine = 49, 
        fake_exit = 57, 
    
        bug = 64, 
        fireball = 68, 
        pink_ball = 72, 
        tank = 76, 
        glider = 80, 
        teeth = 84, 
        walker = 88, 
        blob = 92, 
        germ = 96, 
       
        key = 100, 
        
        flippers = 104, 
        fire_boots = 105, 
        skates = 106, 
        suction_boots = 107, 

        chip = 108
    };
    
    
    enum class texture_id
    {
        floor = 0, 
        wall = 1,
        computer_chip = 2,
        water = 3,
        fire = 4, 
        invisible_wall = 5,
        thin_wall_N = 6, 
        thin_wall_W = 7, 
        thin_wall_S = 8, 
        thin_wall_E = 9, 
        block = 10,
        dirt = 11, 
        ice = 12, 
        force_flow_S = 13,
        //TODO figure these out
        block_14 = 14, 
        block_15 = 15,
        block_16 = 16, 
        block_17 = 17, 
        force_floor_N = 18, 
        force_floor_E = 19, 
        force_floor_W = 20, 
        exit = 21, 
        blue_lock = 22,
        red_lock = 23, 
        green_lock = 24, 
        // wiki says 26,  but I think it is wrong
        yellow_lock = 25,
        ice_NW = 26, 
        ice_NE = 27, 
        ice_SE = 28, 
        ice_SW = 29, 
        blue_wall_fake = 30, 
        blue_wall_real = 31, 
        unused_32 = 32, 
        thief = 33, 
        socket = 34, 
        green_button = 35, 
        red_button = 36, 
        toggle_wall_closed = 37, 
        toggle_wall_opened = 38, 
        brown_button = 39, 
        blue_button = 40, 
        teleport = 41, 
        bomb = 42,
        trap = 43, 
        UNUSED_44 = 44, 
        gravel = 45, 
        pop_up_wall = 46, 
        hint = 47, 
        thin_wall_SE = 48, 
        clone_machine = 49, 
        force_floor_random = 50, 
        chip_drowned = 51, 
        chip_burned_fire = 52, 
        chip_burned_smoke = 53,
        not_used_54 = 54, 
        not_used_55 = 55, 
        not_used_56 = 56, 
        chip_fake_exit = 57, 
        dark_fake_exit = 58, 
        light_fake_exit = 59, 
        chip_swimming_N = 60, 
        chip_swimming_W = 61, 
        chip_swimming_S = 62, 
        chip_swimming_E = 63, 
        bug_N = 64, 
        bug_W = 65, 
        bug_S = 66, 
        bug_E = 67,
        fireball_N = 68, 
        fireball_W = 69, 
        fireball_S = 70, 
        fireball_E = 71, 
        pink_ball_N = 72, 
        pink_ball_W = 73, 
        pink_ball_S = 74, 
        pink_ball_E = 75, 
        tank_N = 76, 
        tank_W = 77, 
        tank_S = 78, 
        tank_E = 79, 
        glider_N = 80, 
        glider_W = 81, 
        glider_S = 82, 
        glider_E = 83, 
        teeth_N = 84, 
        teeth_W = 85, 
        teeth_S = 86, 
        teeth_E = 87, 
        walker_N = 88, 
        walker_W = 89, 
        walker_S = 90, 
        walker_E = 91, 
        blob_N = 92, 
        blob_W = 93, 
        blob_S = 94, 
        blob_E = 95, 
        germ_N = 96, 
        germ_W = 97, 
        germ_S = 98, 
        germ_E = 99, 
        blue_key = 100, 
        red_key = 101, 
        green_key = 102, 
        yellow_key = 103,
        flippers = 104, 
        fire_boots = 105, 
        skates = 106, 
        suction_boots = 107, 
        // wiki says this starts on 107
        chip_N = 108, 
        chip_W = 109, 
        chip_S = 110, 
        chip_E = 111, 
    };
    
    
    enum class texture_type
    {
        tile, 
        cutout,
        outline
    };
    
    bool is_directional(tile_id);
    bool is_monster(tile_id);
    bool is_key(tile_id);
    bool is_lock(tile_id);
    bool is_element(tile_id);
    bool is_boots(tile_id);
    bool is_button(tile_id);
    bool is_collectable(tile_id);
    
    tile_id texture_to_tile_id(texture_id) noexcept;
    
    texture_id 
    directional_texture_id(tile_id, direction
                         , texture_type = texture_type::tile) noexcept;
                                    
    position 
    directional_texture_position(tile_id, direction
                               , texture_type = texture_type::tile) noexcept;
                                        
    
    // base class for all tiles (except chip?)
    class tile;
    
    // player
    class chip;
    
    class floor;
    class wall;
    class invisible_wall;
    class thin_wall;
    class block;
    class dirt;
    class blue_block;
    
    class water;
    class fire;
    class ice;
    class force_floor;
    
    class computer_chip;
    
    template <texture_id BootID>
    class boot;
    
    using flippers = boot<texture_id::flippers>;
    using fire_boots = boot<texture_id::fire_boots>;
    using skates = boot<texture_id::skates>;
    using suction_boots = boot<texture_id::suction_boots>;
    
    template <texture_id KeyID>
    class key;
    
    using blue_key = key<texture_id::blue_key>;
    using red_key = key<texture_id::red_key>;
    using green_key = key<texture_id::green_key>;
    using yellow_key = key<texture_id::yellow_key>;
    
    template <texture_id LockID>
    class lock;
    
    using blue_lock = lock<texture_id::blue_lock>;
    using red_lock = lock<texture_id::red_lock>;
    using green_lock = lock<texture_id::green_lock>;
    using yellow_lock = lock<texture_id::yellow_lock>;
    
    class exit;
    class thief;
    
    template <unsigned NumChipReq>
    class socket;
    
    class green_button;
    class red_button;
    class brown_button;
    class blue_button;
    
    class help;
    class teleport;
    class bomb;
    class trap;
    class gravel;
    class pop_up_wall;
    class hint;
    class clone_machine;
    class fake_exit;
    
    class monster;
    class bug;
    class fireball;
    class pink_ball;
    class tank;
    class glider;
    class teeth;
    class walker;
    class blob;
    class germ;
}                                                           // namespace chips
#endif /* CHIPS_TILES_FWD_HPP */