#include "chips/chips_state.hpp"


namespace elib { namespace enumeration
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wglobal-constructors"
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    
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
}}