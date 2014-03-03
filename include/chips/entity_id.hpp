#ifndef CHIPS_ENTITY_ID_HPP
#define CHIPS_ENTITY_ID_HPP

# include "chips/attribute.hpp"
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <map>
# include <string>

namespace chips
{
    enum class entity_id
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
        blue_lock = 22, 
        red_lock = 23, 
        green_lock = 24, 
        yellow_lock = 25, 
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
        blue_key = 100,
        red_key = 101, 
        green_key = 102, 
        yellow_key = 103, 
        flippers = 104, 
        fire_boots = 105, 
        skates = 106, 
        suction_boots = 107, 
        chip = 108
    };
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<entity_id> : elib::true_ {};
    }                                                    // namespace extension
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    template <>
    struct basic_enum_traits<::chips::entity_id>
    {
        static const std::map<::chips::entity_id, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    inline std::string to_string(entity_id id)
    {
        return elib::enumeration::enum_cast<std::string>(id);
    }
    
    constexpr bool is_monster(entity_id id) noexcept
    {
        return (id >= entity_id::bug && id <= entity_id::germ);
    }
    
    constexpr bool is_wall(entity_id id) noexcept
    {
        return id == entity_id::wall
            || id == entity_id::invisible_wall
            || id == entity_id::thin_wall
            || id == entity_id::blue_wall
            || id == entity_id::pop_up_wall;
    }
    
    constexpr bool is_element_floor(entity_id id) noexcept
    {
        return id == entity_id::water
            || id == entity_id::fire
            || id == entity_id::ice
            || id == entity_id::force_floor;
    }
    
    constexpr bool is_special_floor(entity_id id) noexcept
    {
        return id == entity_id::dirt
            || id == entity_id::gravel
            || is_element_floor(id);
    }
    
    constexpr bool is_floor(entity_id id) noexcept
    {
        return id == entity_id::floor
            || is_special_floor(id);
    }
    
    constexpr bool is_boots(entity_id id) noexcept
    {
        return id == entity_id::flippers
            || id == entity_id::fire_boots
            || id == entity_id::skates
            || id == entity_id::suction_boots;
    }
    
    constexpr bool is_button(entity_id id) noexcept
    {
        return id >= entity_id::green_button
            && id <= entity_id::blue_button;
    }
    
    constexpr bool is_key(entity_id id) noexcept
    {
        return id == entity_id::red_key
            || id == entity_id::blue_key
            || id == entity_id::green_key
            || id == entity_id::yellow_key;
    }
    
    constexpr bool is_lock(entity_id id) noexcept
    {
        return id == entity_id::red_lock
            || id == entity_id::blue_lock
            || id == entity_id::green_lock
            || id == entity_id::yellow_lock;
    }
    
    constexpr bool is_collectable(entity_id id) noexcept
    {
        return id == entity_id::computer_chip
            || is_key(id)
            || is_boots(id); 
    }
    
    constexpr bool boots_match_floor(entity_id boots, entity_id floor) noexcept
    {
        return (boots == entity_id::flippers      && floor == entity_id::water)
            || (boots == entity_id::fire_boots    && floor == entity_id::fire)
            || (boots == entity_id::skates        && floor == entity_id::ice)
            || (boots == entity_id::suction_boots && floor == entity_id::force_floor);
    }
    
    constexpr bool key_matches_lock(entity_id key, entity_id lock) noexcept
    {
        return (key == entity_id::red_key    && lock == entity_id::red_lock)
            || (key == entity_id::red_key    && lock == entity_id::red_lock)
            || (key == entity_id::green_key  && lock == entity_id::green_lock)
            || (key == entity_id::yellow_key && lock == entity_id::yellow_lock);
    }
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_ID_HPP */