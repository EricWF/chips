#ifndef CHIPS_ERROR_HPP
#define CHIPS_ERROR_HPP

# include <elib/aux.hpp>
# include <elib/except.hpp>

namespace chips
{
    class chips_error : public elib::exception
    {
    public:
        using elib::exception::exception;
        
        ELIB_DEFAULT_COPY_MOVE(chips_error);
    };
    
    void set_terminate_handler();
    void terminate_handler();
}                                                           // namespace chips
#endif /* CHIPS_ERROR_HPP */