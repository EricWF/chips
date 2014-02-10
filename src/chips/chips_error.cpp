#include "chips/chips_error.hpp"

#include <elib/aux.hpp>
#include <elib/fmt.hpp>
#include <elib/assert.hpp>
#include <exception>
#include <iostream>
#include <SDL2/SDL.h>

namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    //
    void chips_terminate_handler()
    {
        if (std::current_exception() != std::exception_ptr{})
        {
            std::cerr << "Chips exiting with uncaught exception" << std::endl;
            try {
                std::rethrow_exception(std::current_exception());
            } catch (sdl_error & se) {
                std::cerr << "sdl_error: " << se.what() << std::endl;
            } catch (chips_error & ce) {
                std::cerr << "chips_error: " <<  ce.what() <<  std::endl;
            } catch (std::exception & e) {
                std::cerr <<  "std::exception: " << e.what() <<  std::endl;
            } catch (...) {
                std::cerr << "UNKNOWN EXCEPTION" << std::endl;
            }
        }
        else
        {
            std::cerr << "terminate called without active exception!" << std::endl;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    void set_terminate_handler()
    {
        std::set_terminate(&chips_terminate_handler);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    sdl_error::sdl_error(const char *what_arg, std::string const & sdl_what)
      : chips_error(elib::fmt(what_arg, sdl_what))
    { }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    [[noreturn]] void throw_sdl_error(const char *what_arg)
    {
        const char *err_str = SDL_GetError();
        ELIB_ASSERT(err_str);
        throw sdl_error{what_arg, err_str};
    }
    
    [[noreturn]] void throw_and_clear_sdl_error(const char *what_arg)
    {
        const char *err_str = SDL_GetError();
        ELIB_ASSERT(err_str);
        sdl_error e{what_arg, err_str};
        SDL_ClearError();
        throw err_str;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
}                                                           // namespace chips