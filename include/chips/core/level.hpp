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
    
    enum class level_state
    {
        in_play, 
        passed, 
        failed, 
        exited
    };
    
# if defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wswitch-default"
#   endif
    inline std::string to_string(level_state l)
    {
        switch (l)
        {
            case level_state::in_play:
                return "in_play";
            case level_state::passed:
                return "passed";
            case level_state::failed:
                return "failed";
            case level_state::exited:
                return "exited";

        }
    }
# if defined(__GNUC__)
#   pragma GCC diagnostic pop
# endif
    
    class level
    {
    public:
        level(std::string xname, unsigned xchip_count, std::string help_str)
          : state(level_state::in_play)
          , m_name(elib::move(xname))
          , m_chip_count(xchip_count)
          , m_help(help_str)
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
        
        bool in_play() const noexcept { return state == level_state::in_play; }
        bool passed()  const noexcept { return state == level_state::passed;  }
        bool failed()  const noexcept { return state == level_state::failed;  }
        
        void update_lists();
        
    public:
        entity chip;
        std::vector<entity> entity_list;
        std::vector<entity> new_entity_list;
        level_state state;
        
    private:
        std::string m_name;
        unsigned m_chip_count;
        std::string m_help;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_LEVEL_HPP */