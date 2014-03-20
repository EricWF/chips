#ifndef CHIPS_ENTITY_FWD_HPP
#define CHIPS_ENTITY_FWD_HPP

# include <elib/aux.hpp>

/// The two macros just ensure that a type is either an attribute or a method
///  but not both
# define CHIPS_ASSERT_ATTRIBUTE_TYPE(Type)                                 \
    static_assert(is_attribute<Type>::value, "Type must be an attribute"); \
    static_assert(!is_method<Type>::value, "An attribute cannot also be a method")
    
# define CHIPS_ASSERT_METHOD_TYPE(Type)                                 \
    static_assert(is_method<Type>::value, "Type must be a method tag"); \
    static_assert(!is_attribute<Type>::value, "A Method may not be an attribute")

namespace chips
{
    class entity;
    
////////////////////////////////////////////////////////////////////////////////
//                              Attribute
////////////////////////////////////////////////////////////////////////////////

     /// To mark a type as an attribute it either needs to inherit from 
    /// attribute_base OR it needs to overload the is_attribute_impl class
    /// in the extension namespace
    struct attribute_base {};
    
    namespace extension
    {
        template <class T>
        struct is_attribute_impl
          : elib::aux::is_base_of<attribute_base, T>::type
        {};
    }
    
    /// Check to see if type T is an attribute
    template <class T>
    using is_attribute = typename 
        extension::is_attribute_impl<elib::aux::uncvref<T>>::type;
    
////////////////////////////////////////////////////////////////////////////////
//                               Method
////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        struct method_tag {};
    }
    
     /// Check to see if type T is a method tag
    template <class T>
    using is_method = typename
        elib::aux::is_convertible<
            elib::aux::uncvref<T>, detail::method_tag
        >::type;
    
    /// Define method tags by inherting from method_base.
    /// @see entity.hpp for explanation and sample usage
    template <class Tag, class Signature>
    struct method_base;
    
////////////////////////////////////////////////////////////////////////////////
//                              Concept
////////////////////////////////////////////////////////////////////////////////
  
    namespace detail
    {
        struct concept_tag {};
    }
    
    template <class T>
    using is_concept = typename elib::aux::is_convertible<T, detail::concept_tag>;
    
    template <class Derived> struct concept_base;
    
////////////////////////////////////////////////////////////////////////////////
//                              FILTER
////////////////////////////////////////////////////////////////////////////////

    template <class Sequence, class ConceptType> 
    class filter_view;
    
    template <class Iterator, class ConceptType>
    class filter_iterator;
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_FWD_HPP */