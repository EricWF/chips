#ifndef CHIPS_CHIPS_STATE_HPP
# define CHIPS_CHIPS_STATE_HPP

# include "chips/attribute.hpp"
# include "chips/texture_id.hpp"
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <map>
# include <string>

namespace chips
{
    // TODO this should probably go elsewhere
    enum class chips_state
    {
        normal       = static_cast<int>(texture_id::chip_N),
        swimming     = static_cast<int>(texture_id::chip_swimming_N), 
        drowned      = static_cast<int>(texture_id::chip_drowned),
        burned_fire  = static_cast<int>(texture_id::chip_burned_fire), 
        burned_smoke = static_cast<int>(texture_id::chip_burned_smoke), 
        fake_exit    = static_cast<int>(texture_id::chip_fake_exit)
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
}                                                           // namespace chips
#endif /* CHIPS_CHIPS_STATE_HPP */
