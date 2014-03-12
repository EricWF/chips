#ifndef CHIPS_LEVEL_HPP
#define CHIPS_LEVEL_HPP

# include "chips/config.hpp"
# include "chips/entity.hpp"
# include <array>
# include <vector>

namespace chips
{
    
    using level_grid_t = 
        std::array<
            std::array<entity, level_height>
          , level_width
        >;
    
    class level
    {
	public:
		
		
	};
}                                                           // namespace chips
#endif /* CHIPS_LEVEL_HPP */
