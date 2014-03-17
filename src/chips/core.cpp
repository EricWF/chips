#include "chips/core.hpp"

namespace elib { namespace enumeration
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wglobal-constructors"
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif

#define _(Name) { ::chips::entity_id::Name, #Name }
    const std::map<::chips::entity_id, std::string>
    basic_enum_traits<::chips::entity_id>::name_map =
        {
              _(floor) 
            , _(wall)
            , _(computer_chip)
            , _(water)
            , _(fire) 
            , _(invisible_wall)
            , _(thin_wall) 
            , _(block)
            , _(dirt) 
            , _(ice) 
            , _(force_floor)
            , _(exit) 
            , _(blue_lock)
            , _(red_lock)
            , _(green_lock)
            , _(yellow_lock)
            , _(blue_wall) 
            , _(thief) 
            , _(socket) 
            , _(green_button) 
            , _(red_button) 
            , _(toggle_wall) 
            , _(brown_button) 
            , _(blue_button) 
            , _(teleport) 
            , _(bomb)
            , _(trap) 
            , _(gravel) 
            , _(pop_up_wall) 
            , _(hint) 
            , _(clone_machine) 
            , _(fake_exit) 
            , _(bug) 
            , _(fireball) 
            , _(pink_ball) 
            , _(tank) 
            , _(glider) 
            , _(teeth) 
            , _(walker) 
            , _(blob) 
            , _(germ) 
            , _(blue_key)
            , _(red_key)
            , _(green_key)
            , _(yellow_key)
            , _(flippers) 
            , _(fire_boots) 
            , _(skates) 
            , _(suction_boots) 
            , _(chip)
            , _(BAD_ID)
        };
#undef _

#define _(Name) { ::chips::entity_type::Name, #Name }
    const std::map<::chips::entity_type, std::string>
    basic_enum_traits<::chips::entity_type>::name_map =
        {
            _(chip)
          , _(actor)
          , _(item)
          , _(base)
        };
#undef _

# define _(Name) { ::chips::tile_id::Name, #Name }
    const std::map<::chips::tile_id, std::string>
    basic_enum_traits<::chips::tile_id>::name_map =
        {
            _(floor) 
            , _(wall)
            , _(computer_chip)
            , _(water)
            , _(fire) 
            , _(invisible_wall)
            , _(thin_wall_N) 
            , _(thin_wall_W) 
            , _(thin_wall_S) 
            , _(thin_wall_E) 
            , _(block)
            , _(dirt) 
            , _(ice) 
            , _(force_floor_S)
            //TODO figure these out
            , _(block_14) 
            , _(block_15)
            , _(block_16) 
            , _(block_17) 
            , _(force_floor_N) 
            , _(force_floor_E) 
            , _(force_floor_W) 
            , _(exit) 
            , _(blue_lock)
            , _(red_lock) 
            , _(green_lock) 
            // wiki says 26,  but I think it is wrong
            , _(yellow_lock)
            , _(ice_NW) 
            , _(ice_NE) 
            , _(ice_SE) 
            , _(ice_SW) 
            , _(blue_wall_fake) 
            , _(blue_wall_real) 
            , _(unused_32) 
            , _(thief) 
            , _(socket) 
            , _(green_button) 
            , _(red_button) 
            , _(toggle_wall_closed) 
            , _(toggle_wall_open) 
            , _(brown_button) 
            , _(blue_button) 
            , _(teleport) 
            , _(bomb)
            , _(trap) 
            // TODO what is this
            , _(UNUSED_44) 
            , _(gravel) 
            , _(pop_up_wall) 
            , _(hint) 
            , _(thin_wall_SE) 
            , _(clone_machine) 
            , _(force_floor_random) 
            , _(chip_drowned) 
            , _(chip_burned_fire) 
            , _(chip_burned_smoke)
            , _(unused_54) 
            , _(unused_55) 
            , _(unused_56) 
            , _(chip_fake_exit) 
            , _(fake_exit_dark) 
            , _(fake_exit_light) 
            , _(chip_swimming_N) 
            , _(chip_swimming_W) 
            , _(chip_swimming_S) 
            , _(chip_swimming_E) 
            , _(bug_N) 
            , _(bug_W) 
            , _(bug_S) 
            , _(bug_E)
            , _(fireball_N) 
            , _(fireball_W) 
            , _(fireball_S) 
            , _(fireball_E) 
            , _(pink_ball_N) 
            , _(pink_ball_W) 
            , _(pink_ball_S) 
            , _(pink_ball_E) 
            , _(tank_N) 
            , _(tank_W) 
            , _(tank_S) 
            , _(tank_E) 
            , _(glider_N) 
            , _(glider_W) 
            , _(glider_S) 
            , _(glider_E) 
            , _(teeth_N) 
            , _(teeth_W) 
            , _(teeth_S) 
            , _(teeth_E) 
            , _(walker_N) 
            , _(walker_W) 
            , _(walker_S) 
            , _(walker_E) 
            , _(blob_N) 
            , _(blob_W) 
            , _(blob_S) 
            , _(blob_E) 
            , _(germ_N) 
            , _(germ_W) 
            , _(germ_S) 
            , _(germ_E) 
            , _(blue_key) 
            , _(red_key) 
            , _(green_key) 
            , _(yellow_key)
            , _(flippers) 
            , _(fire_boots) 
            , _(skates) 
            , _(suction_boots) 
            // wiki says this starts on 107
            , _(chip_N) 
            , _(chip_W) 
            , _(chip_S) 
            , _(chip_E)
        };
#undef _

# define _(Name) { ::chips::texture_type::Name, #Name }
    const std::map<::chips::texture_type, std::string>
    basic_enum_traits<::chips::texture_type>::name_map =
        {
            _(tile)
          , _(cutout)
          , _(outline)
        };
#undef _

# define _(Name) { ::chips::chips_state::Name, #Name }
    const std::map<::chips::chips_state, std::string>
    basic_enum_traits<::chips::chips_state>::name_map =
        {
            _(normal)
          , _(swimming)
          , _(drowned)
          , _(burned_fire)
          , _(burned_smoke)
          , _(fake_exit)
        };
#undef _

#if defined(__clang__)
# pragma clang diagnostic pop
#endif
}}                                              // namespace elib::enumeration