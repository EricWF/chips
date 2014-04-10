#ifndef CHIPS_SOUNDS_HPP
#define CHIPS_SOUNDS_HPP

namespace chips 
{
    // forward //
    enum class sound_uid;
    
    
    void bug_die_sound();
    void teeth_die_sound();
    void chip_die_sound();
    void generic_die_sound();
    void add_inventory_sound();
    void pickup_chip_sound();
    void pistol_sound();
	void bomb_sound();

    void play_sound(sound_uid);
}
#endif
