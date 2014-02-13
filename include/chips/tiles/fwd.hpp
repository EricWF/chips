#ifndef CHIPS_TILES_FWD_HPP
#define CHIPS_TILES_FWD_HPP

# include "chips/fwd.hpp"
# include "chips/position.hpp"

# define CHIPS_TILE_DEFAULT_CLASS(Name) \
    using traits::traits;               \
    virtual ~Name() noexcept = default; \
    virtual void accept_visitor(tile_visitor & v)
    
# define CHIPS_TILE_DEFAULT_CLASS_IMPL(Name)    \
    void Name::accept_visitor(tile_visitor & v) \
    {                                           \
        v.visit(*this);                         \
    }

namespace chips
{
   
    /* WARNING: ordering and assigned values are VERY IMPORTANT
     * DO NOT MODIFY */
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
        force_floor = 13,
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
    
    /* WARNING: Order and assigned values are VERY IMPORTANT!
     * DO NOT MODIFY */
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
        force_floor_S = 13,
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
        toggle_wall_open = 38, 
        brown_button = 39, 
        blue_button = 40, 
        teleport = 41, 
        bomb = 42,
        trap = 43, 
        // TODO what is this
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
        unused_54 = 54, 
        unused_55 = 55, 
        unused_56 = 56, 
        chip_fake_exit = 57, 
        fake_exit_dark = 58, 
        fake_exit_light = 59, 
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
    
    // used as a strongly typed integral
    enum class texture_index {};
    
    // TODO this should probably go elsewhere
    enum class chip_state
    {
        normal       = static_cast<int>(texture_id::chip_N),
        swimming     = static_cast<int>(texture_id::chip_swimming_N), 
        drowned      = static_cast<int>(texture_id::chip_drowned),
        burned_fire  = static_cast<int>(texture_id::chip_burned_fire), 
        burned_smoke = static_cast<int>(texture_id::chip_burned_smoke), 
        fake_exit    = static_cast<int>(texture_id::chip_fake_exit)
    };
    
    constexpr bool is_monster(tile_id) noexcept;
    constexpr bool is_wall(tile_id) noexcept;
    constexpr bool is_floor(tile_id) noexcept;
    constexpr bool is_special_floor(tile_id) noexcept;
    constexpr bool is_element_floor(tile_id) noexcept;
    constexpr bool is_boots(tile_id) noexcept;
    constexpr bool is_button(tile_id) noexcept;
    constexpr bool is_collectable(tile_id) noexcept;
    constexpr bool boots_match_floor(tile_id boots, tile_id floor) noexcept;
    
    constexpr bool key_matches_lock(texture_id key, texture_id lock) noexcept;
    constexpr bool boots_match_floor(texture_id boots, texture_id floor) noexcept;
    
    constexpr bool is_directional_texture(texture_id) noexcept;
    constexpr bool is_typed_texture(texture_id) noexcept;
    
    constexpr bool is_tile_texture(texture_index) noexcept;
    constexpr bool is_cutout_texture(texture_index) noexcept;
    constexpr bool is_outline_texture(texture_index) noexcept;
    
    tile_id texture_to_tile_id(texture_id) noexcept;
    
    constexpr texture_id directional_texture_id(texture_id, direction);
    
    /* Only supports texture_id's with contigious directions */
    constexpr texture_index to_texture_index(texture_id) noexcept;
    constexpr texture_index to_texture_index(texture_id, direction);
    constexpr texture_index to_texture_index(texture_id, direction, texture_type);
    
    constexpr int texture_index_row(texture_index) noexcept;
    constexpr int texture_index_col(texture_index) noexcept;
    
    constexpr position to_texture_position(texture_id) noexcept;
    constexpr position to_texture_position(texture_id, direction);
    constexpr position to_texture_position(texture_id, direction, texture_type);
    constexpr position to_texture_position(texture_index) noexcept;
    
    constexpr texture_index get_chip_texture_index(chip_state, direction);
    constexpr texture_index get_monster_texture_index(tile_id, direction);
    
    // base class for all tiles (except chip?)
    class tile;
    
    constexpr bool intersect(tile const & lhs, tile const & rhs) noexcept;
    
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
    
    class key;
    class lock;
    class exit;
    class thief;
    class socket;
    
    class green_button;
    class red_button;
    class brown_button;
    class blue_button;
    
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