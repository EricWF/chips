#ifndef CHIPS_GAME_HPP
#define CHIPS_GAME_HPP

# include "chips/core.hpp"
# include "chips/entity_fwd.hpp"
# include "chips/entity.hpp"
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <elib/fmt.hpp>
# include <elib/lexical_cast.hpp>
# include <map>
# include <string>
# include <vector>
# include <utility>
# include <cstddef>

namespace chips
{
    // forward //
    class inventory;
    class level;
 
////////////////////////////////////////////////////////////////////////////////
//                               MISC ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct velocity_tag {}; }

    /// A attribute that stores velocity as an unsigned int
    using velocity = any_attribute<unsigned, detail::velocity_tag>;
    
    inline std::string to_string(velocity const & v)
    {
        return "velocity = " + elib::lexical_cast<std::string>(v.value());
    }

    inline velocity to_velocity(std::string const & s)
    {
        return velocity( elib::lexical_cast<unsigned>(s) );;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct toggle_state_tag {}; }
    
    /// A boolean meant for use as a toggle switch via toggle_ notify_ binding
    using toggle_state = any_attribute<bool, detail::toggle_state_tag>;
    
    inline std::string to_string(toggle_state const & s)
    {
        return "toggle_state = " + elib::lexical_cast<std::string>(s.value());
    }
    
    inline toggle_state to_toggle_state(std::string const & s)
    {
        return toggle_state( elib::lexical_cast<bool>(s) );
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////
    namespace detail { struct bindings_tag {}; } 
    
    /// A list of pointers no entities that should be notified via notify_ call.
    using bindings = any_attribute<std::vector<entity*>, detail::bindings_tag>;

////////////////////////////////////////////////////////////////////////////////
//                                 METHODS
////////////////////////////////////////////////////////////////////////////////
  
    /// Every entity that has the possibility to change should implement this method.
    /// things like walls and floors can never change, so no overload is needed.
    /// No default provided
    struct update_m : method_base<update_m, void(level &)> {};
    
    /// for use with the toggle_switch attribute. This method sets toggle_switch
    /// Default Requires: toggle_switch
    struct toggle_m : method_base<toggle_m, void()> {};
    
    /// Call toggle_ on all the attributes in the bindings attribute.
    /// Default Requires: bindings, (bound entities require toggle_)
    struct notify_m : method_base<notify_m, void() const> {};
    
    /// Move an object in its direction by its velocity.
    /// Default requires: position, direction, velocity
    struct move_m : method_base<move_m, void()>{};
    
    /// Move an object in a given direction, N times. and turn the entity to
    /// face that direction.
    struct move_in_m : method_base<move_in_m, void(direction, unsigned)> {};
    
    /// Check if a given entity colides with you.
    struct collides_m : method_base<collides_m, bool(entity const &) const> {};
    
    /// Apply results of a collision with a given entity.
    struct on_collision_m : method_base<on_collision_m, void(entity const &)> {};
    
    /// When an item is picked up, or a lock destroyed, or whenever custom 
    /// death logic is needed, this method should be used
    struct on_death_m : method_base<on_death_m, void()> {};
    
    constexpr update_m       update_{};
    constexpr toggle_m       toggle_{};
    constexpr notify_m       notify_{};
    constexpr move_m         move_{};
    constexpr move_in_m      move_in_{};
    constexpr collides_m     collides_{};
    constexpr on_collision_m on_collision_{};
    constexpr on_death_m     on_death_{};
    
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wglobal-constructors"
# endif
    /// Default implementations are provided for some functions.
    namespace common
    {
        auto toggle_ =
        [](entity & e)
        {
            toggle_state st;
            e >> st;
            e << (st = !st);
        };
        
        auto notify_ =
        [](entity const & e)
        {
            bindings b;
            e >> b;
            for (entity * other_ptr : *b)
            {
                auto & other = *other_ptr;
                if (other && other.has_method(chips::toggle_)) 
                    other.call(chips::toggle_);
            }
        };
        
        auto move_ = 
        [](entity & e)
        {
            position p; velocity v; direction d;
            e >> p >> v >> d;
            e << move(p, d, v);
        };
        
        auto move_in_ =
        [](entity & e, direction d, unsigned n)
        {
            ELIB_ASSERT(e.has_attribute<position>()
                     && e.has_attribute<direction>());
            
            position p;
            e >> p;
            e << d << move(p, d, n);
        };
        
    }                                                       // namespace common
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif
    
////////////////////////////////////////////////////////////////////////////////
//                               INVENTORY
////////////////////////////////////////////////////////////////////////////////

    /// an inventory is an attribute that chip has.
    /// It can hold boots and keys. Keys have an associated count.
    /// It also holds the number of computer chips that chip has collected
    class inventory : attribute_base
    {
    private:
        using item_map = std::map<entity_id, unsigned>;
    public:
        inventory() = default;
        
        ELIB_DEFAULT_COPY_MOVE(inventory);
        
        /// insert the item in non-existent, otherwise increase its count
        void add_item(entity_id item);
        
        /// Decrease an items count, throw if it isn't contained
        void use_item(entity_id item);
        
        /// Remove all instances of an item
        void erase_item(entity_id item);
        
        /// Return the number of a certain item that are held
        unsigned count(entity_id item) const;
        
        /// Return if the inventory contains an item
        bool contains(entity_id item) const;
        
        /// clear the entire inventory
        void clear() { m_items.clear(); }
        
        item_map::iterator begin() { return m_items.begin(); }
        item_map::iterator end()   { return m_items.end();   }
        
        item_map::const_iterator begin() const { return m_items.begin(); }
        item_map::const_iterator end()   const { return m_items.end();   }
        
    private:
        std::map<entity_id, unsigned> m_items;
    };
    
////////////////////////////////////////////////////////////////////////////////
//                                LEVEL
////////////////////////////////////////////////////////////////////////////////

    /// TODO make this better.
    /// only access to id and chip_count are guarded since
    /// they should not be changed.
    class level
    {
    public:
        level(unsigned xid, unsigned xchip_count, std::string help_str)
          : m_id(xid), m_chip_count(xchip_count), m_help(help_str)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(level);
        
        /// The level number
        unsigned id() const noexcept { return m_id; }
        void id(unsigned xid) { m_id = xid; }
        
        /// The number of computer chips in the level
        unsigned chip_count() const noexcept { return m_chip_count; }
        void chip_count(unsigned c) { m_chip_count = c; }
        
        /// return the help text for the level.
        std::string const & help() const noexcept { return m_help; }
        
        entity chip;
        
        std::vector<entity> actors;
        std::vector<entity> items;
        std::vector<entity> base;
        
        /// TODO: maybe? It makes iteration easier.
        /// The order of entities in all is
        /// Base -> Items -> Actors
        std::vector<entity*> all;
        
        /// put every entity into the all vector
        void update_all();
    private:
        unsigned m_id;
        unsigned m_chip_count;
        std::string m_help;
    };
    

////////////////////////////////////////////////////////////////////////////////
//                              PARSING
////////////////////////////////////////////////////////////////////////////////
    
    /// Parsed from XML. The entity ID is required
    /// the rest of the properties are left as name value pairs to be parsed
    /// elsewhere 
    struct tile_properties
    {
        entity_id id;
        std::vector< std::pair<std::string, std::string> > properties;
    };
    
    /// Parse the tileset information into a [UID -> Properties] map
    std::map<unsigned, tile_properties> 
    parse_tileset(std::string const &);
    
    /// The raw level data as parsed from the XML. 
    struct parsed_level
    {
        unsigned level;
        unsigned chip_count;
        std::string help;
        
        std::vector<int> base;
        std::vector<int> items;
        std::vector<int> actors;
    };
    
    /// Parse XML output by tiled into an intermediate representation
    parsed_level parse_level(unsigned level);
    
    enum class action_type
    {
        none,
        /// bind a trigger to another entity
        bind, 
    };
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    template <>
    struct basic_enum_traits<::chips::action_type>
    {
        static const std::map<::chips::action_type, std::string> name_map;
    };
}}                                                          // namespace elib
    
namespace chips
{
    inline std::string to_string(action_type a) 
    {
        return elib::enumeration::enum_cast<std::string>(a);
    }
    
    inline action_type to_action_type(std::string const & s)
    {
        return elib::enumeration::enum_cast<action_type>(s);
    }
    
    /// The required fields to locate an entity in the level struct
    /// This is used when parsing special actions
    struct entity_location
    {
        entity_id id;
        position pos;
    };
    
    /// The type of action parsed
    struct parsed_action
    {
        /// An enumeration representing the action
        action_type action;
        
        /// The entity that requested the action
        entity_location actor;
        
        /// The entities to act on
        /// USED BY:
        ///  - bind (buttons & teleports)
        std::vector<entity_location> act_on;
    };
    
    std::vector<parsed_action> parse_actions(unsigned level);
    
////////////////////////////////////////////////////////////////////////////////
//                              CREATION
////////////////////////////////////////////////////////////////////////////////
    /// create a level from its number
    /// and the list of properties to pass to the tile factory
    level create_level(
        unsigned level_number, std::map<unsigned, tile_properties> const &
    );
    
    /// Create an entity from its tile gid and a list of entity properties
    /// NOTE: gid has not been changed, that means that 0 represents empty
    /// and it must be translated to the coorisponding key for the 
    /// properties map
    entity create_entity(
        int gid, unsigned index
      , std::map<unsigned, tile_properties> const &
    );
    
    void init_chip(entity &);
    void init_actor(entity &);
    void init_item(entity &);
    void init_base(entity &);
    
    namespace detail
    {
        void init_monster(entity & e);
        void init_wall(entity & e);
        void init_floor(entity & e);
    }
    
    void process_level(level & l);
    
    namespace detail
    {
        void process_actions(level & l);
    }
    
////////////////////////////////////////////////////////////////////////////////
//                                  MISC
////////////////////////////////////////////////////////////////////////////////

    /// Convert a position on the 32x32 grid to an index in an array
    constexpr std::size_t 
    to_index(position p) noexcept
    {
        return static_cast<std::size_t>(
            (p.x * level_height) + p.y
        );
    }
    
    /// Convert an index in an array to a position on the 32x32 grid
    constexpr position 
    to_position(std::size_t index) noexcept
    {
        return position{
            static_cast<unsigned>(index / level_height)
          , static_cast<unsigned>(index % level_height)
        };
    }

}                                                           // namespace chips
#endif /* CHIPS_GAME_HPP */