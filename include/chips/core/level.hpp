#ifndef CHIPS_GAME_LEVEL_HPP
#define CHIPS_GAME_LEVEL_HPP

# include "chips/entity/entity.hpp"
# include <string>
# include <vector>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                                LEVEL
////////////////////////////////////////////////////////////////////////////////

    class level
    {
    public:
        level(std::string xname, unsigned xchip_count, std::string help_str)
          : m_name(elib::move(xname)), m_chip_count(xchip_count), m_help(help_str)
        {}
        
        level(level const &) = default;
        level(level &&) = default;
        level & operator=(level const &) = default;
        level & operator=(level &&) = default;
        
        /// The level number
        std::string const & name() const { return m_name; }
        void name(std::string xname) { m_name = elib::move(xname); }
        
        /// The number of computer chips in the level
        unsigned chip_count() const noexcept { return m_chip_count; }
        void chip_count(unsigned c) { m_chip_count = c; }
        
        /// return the help text for the level.
        std::string const & help() const noexcept { return m_help; }
        
        entity chip;
        std::vector<entity> entity_list;
        
    private:
        std::string m_name;
        unsigned m_chip_count;
        std::string m_help;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_LEVEL_HPP */