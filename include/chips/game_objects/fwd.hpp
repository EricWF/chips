#ifndef CHIPS_GAME_OBJECTS_FWD_HPP
#define CHIPS_GAME_OBJECTS_FWD_HPP

namespace chips
{
   
   constexpr const int texture_rows = 16;
   constexpr const int texture_cols = 13;
   
   
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
        brown_button 39, 
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
        bug = 96, 
       
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
        ice_SE 28, 
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
        brown_button 39, 
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
        bug_N = 96, 
        bug_W = 97, 
        bug_S = 98, 
        bug_E = 99, 
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
    
    class tile;
    
    
    class wall;
    
    //static walls
    class static_wall;
    class blue_wall;
    class hidden_wall;
    class invisible_wall;
    class thin_wall;
    class recessed_wall;
    
    // special walls
    class toggle_wall;
    class clone_machine;
    class clone_block;
    
    // acting_walls
    class wall;
    class static_wall;
    
    
    
    template <std::size_t ReqChipCount>
    class level_lock;
    
    template <tile_id KeyID>
    class lock;
    
    template <tile_id KeyID>
    class key;
    
    
    class monster;
    class bug;
    class fireball;
    class pink_ball;
    class tank;
    class glider;
    class teeth;
    class walker;
    class blob;
    class organism;
    
    
    
}                                                           // namespace chips
#endif /* CHIPS_GAME_OBJECTS_FWD_HPP */