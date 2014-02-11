#ifndef CHIPS_TILES_SUPPORT_HPP
#define CHIPS_TILES_SUPPORT_HPP

# include "chips/tiles/fwd.hpp"
# include "chips/error.hpp"
# include <elib/aux.hpp>

namespace chips
{
    constexpr bool is_monster(tile_id id) noexcept
    {
        return (id >= tile_id::bug && id <= tile_id::germ);
    }
    
    constexpr bool is_wall(tile_id id) noexcept
    {
        return id == tile_id::wall
            || id == tile_id::invisible_wall
            || id == tile_id::thin_wall
            || id == tile_id::blue_wall
            || id == tile_id::pop_up_wall;
    }
    
    constexpr bool is_floor(tile_id id) noexcept
    {
        return id == tile_id::floor
            || is_special_floor(id);
    }
    
    constexpr bool is_special_floor(tile_id id) noexcept
    {
        return id == tile_id::dirt
            || id == tile_id::gravel
            || is_element_floor(id);
    }
    
    constexpr bool is_element_floor(tile_id id) noexcept
    {
        return id == tile_id::water
            || id == tile_id::fire
            || id == tile_id::ice
            || id == tile_id::force_floor;
    }
    
    constexpr bool is_boots(tile_id id) noexcept
    {
        return id == tile_id::flippers
            || id == tile_id::fire_boots
            || id == tile_id::skates
            || id == tile_id::suction_boots;
    }
    
    constexpr bool is_button(tile_id id) noexcept
    {
        return id >= tile_id::green_button
            && id <= tile_id::blue_button;
    }
    
    constexpr bool is_collectable(tile_id id) noexcept
    {
        return id == tile_id::computer_chip
            || id == tile_id::key
            || is_boots(id); 
    }
    
    constexpr bool boots_match_floor(tile_id boots, tile_id floor) noexcept
    {
        return (boots == tile_id::flippers      && floor == tile_id::water)
            || (boots == tile_id::fire_boots    && floor == tile_id::fire)
            || (boots == tile_id::skates        && floor == tile_id::ice)
            || (boots == tile_id::suction_boots && floor == tile_id::force_floor);
    }
    
    constexpr bool key_matches_lock(texture_id key, texture_id lock) noexcept
    {
        return (key == texture_id::red_key    && lock == texture_id::red_lock)
            || (key == texture_id::red_key    && lock == texture_id::red_lock)
            || (key == texture_id::green_key  && lock == texture_id::green_lock)
            || (key == texture_id::yellow_key && lock == texture_id::yellow_lock);
    }
    
    /* for the sake of consistency this function is provided. Since 
     * boots and floor SHOULD have the same value in both enums we can use the
     * other implementation */
    constexpr bool boots_match_floor(texture_id boots, texture_id floor) noexcept
    {
        return boots_match_floor(
            static_cast<tile_id>(boots), static_cast<tile_id>(floor)
        );
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

    constexpr texture_id directional_texture_id(texture_id id, direction dir)
    {
        return (
            is_directional_texture(id)
                ? static_cast<texture_id>(
                    static_cast<int>(id) + static_cast<int>(dir)
                  )
                 : throw chips_error{"not a directional texture"}
          );
    }
    
    constexpr position position_index_to_position(position p) noexcept
    {
        return position{
            p.x * tile_width
          , p.y * tile_height
        };
    }
    
    constexpr position texture_to_position_index(texture_id id) noexcept
    {
        using UnderT = elib::aux::underlying_type_t<texture_id>;
        return position{
            static_cast<UnderT>(id) / texture_cols
          , static_cast<UnderT>(id) % texture_cols
        };
    }
    
    /* I know this function is a god-danmed mess but its the only way to make
     * it constexpr */
    constexpr position texture_to_position_index(texture_id id, texture_type t)
    {
        using UnderT = elib::aux::underlying_type_t<texture_id>;
        return (is_typed_texture(id)
                ? position{
                    (static_cast<UnderT>(id) / texture_cols) + static_cast<int>(t)
                   , static_cast<UnderT>(id) % texture_cols
                  }
                : throw chips_error{
                        "texture_id does not have multiple texture types!"
                        }
          );
    }
    
    constexpr position texture_to_position(texture_id id) noexcept
    {
        return position_index_to_position(
            texture_to_position_index(id)
          );
    }
    
    constexpr position texture_to_position(texture_id id, texture_type t)
    {
        return position_index_to_position(
            texture_to_position_index(id, t)
          );
    }
}                                                           // namespace chips
#endif /* CHIPS_TILES_SUPPORT_HPP */