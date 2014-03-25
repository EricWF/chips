#ifndef CHIPS_GAME_FWD_HPP
#define CHIPS_GAME_FWD_HPP

namespace chips
{
    // forward //
    enum class entity_id;
    enum class tile_id;
    class entity;
    
    class level;
    
    enum class direction : unsigned;
    
    struct position;
    
    class inventory;
    
    void init_entity(entity &, level &);
    void process_entity(entity &, level &);
    void finalize_entity(entity &, level &);
    
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
    
}                                                           // namespace chips
#endif /* CHIPS_GAME_FWD_HPP */