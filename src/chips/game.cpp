#include "chips/game.hpp"

namespace chips
{
    bool game::init(const char* title, int xpos, int ypos,
                    int width, int height, Uint32 flags)
    {

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
        _window = SDL_CreateWindow(title, xpos, ypos,
                                    width, height, flags);

        if(_window)
            {
            _renderer = SDL_CreateRenderer(_window, -1, 0);

            if(_renderer)
                {
                SDL_SetRenderDrawColor(_renderer, 255, 255,
                                        255, 255);
                }
            else
                {
                puts("Renderer init failed");
                return false;
                }
            }
        else
            {
            puts("Window init failed");
            return false;
            }
        }
    else
        {
        puts("SDL init failed");
        }

    SDL_Surface* tempSurface = SDL_LoadBMP("../res/elephant.bmp");
    _texture = SDL_CreateTextureFromSurface(_renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    SDL_QueryTexture(_texture, NULL, NULL,
                    &_sourceRect.w, &_sourceRect.h);

    _destRect.x = _sourceRect.x = 0;
    _destRect.y = _sourceRect.y = 0;
    _destRect.w = 134;
    _destRect.h = 84;

    _sourceRect.w = 134;
    _sourceRect.h = 84;
        
    _running = true;

    return true;

    }

    void game::render()
    {
    SDL_RenderClear(_renderer);

    SDL_RenderCopy(_renderer, _texture, &_sourceRect, &_destRect);

    SDL_RenderPresent(_renderer);
    }


    void game::update()
    {
    _sourceRect.x = 134 * ((int) (SDL_GetTicks() / 100) % 4);
    }

    void game::handleEvents()
    {
    SDL_Event event;

    if(SDL_PollEvent(&event))
        {
        switch(event.type)
            {
            case SDL_QUIT:
            _running = false;
            break;
            default:
            break;
            }
        }
    }

    void game::clean()
    {
        SDL_DestroyWindow(_window);
        SDL_DestroyRenderer(_renderer);
        SDL_Quit();
    }

}                                                           // namespace chips


