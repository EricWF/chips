
#include "chips/chips_main.hpp"

#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <string>

//The screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 20;

//The dimensions of the dot
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

//The surfaces
SDL_Surface *dot = nullptr;
SDL_Surface *screen = nullptr;

//The event structure
SDL_Event event;



//The dot that will move around on the screen
class Dot
{
private:
  //The X and Y offsets of the dot
  int x, y;

  //The velocity of the dot
  int xVel, yVel;

public:
  //Initializes the variables
  Dot();

  //Takes key presses and adjusts the dot's velocity
  void handle_input();

  //Moves the dot
  void move();

  //Shows the dot on the screen
  void show();
};

//The timer
class Timer
{
private:
  //The clock time when the timer started
  int startTicks;

  //The ticks stored when the timer was paused
  int pausedTicks;

  //The timer status
  bool paused;
  bool started;

public:
  //Initializes variables
  Timer();

  //The various clock actions
  void start();
  void stop();
  void pause();
  void unpause();

  //Gets the timer's time
  int get_ticks();

  //Checks the status of the timer
  bool is_started();
  bool is_paused();
};

SDL_Surface *load_image( std::string filename )
{
  //The image that's loaded
  SDL_Surface* loadedImage = nullptr;

  //The optimized surface that will be used
  SDL_Surface* optimizedImage = nullptr;

  //Load the image
  loadedImage = IMG_Load( filename.c_str() );

  //If the image loaded
  if( !loadedImage )
    {
      //Create an optimized surface
      optimizedImage = SDL_DisplayFormat( loadedImage );

      //Free the old surface
      SDL_FreeSurface( loadedImage );

      //If the surface was optimized
      if( !optimizedImage )
        {
          //Color key surface
          SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY,
                           SDL_MapRGB( optimizedImage->format, 0,
                                       0xFF, 0xFF ) );
        }
    }

  //Return the optimized surface
  return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source,
                    SDL_Surface* destination, SDL_Rect* clip = nullptr )
{
  //Holds offsets
  SDL_Rect offset;

  //Get offsets
  offset.x = x;
  offset.y = y;

  //Blit
  SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
  //Initialize all SDL subsystems
  if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
      return false;
    }

  //Set up the screen
  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

  //If there was an error in setting up the screen
  if( screen == NULL )
    {
      return false;
    }

  //Set the window caption
  SDL_WM_SetCaption( "Move the Dot", NULL );

  //If everything initialized fine
  return true;
}

bool load_files()
{
  //Load the dot image
  dot = load_image( "dot.bmp" );

  //If there was a problem in loading the dot
  if( dot == NULL )
    {
      return false;
    }

  //If everything loaded fine
  return true;
}

void clean_up()
{
  //Free the surface
  SDL_FreeSurface( dot );

  //Quit SDL
  SDL_Quit();
}

Dot::Dot()
{
  //Initialize the offsets
  x = 0;
  y = 0;

  //Initialize the velocity
  xVel = 0;
  yVel = 0;
}

void Dot::handle_input()
{
  //If a key was pressed
  if( event.type == SDL_KEYDOWN )
    {
      //Adjust the velocity
      switch( event.key.keysym.sym )
        {
        case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
        case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
        case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
        case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
        }
    }
  //If a key was released
  else if( event.type == SDL_KEYUP )
    {
      //Adjust the velocity
      switch( event.key.keysym.sym )
        {
        case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
        case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
        case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
        case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
        }
    }
}

void Dot::move()
{
  //Move the dot left or right
  x += xVel;

  //If the dot went too far to the left or right
  if( ( x < 0 ) || ( x + DOT_WIDTH > SCREEN_WIDTH ) )
    {
      //move back
      x -= xVel;
    }

  //Move the dot up or down
  y += yVel;

  //If the dot went too far up or down
  if( ( y < 0 ) || ( y + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
      //move back
      y -= yVel;
    }
}

void Dot::show()
{
  //Show the dot
  apply_surface( x, y, dot, screen );
}

Timer::Timer()
{
  //Initialize the variables
  startTicks = 0;
  pausedTicks = 0;
  paused = false;
  started = false;
}

void Timer::start()
{
  //Start the timer
  started = true;

  //Unpause the timer
  paused = false;

  //Get the current clock time
  startTicks = SDL_GetTicks();
}

void Timer::stop()
{
  //Stop the timer
  started = false;

  //Unpause the timer
  paused = false;
}

void Timer::pause()
{
  //If the timer is running and isn't already paused
  if( ( started == true ) && ( paused == false ) )
    {
      //Pause the timer
      paused = true;

      //Calculate the paused ticks
      pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
  //If the timer is paused
  if( paused == true )
    {
      //Unpause the timer
      paused = false;

      //Reset the starting ticks
      startTicks = SDL_GetTicks() - pausedTicks;

      //Reset the paused ticks
      pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
  //If the timer is running
  if( started == true )
    {
      //If the timer is paused
      if( paused == true )
        {
          //Return the number of ticks when the timer was paused
          return pausedTicks;
        }
      else
        {
          //Return the current time minus the start time
          return SDL_GetTicks() - startTicks;
        }
    }

  //If the timer isn't running
  return 0;
}

bool Timer::is_started()
{
  return started;
}

bool Timer::is_paused()
{
  return paused;
}


int chips_main(int, char**, char**)
{
  //Quit flag
  bool quit = false;

  //The dot that will be used
  Dot myDot;

  //The frame rate regulator
  Timer fps;

  //Initialize
  if( init() == false )
    {
      return 1;
    }

  //Load the files
  if( load_files() == false )
    {
      return 1;
    }

  //While the user hasn't quit
  while( quit == false )
    {
      //Start the frame timer
      fps.start();

      //While there's events to handle
      while( SDL_PollEvent( &event ) )
        {
          //Handle events for the dot
          myDot.handle_input();

          //If the user has Xed out the window
          if( event.type == SDL_QUIT )
            {
              //Quit the program
              quit = true;
            }
        }

      //Move the dot
      myDot.move();

      //Fill the screen white
      SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

      //Show the dot on the screen
      myDot.show();

      //Update the screen
      if( SDL_Flip( screen ) == -1 )
        {
          return 1;
        }

      //Cap the frame rate
      if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
          SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

  //Clean up
  clean_up();

  return 0;
}
