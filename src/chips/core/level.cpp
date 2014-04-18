#include "chips/core/level.hpp"
#include <algorithm>

namespace chips
{
    void level::update_lists()
    {
        auto new_end = std::remove_if(
            entity_list.begin(), entity_list.end()
            , [](entity const & e) { return not e; }
            );
            
        entity_list.erase(new_end, entity_list.end());
            
        for (auto & ne :  new_entity_list) {
            entity_list.emplace_back(elib::move(ne));
        }
        new_entity_list.clear();
    }
}                                                           // namespace chips