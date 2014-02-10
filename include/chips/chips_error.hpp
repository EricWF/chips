#ifndef CHIPS_CHIPS_ERROR_HPP
#define CHIPS_CHIPS_ERROR_HPP

# include <elib/aux.hpp>
# include <string>
# include <stdexcept>

namespace chips
{
    /* Handler for unexpected termination */
    void chips_terminate_handler();
    void set_terminate_handler();
    
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wweak-vtables"
# endif
    ////////////////////////////////////////////////////////////////////////////
    //
    /* This should be the base type FOR ALL exceptions thrown in chips */
    class chips_error
      : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
        
        ELIB_DEFAULT_COPY_MOVE(chips_error);
        
        virtual ~chips_error() = default;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class sdl_error
      : public chips_error
    {
    public:
        sdl_error(const char* what_arg, std::string const & err_str);
        using chips_error::chips_error;
        
        ELIB_DEFAULT_COPY_MOVE(sdl_error);
        
        virtual ~sdl_error() = default;
    };
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif
    
    ////////////////////////////////////////////////////////////////////////////
    //
    [[noreturn]] void throw_sdl_error(const char *what_arg);
    [[noreturn]] void throw_and_clear_sdl_error(const char *what_arg);
      
}                                                           // namespace chips
#endif /* CHIPS_CHIPS_ERROR_HPP */