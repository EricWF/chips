#ifndef CHIPS_CHIPS_ERROR_HPP
#define CHIPS_CHIPS_ERROR_HPP

# include <stdexcept>

namespace chips
{
    /* Handler for unexpected termination */
    void chips_term_handler();
    void set_term_handler();
    
    /* This should be the base type FOR ALL exceptions thrown in chips */
    class chips_error
      : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
        virtual ~chips_error() = default;
    };
      
}                                                           // namespace chips
#endif /* CHIPS_CHIPS_ERROR_HPP */