#include "chips/tiles/support.hpp"

namespace chips
{
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wswitch-enum"
# elif defined(__GNUG__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-enum"
# endif
    inline tile_id texture_to_tile_id(texture_id id) noexcept
    {
        switch (id)
        {
            /* if a enum exists with the same number in both
            * it is a default case */
            default: 
                return static_cast<tile_id>(id);
                
            case texture_id::thin_wall_N: case texture_id::thin_wall_W:
            case texture_id::thin_wall_S: case texture_id::thin_wall_E:
            case texture_id::thin_wall_SE:
                return tile_id::thin_wall;
                
            case texture_id::force_floor_N: case texture_id::force_floor_W:
            case texture_id::force_floor_S: case texture_id::force_floor_E:
            case texture_id::force_floor_random:
                return tile_id::force_floor;
                
            case texture_id::blue_lock: case texture_id::red_lock:
            case texture_id::green_lock: case texture_id::yellow_lock:
                return tile_id::lock;
            
            case texture_id::ice_NW: case texture_id::ice_NE:
            case texture_id::ice_SE: case texture_id::ice_SW:
                return tile_id::ice;
            
            case texture_id::blue_wall_fake: case texture_id::blue_wall_real:
                return tile_id::blue_wall;
                
            case texture_id::toggle_wall_open: case texture_id::toggle_wall_closed:
                return tile_id::toggle_wall;
            
            case texture_id::fake_exit_dark: case texture_id::fake_exit_light:
                return tile_id::fake_exit;
                
            case texture_id::bug_N: case texture_id::bug_W:
            case texture_id::bug_S: case texture_id::bug_E:
                return tile_id::bug;
                
            case texture_id::fireball_N: case texture_id::fireball_W:
            case texture_id::fireball_S: case texture_id::fireball_E:
                return tile_id::fireball;
                
            case texture_id::pink_ball_N: case texture_id::pink_ball_W:
            case texture_id::pink_ball_S: case texture_id::pink_ball_E:
                return tile_id::pink_ball;
            
            case texture_id::tank_N: case texture_id::tank_W:
            case texture_id::tank_S: case texture_id::tank_E:
                return tile_id::tank;
                
            case texture_id::glider_N: case texture_id::glider_W:
            case texture_id::glider_S: case texture_id::glider_E:
                return tile_id::glider;
                
            case texture_id::teeth_N: case texture_id::teeth_W:
            case texture_id::teeth_S: case texture_id::teeth_E:
                return tile_id::teeth;
                
            case texture_id::walker_N: case texture_id::walker_W:
            case texture_id::walker_S: case texture_id::walker_E:
                return tile_id::walker;
                
            case texture_id::blob_N: case texture_id::blob_W:
            case texture_id::blob_S: case texture_id::blob_E:
                return tile_id::blob;
                
            case texture_id::germ_N: case texture_id::germ_W:
            case texture_id::germ_S: case texture_id::germ_E:
                return tile_id::germ;
            
            //TODO should chip_fake_exit convert to fake_exit or chip
            case texture_id::chip_drowned: case texture_id::chip_burned_fire:
            case texture_id::chip_burned_smoke: case texture_id::chip_fake_exit:
            case texture_id::chip_swimming_N: case texture_id::chip_swimming_W:
            case texture_id::chip_swimming_S: case texture_id::chip_swimming_E:
            case texture_id::chip_N: case texture_id::chip_W:
            case texture_id::chip_S: case texture_id::chip_E:
                return tile_id::chip;
            
            case texture_id::block_14: case texture_id::block_15:
            case texture_id::block_16: case texture_id::block_17:
            case texture_id::unused_32: case texture_id::UNUSED_44:
            case texture_id::unused_54: case texture_id::unused_55:
            case texture_id::unused_56:
                throw chips_error{"Attempted conversion of unused texture_id"};
        }
    }
# if defined(__clang__)
#   pragma clang diagnostic pop
# elif defined(__GNUG__)
#   pragma GCC diagnostic pop
# endif

}                                                           // namespace chips