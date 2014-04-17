#include "chips/sounds.hpp"
#include "chips/resource_manager.hpp"
#include <SFML/Audio.hpp>

namespace chips
{
	void bug_die_sound()
	{ 
        play_sound(sound_uid::bug_die); 
    }

	void teeth_die_sound()
	{ 
        play_sound(sound_uid::teeth_die); 
    }
  
	void chip_die_sound()
	{ 
        play_sound(sound_uid::chip_die); 
    }
  
	void generic_die_sound()
	{ 
        play_sound(sound_uid::generic_die); 
    }

	void add_inventory_sound()
	{ 
        play_sound(sound_uid::add_inventory); 
    }

	void pickup_chip_sound()
	{ 
        play_sound(sound_uid::pickup_chip); 
    }

	void pistol_sound()
    { 
        play_sound(sound_uid::pistol);
    }
	
	void bomb_sound()
	{ 
        play_sound(sound_uid::bomb); 
    }
		
	void play_sound(sound_uid id)
	{
		auto & res = resource_manager::get();
		res[id].play();   
	}
  
}
