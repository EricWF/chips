#ifndef CHIPS_CORE_ID_HPP
#define CHIPS_CORE_ID_HPP

# include "chips/core/error.hpp"
# include "chips/core/direction.hpp"
# include "chips/entity/fwd.hpp"
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
        gun = 54, 
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
    
    
////////////////////////////////////////////////////////////////////////////////
//                                TILE_ID
////////////////////////////////////////////////////////////////////////////////
    
    /**
     * tile_id's map to an entity's spot in the texture map, 
     * after transforming for direction (and sometimes state)
     *  WARNING: Order and assigned values are VERY IMPORTANT!
     * DO NOT MODIFY */
    enum class tile_id
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
        blue_wall_real = 30, 
        blue_wall_fake = 31, 
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
        chip_burned_smoke = 52, 
        chip_burned_fire = 53, 
        gun = 54, 
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
    
    /// Allow tile_id to be used as
    /// attributes in entity (compile time check)
    namespace extension
    {
        template <>
        struct is_attribute_impl<tile_id> : elib::true_ {};
    }                                                    // namespace extension
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    /// @see basic_enum_traits<entity_id>
    template <>
    struct basic_enum_traits<::chips::tile_id>
    {
        static const std::map<::chips::tile_id, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    inline std::string to_string(tile_id id)
    {
        return elib::enumeration::enum_cast<std::string>(id);
    }
    
    inline tile_id to_tile_id(std::string const & s)
    {
        return elib::enumeration::enum_cast<tile_id>(s);
    }
    
    /// Checks if the texture has a direction component
    constexpr bool is_directional_texture(tile_id id) noexcept
    {
        return (id >= tile_id::thin_wall_N && id <= tile_id::thin_wall_E)
            || (id >= tile_id::chip_swimming_N && id <= tile_id::germ_E)
            || (id >= tile_id::chip_N && id <= tile_id::chip_E);
    }
    
    ////////////////////////////////////////////////////////////////////////
    constexpr tile_id directional_tile_id(tile_id id, direction dir)
    {
        return (is_directional_texture(id)
            ? static_cast<tile_id>(
                static_cast<unsigned>(id) + static_cast<unsigned>(dir)
            )
            : throw chips_error("not a directional texture")
        );
    }
}                                                           // namespace chips
#endif /* CHIPS_CORE_ID_HPP */