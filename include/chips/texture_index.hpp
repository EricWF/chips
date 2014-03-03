#ifndef CHIPS_TEXTURE_INDEX_HPP
# define CHIPS_TEXTURE_INDEX_HPP

# include "chips/attribute.hpp"
# include "chips/config.hpp"
# include "chips/error.hpp"
# include "chips/position.hpp"
# include "chips/texture_id.hpp"
# include <elib/aux.hpp>

namespace chips
{
    enum class texture_index {};
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<texture_index> : elib::true_ {};
    }                                                    // namespace extension
    
    constexpr int texture_index_row(texture_index i) noexcept
    {
        return static_cast<int>(i) % texture_rows;
    }
    
    constexpr int texture_index_col(texture_index i) noexcept
    {
        return static_cast<int>(i) / texture_rows;
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
        return (is_directional_texture(id)
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
    
    constexpr position to_texture_position(texture_index i) noexcept
    {
        return position{
            texture_index_col(i) * tile_width
          , texture_index_row(i) * tile_height
        };
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
    
}                                                           // namespace chips
#endif /* CHIPS_TEXTURE_INDEX_HPP */