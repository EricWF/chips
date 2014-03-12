#ifndef CHIPS_LEVEL_HPP
#define CHIPS_LEVEL_HPP

# include "chips/entity.hpp"
# include <vector>

namespace chips
{
    class level{
	public:
		level() = default;
		
		std::vector<entity> m_tiles;
		
	};
}                                                           // namespace chips
#endif /* CHIPS_LEVEL_HPP */
