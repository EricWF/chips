#ifndef CHIPS_RESOURCE_MANAGER_HPP
#define CHIPS_RESOURCE_MANAGER_HPP

# include <SFML/Graphics.hpp>
# include <SFML/Audio.hpp>
# include <map>

namespace chips
{
  enum class tile_id;
    
  ////////////////////////////////////////////////////////////////////////////////
  //                           RESOURCE MANAGER
  ////////////////////////////////////////////////////////////////////////////////
    
    enum class texture_uid
    {
        none, tiles, menu, scoreboard
    };
        
    enum class font_uid
    {
        none, arial
    };
        
    enum class sound_uid
    { 
        none, 
        bug_die, 
        teeth_die, 
        chip_die, 
        generic_die,
        add_inventory, 
        pickup_chip, 
        pistol, 
        bomb
    };
    
    enum class music_uid
    {
        none, 
        theme
    };

  class resource_manager
  {
  private:
    static resource_manager & get_impl(const char* tileset_str = nullptr);

  public:
    static resource_manager & init(std::string const &);
    static resource_manager & get();
        
        
    // creates font if not found
    const sf::Font & operator[](font_uid);
    // creates sound and buffer if not found
    sf::Sound & operator[](sound_uid id);
    sf::Music & operator[](music_uid id);
    // throws if font not found
    const sf::Font & at(font_uid id) const;
        
    sf::Texture const & operator[](texture_uid) const;
    sf::Sprite & operator[](tile_id);
        
    sf::Sprite & scoreboard()
    { return m_scoreboard; }
        
  private:
    resource_manager(const char* tileset_name);
    ~resource_manager() = default;
        
    void init_tileset(const char* tileset_name);
    void init_scoreboard(const char* tileset_name);
    void init_fonts();
    void create_sprite(tile_id);
    
  private:
        
    std::map<font_uid, sf::Font> m_font_map;
    
    std::map<sound_uid, sf::SoundBuffer> m_sb_map;
    std::map<sound_uid, sf::Sound> m_sound_map;
    std::map<music_uid, sf::Music> m_music_map;
    
    std::map<texture_uid, sf::Texture> m_tex_map;
    std::map<tile_id, sf::Sprite> m_sprite_map;
    
    sf::Sprite m_scoreboard;
  };
}                                                           // namespace chips
#endif /* CHIPS_RESOURCE_MANAGER_HPP */