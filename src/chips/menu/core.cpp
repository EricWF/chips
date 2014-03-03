#include "chips/menu/core.hpp"

namespace elib { namespace enumeration
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wglobal-constructors"
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif

#define _(Name) { ::chips::menu_item_id::Name, #Name }
    const std::map<::chips::menu_item_id, std::string>
    basic_enum_traits<::chips::menu_item_id>::name_map =
        {
            _(none)
          , _(begin)
          , _(levels)
          , _(highscore)
          , _(quit)
        };
#undef _

#define _(Name) { ::chips::align_h::Name, #Name }
    const std::map<::chips::align_h, std::string>
    basic_enum_traits<::chips::align_h>::name_map =
        {
            _(left)
          , _(center)
          , _(right)
        };
#undef _

#define _(Name) { ::chips::align_v::Name, #Name }
    const std::map<::chips::align_v, std::string>
    basic_enum_traits<::chips::align_v>::name_map =
        {
            _(top)
          , _(center)
          , _(bottom)
        };

#undef _
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
}}                                               // namespace elib::enumeration