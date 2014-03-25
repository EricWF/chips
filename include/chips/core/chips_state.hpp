#ifndef CHIPS_CORE_CHIPS_STATE_HPP
#define CHIPS_CORE_CHIPS_STATE_HPP

# include "chips/core/id.hpp"
# include <elib/enumeration.hpp>
# include <string>
# include <map>

namespace chips
{
    
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
    
    inline chips_state to_chips_state(std::string const & s)
    {
        return elib::enumeration::enum_cast<chips_state>(s);
    }
}                                                           // namespace chips
#endif /* CHIPS_CORE_CHIPS_STATE_HPP */