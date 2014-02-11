#ifndef CHIPS_VISITORS_VOID_VISITOR_HPP
#define CHIPS_VISITORS_VOID_VISITOR_HPP

# include "chips/visitors/fwd.hpp"
# include "chips/visitors/tile_visitor.hpp"
# include "chips/tiles/fwd.hpp"

namespace chips
{
    class void_visitor
      : public tile_visitor
    {
    public:
        ELIB_DEFAULT_CLASS(void_visitor);
        virtual ~void_visitor() noexcept = default;
    };
}                                                           // namespace chips
#endif /* CHIPS_VISITORS_VOID_VISITOR_HPP */