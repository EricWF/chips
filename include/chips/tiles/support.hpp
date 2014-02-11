#ifndef CHIPS_TILES_SUPPORT_HPP
#define CHIPS_TILES_SUPPORT_HPP

# include "chips/tiles/fwd.hpp"
# include "chips/error.hpp"

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
    
    constexpr texture_index to_texture_index(texture_id id) noexcept
    {
        return static_cast<texture_index>(id);
    }
    
    constexpr texture_index to_texture_index(texture_id id, direction dir)
    {
        return static_cast<texture_index>(
            directional_texture_id(id, dir)
          );
    }
    
    constexpr texture_index 
    to_texture_index(texture_id id, direction dir, texture_type t)
    {
        return static_cast<texture_index>(
            static_cast<int>(directional_texture_id(id, dir))
            + (static_cast<int>(t) * 3 * texture_rows)
          );
    }
    
    constexpr int texture_index_row(texture_index i) noexcept
    {
        return static_cast<int>(i) % texture_rows;
    }
    
    constexpr int texture_index_col(texture_index i) noexcept
    {
        return static_cast<int>(i) / texture_rows;
    }
    
    constexpr position to_texture_position(texture_id id) noexcept
    {
        return to_texture_position( to_texture_index(id) );
    }
    
    constexpr position to_texture_position(texture_id id, direction dir)
    {
        return to_texture_position( to_texture_index(id, dir) );
    }
    
    constexpr position 
    to_texture_position(texture_id id, direction dir, texture_type t)
    {
        return to_texture_position( to_texture_index(id, dir, t) );
    }
    
    constexpr position to_texture_position(texture_index i) noexcept
    {
        return position{
            texture_index_col(i) * tile_width
          , texture_index_row(i) * tile_height
        };
    }
    
    constexpr texture_index get_chip_texture_index(chip_state st, direction dir)
    {
        return ((st == chip_state::normal || st == chip_state::swimming)
                ? to_texture_index(static_cast<texture_id>(st), dir)
                : to_texture_index(static_cast<texture_id>(st))
          );
    }
    
    constexpr texture_index get_monster_texture_index(tile_id id, direction dir)
    {
        return to_texture_index(static_cast<texture_id>(id), dir);
    }
}                                                           // namespace chips
#endif /* CHIPS_TILES_SUPPORT_HPP */