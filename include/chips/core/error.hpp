#ifndef CHIPS_CORE_ERROR_HPP
#define CHIPS_CORE_ERROR_HPP

# include <elib/exception.hpp>

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
        
        chips_error(chips_error const &) = default;
        chips_error(chips_error &&) = default;
        chips_error & operator=(chips_error const &) = default;
        chips_error & operator=(chips_error &&) = default;
    };
}                                                           // namespace chips
#endif /* CHIPS_CORE_ERROR_HPP */