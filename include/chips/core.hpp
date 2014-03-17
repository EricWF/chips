#ifndef CHIPS_CORE_HPP
#define CHIPS_CORE_HPP

# include "chips/entity_fwd.hpp"
# include <elib/except.hpp>
# include <elib/enumeration.hpp>
# include <elib/log.hpp>
# include <map>
# include <string>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               CONFIG
////////////////////////////////////////////////////////////////////////////////
    
    /// The directory where all resources are held
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
    constexpr const unsigned level_window_xpos = 10;
    constexpr const unsigned level_window_ypos = 10;
    constexpr const unsigned level_window_width = level_view_width * tile_width;
    constexpr const unsigned level_window_height = level_view_height * tile_height;
    
    /// Information about the top level window
    constexpr const char window_name[] = "chips";
    constexpr const unsigned window_xpos = 100;
    constexpr const unsigned window_ypos = 100;
    constexpr const unsigned window_width = level_window_width + 300 + 20;
    constexpr const unsigned window_height = level_window_height + 100 + 20;

////////////////////////////////////////////////////////////////////////////////
//                               ERROR
////////////////////////////////////////////////////////////////////////////////
    
    /// The error used in chips. It should be thrown with
    /// ELIB_THROW_EXCEPTION to get throwsite information
    class chips_error : public elib::exception
    {
    public:
        using elib::exception::exception;
        
        ELIB_DEFAULT_COPY_MOVE(chips_error);
    };
    
////////////////////////////////////////////////////////////////////////////////
//                               LOG
////////////////////////////////////////////////////////////////////////////////
    
    namespace detail { struct chips_log_tag {}; }
    
    /// the cutoff logging level. It's defined as
    /// enum class level_e
    /// { debug, step, info, warn, err }
    /// usage: level_e::debug, level_e::err...
    using elib::log::level_e;
    
    /// a log class with static logging methods.
    /// usage:
    /// log::debug(const char *msg, ...)
    /// log::step(const char *msg, ...)
    /// NOTE: they have the same signature as printf
    using log = elib::log::static_log<detail::chips_log_tag>;
    
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
}}                                               // namespace elib::enumeration

namespace chips
{
    /// pretty-print entity_id
    inline std::string to_string(entity_id id)
    {
        return elib::enumeration::enum_cast<std::string>(id);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //  Entity ID Queries and Catagories
    ////////////////////////////////////////////////////////////////////////////
    
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
    
////////////////////////////////////////////////////////////////////////////////
//                                TEXTURE_ID
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
    
    /// texture_type refers to the different sets of colunms in the sprite sheet.
    /// tile: the sprite against the default base tile
    /// cutout: the sprite's against white.
    /// outline: the black outline version of the sprites
    enum class texture_type
    {
        tile, 
        cutout,
        outline
    };
    
    /// Allow tile_id and texture_type to be used as
    /// attributes in entity (compile time check)
    namespace extension
    {
        template <>
        struct is_attribute_impl<tile_id> : elib::true_ {};
        
        template <>
        struct is_attribute_impl<texture_type> : elib::true_ {};
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
    
    template <>
    struct basic_enum_traits<::chips::texture_type>
    {
        static const std::map<::chips::texture_type, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    inline std::string to_string(tile_id id)
    {
        return elib::enumeration::enum_cast<std::string>(id);
    }

    inline std::string to_string(texture_type t)
    {
        return elib::enumeration::enum_cast<std::string>(t);
    }
    
    /// Checks if the texture has a direction component
    constexpr bool is_directional_texture(tile_id id) noexcept
    {
        return (id >= tile_id::thin_wall_N && id <= tile_id::thin_wall_E)
            || (id >= tile_id::chip_swimming_N && id <= tile_id::germ_E)
            || (id >= tile_id::chip_N && id <= tile_id::chip_E);
    }
    
    /// check if the tile_id has black outline sprite 
    /// and white alpha channel sprite
    constexpr bool is_typed_texture(tile_id id) noexcept
    {
        return id >= tile_id::chip_swimming_N;
    }
    
     /// Chip has a number of textures for dying and moving in
     /// different ways. This enumeration allows you to find the tile_id
     /// of chuck in a given state
    enum class chips_state
    {
        normal       = static_cast<int>(tile_id::chip_N),
        swimming     = static_cast<int>(tile_id::chip_swimming_N), 
        drowned      = static_cast<int>(tile_id::chip_drowned),
        burned_fire  = static_cast<int>(tile_id::chip_burned_fire), 
        burned_smoke = static_cast<int>(tile_id::chip_burned_smoke), 
        fake_exit    = static_cast<int>(tile_id::chip_fake_exit)
    };
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<chips_state> : elib::true_ {};
    }                                                    // namespace extension
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    template <>
    struct basic_enum_traits<::chips::chips_state>
    {
        static const std::map<::chips::chips_state, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    inline std::string to_string(chips_state s)
    {
        return elib::enumeration::enum_cast<std::string>(s);
    }
    
////////////////////////////////////////////////////////////////////////////////
//                           DIRECTION && POSITION
////////////////////////////////////////////////////////////////////////////////
    
    enum class direction : unsigned
    {
        N, W, S, E
    };

# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-default"
# endif
    inline std::string to_string(direction d)
    {
        std::string s = "direction: ";
        switch (d)
        {
            case direction::N:
                s += "N"; break;
            case direction::W:
                s += "W"; break;
            case direction::S:
                s += "S"; break;
            case direction::E:
                s += "E"; break;
        }
        return s;
    }
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic pop
# endif

    struct position
    {
        constexpr position() : x(0), y(0) {}
        constexpr position(unsigned x_, unsigned y_) : x(x_), y(y_) {}
        
        unsigned x, y;
    };
    
   
    inline std::string to_string(position p)
    {
        return elib::fmt("position: x=%i y=%i", p.x, p.y);
    }
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<direction> : elib::true_ {};
        
        template <>
        struct is_attribute_impl<position>  : elib::true_ {};
    }
    
    constexpr bool 
    operator==(position const & lhs, position const & rhs) noexcept
    { 
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    
    constexpr bool
    operator !=(position const & lhs, position const & rhs) noexcept
    {
        return lhs.x != rhs.x || lhs.y != rhs.y;
    }
    
    constexpr bool 
    operator<(position const & lhs, position const & rhs) noexcept
    {
        return lhs.x < rhs.x || 
                (lhs.x == rhs.x && lhs.y < rhs.y);
    }
    
    constexpr bool
    operator>(position const & lhs, position const & rhs) noexcept
    {
        return rhs < lhs; 
    }
    
    constexpr bool
    operator <=(position const & lhs, position const & rhs) noexcept
    {
        return !(rhs < lhs);
    }
    
    constexpr bool
    operator >=(position const & lhs, position const & rhs) noexcept
    {
        return !(lhs < rhs); 
    }
    
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-default"
# endif
    inline position move(position pos, direction dir, unsigned count = 1)
    {
        switch (dir)
        {
            case direction::N: pos.y += count; break;
            case direction::S: pos.y -= count; break;
            case direction::E: pos.x += count; break;
            case direction::W: pos.x -= count; break;
        }
        return pos;
    }
# if defined(ELIB_CONFIG_GCC)
#   pragma GCC diagnostic pop
# endif
    
    constexpr direction 
    turn_clockwise(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            ((static_cast<unsigned>(dir) + times) + 2) % 4
        );
    }
    
    constexpr direction 
    turn_counter_clockwile(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            (static_cast<unsigned>(dir) + times) % 4
        );
    }
    
    constexpr direction
    turn_around(direction dir, unsigned times = 1) noexcept
    {
        return static_cast<direction>(
            (static_cast<unsigned>(dir) + (times * 2)) % 4
        );
    }
    
////////////////////////////////////////////////////////////////////////////////
//                            TEXTURE_INDEX
////////////////////////////////////////////////////////////////////////////////

    enum class texture_index : unsigned {};
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<texture_index> : elib::true_ {};
    }                                                    // namespace extension
    
    constexpr unsigned texture_index_row(texture_index i) noexcept
    {
        return static_cast<unsigned>(i) % texture_rows;
    }
    
    constexpr unsigned texture_index_col(texture_index i) noexcept
    {
        return static_cast<unsigned>(i) / texture_rows;
    }
    
    constexpr bool is_tile_texture(texture_index i) noexcept
    {
        return texture_index_col(i) <= 6;
    }
    
    constexpr bool is_cutout_texture(texture_index i) noexcept
    {
        return texture_index_col(i) >= 7 && texture_index_col(i) <= 9;
    }
    
    constexpr bool is_outline_texture(texture_index i) noexcept
    {
        return texture_index_col(i) >= 10 && texture_index_col(i) <= 12;
    }

    constexpr tile_id directional_tile_id(tile_id id, direction dir)
    {
        return (is_directional_texture(id)
            ? static_cast<tile_id>(
                static_cast<unsigned>(id) + static_cast<unsigned>(dir)
              )
            : throw chips_error{"not a directional texture"}
        );
    }
    
    constexpr texture_index to_texture_index(tile_id id) noexcept
    {
        return static_cast<texture_index>(id);
    }
    
    constexpr texture_index to_texture_index(tile_id id, direction dir)
    {
        return static_cast<texture_index>(
            directional_tile_id(id, dir)
          );
    }
    
    constexpr texture_index 
    to_texture_index(tile_id id, direction dir, texture_type t)
    {
        return static_cast<texture_index>(
            static_cast<unsigned>(directional_tile_id(id, dir))
            + (static_cast<unsigned>(t) * 3 * texture_rows)
          );
    }
    
    constexpr position to_texture_position(texture_index i) noexcept
    {
        return position{
            texture_index_col(i) * tile_width
          , texture_index_row(i) * tile_height
        };
    }
    
    constexpr position to_texture_position(tile_id id) noexcept
    {
        return to_texture_position( to_texture_index(id) );
    }
    
    constexpr position to_texture_position(tile_id id, direction dir)
    {
        return to_texture_position( to_texture_index(id, dir) );
    }
    
    constexpr position 
    to_texture_position(tile_id id, direction dir, texture_type t)
    {
        return to_texture_position( to_texture_index(id, dir, t) );
    }    
}                                                           // namespace chips
#endif /* CHIPS_CORE_HPP */