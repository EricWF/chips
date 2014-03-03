#ifndef CHIPS_MENU_PARSE_HPP
#define CHIPS_MENU_PARSE_HPP

# include "chips/menu/core.hpp"
# include <vector>
# include <string>

namespace chips
{
    std::vector<button_info>
    parse_menu(std::string const & filename);
}                                                           // namespace chips
#endif /* CHIPS_MENU_PARSE_HPP */