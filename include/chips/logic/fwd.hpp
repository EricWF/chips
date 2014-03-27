#ifndef CHIPS_LOGIC_FWD_HPP
#define CHIPS_LOGIC_FWD_HPP

# include "chips/core/fwd.hpp"

namespace chips
{
    class entity;
    
    void clean_entity(entity &);
    bool same_position(entity const &, entity const &);
    bool at_location(entity_locator, entity const &);
}                                                           // namespace chips
namespace chips { namespace logic
{
    /// Create an entity using init.
    /// It sets all the entities methods and attributes to the default state.
    /// This should be the entry point for all entity creation. This function
    /// just dispatches to the below functions.
    void init(entity &, level &);
    /// Currently unused.
    void process(entity &, level &);
    /// Currently unused.
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
    
    /// In MS and Lynx version of chips challenge certain things are linked
    /// in reading order and reverse reading order. These are comparisons
    /// to allow you to sort by reading order.
    struct reading_order_cmp
    {
        bool operator()(entity const &, entity const &) const;
    };
    
    struct reverse_reading_order_cmp
    {
        bool operator()(entity const &, entity const &) const;
    };
    
    /// Compute chips state based on the state of the level.
    chips_state get_chips_state(level const &);
    
    /// Move an entity on ice or force floor. This method should be
    /// called with the ice/ff tile that entity is currently on.
    void move_on_ice(entity & e, entity const & ice, level &);
    void move_on_force_floor(entity & e, entity const & ff, level &);
    
    /// Common methods used by a large amount of entities.
    namespace common
    {
        /// move a entity in a given direction, apply
        /// the results of the collision. 
        void move_(entity &, direction, level &);
        
        /// implements collides_. two entities always colide as long
        /// as they are on the same square and they are not the same entity.
        bool always_collides_(entity const &, entity const &);
        
        /// only collides with monsters in the same square.
        bool collides_with_monster_(entity const &, entity const &);
    }                                                       // namespace common
}}                                                          // namespace chips
#endif /* CHIPS_LOGIC_FWD_HPP */