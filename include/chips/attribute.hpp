#ifndef CHIPS_ATTRIBUTE_HPP
#define CHIPS_ATTRIBUTE_HPP

# include <elib/aux.hpp>

namespace chips
{
    struct attribute_base {};
    
    namespace extension
    {
        template <class T>
        struct is_attribute_impl
          : elib::aux::is_base_of<attribute_base, T>::type
        {};
    }
    
    template <class T>
    using is_attribute = typename 
        extension::is_attribute_impl<elib::aux::uncvref<T>>::type;
    
}                                                           // namespace chips
#endif /* CHIPS_ATTRIBUTE_HPP */