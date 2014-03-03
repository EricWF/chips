#include "chips/entity_id.hpp"

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
        };
#undef _

#if defined(__clang__)
# pragma clang diagnostic pop
#endif
}}                                              // namespace elib::enumeration