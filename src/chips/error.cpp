#include "chips/error.hpp"
#include <exception>
#include <iostream>

namespace chips
{
     ////////////////////////////////////////////////////////////////////////////
    //
    void terminate_handler()
    {
        if (std::current_exception() != std::exception_ptr{})
        {
            std::cerr << "Chips exiting with uncaught exception" << std::endl;
            try {
                std::rethrow_exception(std::current_exception());
            } catch (chips_error & e) {
                std::cerr << "chips_error: " << e.what() << std::endl;
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
        std::set_terminate(&terminate_handler);
    }
}                                                           // namespace chips