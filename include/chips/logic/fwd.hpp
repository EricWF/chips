#ifndef CHIPS_LOGIC_FWD_HPP
#define CHIPS_LOGIC_FWD_HPP

namespace chips
{
    class entity;
    class level;
    struct entity_locator;
    enum class entity_id;
    enum class tile_id;
    enum class chips_state;
    enum class direction : unsigned;
    struct position;
}                                                           // namespace chips
namespace chips { namespace logic
{
    void init(entity &, level &);
    void process(entity &, level &);
    void finalize(entity &, level &);
    
    void init_chip(entity &, level &);
    void process_chip(entity &, level &);
    void finalize_chip(entity &, level &);
    
    void init_actor(entity &, level &);
    void process_actor(entity &, level &);
    void finalize_actor(entity &, level &);
    
    void init_item(entity &, level &);
    void process_item(entity &, level &);
    void finalize_item(entity &, level &);
    
    void init_base(entity &, level &);
    void process_base(entity &, level &);
    void finalize_base(entity &, level &);
    
    struct reading_order_cmp;
    struct reverse_reading_order_cmp;
    
    void clean_entity(entity &);
    bool same_position(entity const &, entity const &);
    bool at_location(entity_locator, entity const &);
    
    struct update_m;
    struct move_m;
    struct collides_m;
    struct on_collision_m;
    struct clone_m;
    
    struct CollidesWith;
    struct SamePosition;
    struct AtLocation;
    
    template <entity_id> struct OnEntity;
    
    namespace common
    {
        void move_(entity &, direction, level &);
        bool always_collides_(entity const &, entity const &);
        bool collides_with_monster_(entity const &, entity const &);
    }                                                       // namespace common
}}                                                          // namespace chips
#endif /* CHIPS_LOGIC_FWD_HPP */