#ifndef CHIPS_TEXTURE_ID_HPP
# define CHIPS_TEXTURE_ID_HPP

# include "chips/attribute.hpp"
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <map>
# include <string>

namespace chips
{
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
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<texture_id> : elib::true_ {};
        
        template <>
        struct is_attribute_impl<texture_type> : elib::true_ {};
    }                                                    // namespace extension
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    template <>
    struct basic_enum_traits<::chips::texture_id>
    {
        static const std::map<::chips::texture_id, std::string> name_map;
    };
    
    template <>
    struct basic_enum_traits<::chips::texture_type>
    {
        static const std::map<::chips::texture_type, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    inline std::string to_string(texture_id id)
    {
        return elib::enumeration::enum_cast<std::string>(id);
    }

    inline std::string to_string(texture_type t)
    {
        return elib::enumeration::enum_cast<std::string>(t);
    }
    
    constexpr bool is_directional_texture(texture_id id) noexcept
    {
        return (id >= texture_id::thin_wall_N && id <= texture_id::thin_wall_E)
            || (id >= texture_id::chip_swimming_N && id <= texture_id::germ_E)
            || (id >= texture_id::chip_N && id <= texture_id::chip_E);
    }
    
    constexpr bool is_typed_texture(texture_id id) noexcept
    {
        return id >= texture_id::chip_swimming_N;
    }
}                                                           // namespace chips
#endif /* CHIPS_TEXTURE_ID_HPP */