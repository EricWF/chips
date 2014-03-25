#ifndef CHIPS_CORE_ERROR_HPP
#define CHIPS_CORE_ERROR_HPP

# include <elib/aux.hpp>
# include <elib/except.hpp>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               ERROR
////////////////////////////////////////////////////////////////////////////////
    
    /// The error used in chips. It should be thrown with
    /// ELIB_THROW_EXCEPTION to get throwsite information
    class chips_error : public elib::exception
    {
    public:
        using elib::exception::exception;
        
        ELIB_DEFAULT_COPY_MOVE(chips_error);
    };
}                                                           // namespace chips
#endif /* CHIPS_CORE_ERROR_HPP */