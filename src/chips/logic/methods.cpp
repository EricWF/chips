#include "chips/logic.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"

namespace chips { namespace logic 
{
    namespace common
    {
        void move_(entity & self, direction d, level & l)
        {
            ELIB_ASSERT(self);
            position old_pos = self.get<position>();
            
            self << d << move(old_pos, d, 1);
            
            for (auto & e : SamePosition(self).filter(l.entity_list))
            {
                e.call_if(on_collision_, self, l);
                if (!self) return;
                if (e.has(collides_) && e(collides_, self))
                {
                    self << old_pos;
                    return;
                }
            }
        }
        
        bool always_collides_(entity const & self, entity const & other)
        {
            return same_position(self, other);
        }
        
        bool collides_with_monster_(entity const & self, entity const & other)
        {
            return same_position(self, other)
                 && is_monster(other);
        }
    }                                                       // namespace common
}}                                                          // namespace chips