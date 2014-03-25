#ifndef CHIPS_CORE_ENTITY_ID_HPP
#define CHIPS_CORE_ENTITY_ID_HPP

# include "chips/core/error.hpp"
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <map>
# include <string>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                              ENTITY_ID
////////////////////////////////////////////////////////////////////////////////
    
    /**
     * Each entity_id should be an entity that needs unique logic.
     * An entity ID should map directly onto texture ids in most cases.
     * certain entities have "directional" textures. chips has a texture for
     * N, S, E, and W facing sprites. In these cases we can use the entity id
     * and a direction to map it onto the correct tile_id
     */
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
        chip = 108, 
        
        BAD_ID = -1
    };
    
    enum class entity_type
    {
        none, 
        chip, 
        actor, 
        item, 
        base
    };
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    /// This nonsense allows the use of the statments
    /// std::string elib::enumeration::enum_cast<std::string>(Enum)
    /// and Enum elib::enumeration::enum_cast<Enum>(std::string)
    template <>
    struct basic_enum_traits<::chips::entity_id>
    {
        static const std::map<::chips::entity_id, std::string> name_map;
    };
    
    template <>
    struct basic_enum_traits<::chips::entity_type>
    {
        static const std::map<::chips::entity_type, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    /// pretty-print entity_id
    inline std::string to_string(entity_id id)
    {
        return elib::enumeration::enum_cast<std::string>(id);
    }
    
    inline std::string to_string(entity_type t)
    {
        return elib::enumeration::enum_cast<std::string>(t);
    }
    
    inline entity_id to_entity_id(std::string const & s) 
    {
        return elib::enumeration::enum_cast<entity_id>(s);
    }
    
    inline entity_type to_entity_type(std::string const & s)
    {
        return elib::enumeration::enum_cast<entity_type>(s);
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    //                           ACTOR QUERIES
    ////////////////////////////////////////////////////////////////////////////
    
    constexpr bool is_chip(entity_id id) noexcept
    {
        return id == entity_id::chip;
    }
    
    constexpr bool is_monster(entity_id id) noexcept
    {
        return (id >= entity_id::bug && id <= entity_id::germ);
    }
    
    constexpr bool is_actor(entity_id id) noexcept
    {
        return is_chip(id) || is_monster(id) || entity_id::block == id;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //                           BASE QUERIES
    ////////////////////////////////////////////////////////////////////////////
    
    constexpr bool is_lock(entity_id id) noexcept
    {
        return id == entity_id::red_lock
            || id == entity_id::blue_lock
            || id == entity_id::green_lock
            || id == entity_id::yellow_lock;
    }
    
    constexpr bool is_button(entity_id id) noexcept
    {
        return entity_id::green_button == id
            || entity_id::red_button   == id
            || entity_id::brown_button == id
            || entity_id::blue_button  == id;
    }
    
    constexpr bool is_acting_wall(entity_id id) noexcept
    {
        return is_lock(id) 
          || entity_id::socket        == id
          || entity_id::pop_up_wall   == id
          || entity_id::toggle_wall   == id
          || entity_id::clone_machine == id;
    }
    
    constexpr bool is_wall(entity_id id) noexcept
    {
        return id == entity_id::wall
            || id == entity_id::invisible_wall
            || id == entity_id::thin_wall
            || id == entity_id::blue_wall
            || is_acting_wall(id);
    }
    
    constexpr bool is_element_floor(entity_id id) noexcept
    {
        return id == entity_id::water
            || id == entity_id::fire
            || id == entity_id::ice
            || id == entity_id::force_floor;
    }
    
    constexpr bool is_acting_floor(entity_id id) noexcept
    {
        return id == entity_id::exit 
            || id == entity_id::teleport
            || id == entity_id::thief
            || id == entity_id::bomb
            || id == entity_id::trap
            || id == entity_id::hint
            || id == entity_id::fake_exit
            || is_button(id)
            || is_element_floor(id);
    }
    
    constexpr bool is_floor(entity_id id) noexcept
    {
        return id == entity_id::floor
            || id == entity_id::dirt
            || id == entity_id::gravel
            || is_acting_floor(id);
    }
    
    constexpr bool is_acting_base(entity_id id) noexcept
    {
        return is_acting_floor(id) || is_acting_wall(id);
    }
    
    constexpr bool is_base(entity_id id) noexcept
    {
        return is_wall(id) || is_floor(id);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //                            ITEM QUERIES
    ////////////////////////////////////////////////////////////////////////////
    
    constexpr bool is_boots(entity_id id) noexcept
    {
        return id == entity_id::flippers
            || id == entity_id::fire_boots
            || id == entity_id::skates
            || id == entity_id::suction_boots;
    }
    
    constexpr bool is_key(entity_id id) noexcept
    {
        return id == entity_id::red_key
            || id == entity_id::blue_key
            || id == entity_id::green_key
            || id == entity_id::yellow_key;
    }
    
    constexpr bool is_item(entity_id id) noexcept
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
        return (key == entity_id::red_key    && lock == entity_id::red_lock   )
            || (key == entity_id::blue_key   && lock == entity_id::blue_lock  )
            || (key == entity_id::green_key  && lock == entity_id::green_lock )
            || (key == entity_id::yellow_key && lock == entity_id::yellow_lock);
    }
    
# if defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-enum"
# endif
    inline entity_id key_for_lock(entity_id lock) noexcept
    {
        ELIB_ASSERT(is_lock(lock));
        switch (lock)
        {
            case entity_id::red_lock:    return entity_id::red_key;
            case entity_id::blue_lock:   return entity_id::blue_key;
            case entity_id::green_lock:  return entity_id::green_key;
            case entity_id::yellow_lock: return entity_id::yellow_key;
            default:
                ELIB_THROW_EXCEPTION(chips_error(
                    "Bad entity_id passed to key for lock"
                ));
        }
    }
    
    inline entity_id boots_for_floor(entity_id floor) noexcept
    {
        ELIB_ASSERT(is_element_floor(floor));
        switch (floor)
        {
            case entity_id::force_floor: return entity_id::suction_boots;
            case entity_id::water:       return entity_id::flippers;
            case entity_id::fire:        return entity_id::fire_boots;
            case entity_id::ice:         return entity_id::skates;
            default:
                ELIB_THROW_EXCEPTION(chips_error(
                    "Bad entity_id passed to boots_for_floor"
                ));
        }
    }
# if defined(__GNUC__)
#   pragma GCC diagnostic pop
# endif
    
    // forward //
    enum class tile_id;
    constexpr bool is_directional_texture(tile_id) noexcept;
    
    constexpr bool is_directional(entity_id id) noexcept
    {
        return is_directional_texture(static_cast<tile_id>(id));
    }
}                                                           // namespace chips
#endif /* CHIPS_CORE_ENTITY_ID_HPP */