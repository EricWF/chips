#ifndef CHIPS_GAME_OBJECT_VISITOR_HPP
#define CHIPS_GAME_OBJECT_VISITOR_HPP

# include "chips/game_object/fwd.hpp"

namespace chips
{
    class game_object_visitor
    {
    public:
        game_object_visitor() noexcept = default;
        
        game_object_visitor(game_object_visitor const &) noexcept = default;
        game_object_visitor(game_object_visitor &&) noexcept = default;
        
        game_object_visitor & operator=(game_object_visitor const &) noexcept = default;
        game_object_visitor & operator=(game_object_visitor &&) noexcept = default;
        
        virtual ~game_object_visitor() noexcept = default;
        
        
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_OBJECT_VISITOR_HPP */