#include "chips/sounds.hpp"
#include "chips/draw.hpp"
namespace chips
{
  
  void bug_die_sound()
  { play_sound(sound_uid::bug_die); }

  void teeth_die_sound()
  { play_sound(sound_uid::teeth_die); }
  
  void chip_die_sound()
  { play_sound(sound_uid::chip_die); }
  
  void generic_die_sound()
  { play_sound(sound_uid::generic_die); }

  void add_inventory_sound()
  { play_sound(sound_uid::add_inventory); }

  void pickup_chip_sound()
  { play_sound(sound_uid::pickup_chip); }

  void pistol_sound()
  { play_sound(sound_uid::pistol); }
  
  void play_sound(sound_uid id){
    sf::Sound sound;
    auto & res = resource_manager::get();
    sound.setBuffer(res[id]);
    sound.play();
  }
  
  
}
