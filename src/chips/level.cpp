#include "chips/level.hpp"
#include "chips/error.hpp"
#include "chips/entity.hpp"
#include "chips/entity_factory.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>
#include <algorithm>
#include <string>


namespace chips
{
    namespace detail { namespace 
    {
        template <class CreateFn>
        void create_group(
            std::vector<int> const & raw_group
          , level_grid_t & dest
          , CreateFn Fn
          )
        {
            auto raw_it = raw_group.begin();
            for (auto & col : dest)
            {
                for (auto & cell : col)
                {
                    ELIB_ASSERT(raw_it != raw_group.end());
                    cell = Fn(*raw_it);  
                }
            }
        }
        
        
    }}                                                      // namespace detail
    
    level create_level(
        unsigned num
      , std::map<unsigned, tile_properties> const &
      )
    {
        std::string filename = elib::fmt(
            CHIPS_RESOURCE_ROOT "level%u.tmx"
          , num
        );
        
        parsed_level raw = parse_level(filename);
        level l(raw.level, raw.chip_count);
        
        
        //TODO
        return l;
    }
}                                                           // namespace chips