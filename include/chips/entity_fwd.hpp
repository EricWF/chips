#ifndef CHIPS_ENTITY_FWD_HPP
#define CHIPS_ENTITY_FWD_HPP

# include <elib/aux.hpp>

namespace chips
{
    class entity;
    
////////////////////////////////////////////////////////////////////////////////
//                            ATTRIBUTES
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

//========================== ATTRIBUTE EXAMPLE ===============================//
# if defined(CHIPS_EXAMPLE)
#   error "CHIPS_EXAMPLE should never be defined"

    /// Inheriting from attribute_base makes a class an attribute
    struct SampleTypeOne : attribute_base
    {
        // SampleTypeOne definition
    };
    
    /// In this case enum types cant inherit from attribute base.
    /// Therefore we overload is_attribute_impl to make it an attribute
    enum class SampleTypeTwo {};
    
    namespace extension
    {
        template <>
        struct is_attribute_impl<SampleTypeTwo> : elib::true_ {};
    }
# endif 
        
////////////////////////////////////////////////////////////////////////////////
//                                METHODS
////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        struct method_tag {};
    }                                                       // namespace detail
    
    /// Define method tags by inherting from method_base.
    /// @see entity.hpp for explanation and sample usage
    template <class Tag, class Signature>
    struct method_base;
    
    template <class Tag, class Ret, class ...Args>
    struct method_base<Tag, Ret(Args...)> : public detail::method_tag 
    {
        /// The method "Tag" on "Name"
        using tag = Tag;
        
        /// The return type of the method (result_type is the standard C++ name)
        using result_type = Ret;
        
        /// The externally visible function signatures for callers
        using signature = Ret(Args...);
        
        /// The actual function signature.
        /// A "this" reference is added as the first parameter
        using function_type = Ret(entity &, Args...);
        
        /// This marks the method as non-const
        static constexpr bool is_const = false;
    };
    
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...)>::is_const;
    
    
    /// @see Above
    template <class Tag, class Ret, class ...Args>
    struct method_base<Tag, Ret(Args...) const> : public detail::method_tag
    {
        using tag = Tag;
        using result_type = Ret;
        using signature = Ret(Args...);
        using function_type = Ret(entity const &, Args...);
        
        static constexpr bool is_const = true;
    };
    
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...) const>::is_const;
    
    
    
    /// Check to see if type T is a method tag
    template <class T>
    using is_method = elib::aux::is_base_of<detail::method_tag, T>;
    
//============================= METHOD EXAMPLE ===============================//
# if defined(CHIPS_EXAMPLE)
    /// An example method tag
    /// inheriting from method_base gives all the information the entity
    /// needs to store and call the method.
    /// The first argument to method_base is the tag name (i.e. move_)
    /// The second argument to method_base is the signature of the function.
    /// In this case move_ takes two ints (x, y) to move the entity to and returns
    /// void.
    /// NOTE: the actual signature of the method move_ must be
    ///       void(entity &, int, int). The entity handles passing the method
    ///       a reference to itself
    constexpr struct move_t : method_base<move_t, void(int, int)> 
    {} move_;
    
    /// A second example of a method tag. In this case the method is CONST.
    /// meaning it does not change the entity it belongs too.
    /// NOTE: once again the actual signature of is_alive_ must be
    ///       void(entity const &)
    constexpr struct is_alive_t : method_base<is_alive_t, void() const>
    {} is_alive_;
# endif /* CHIPS_EXAMPLE */
    
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_FWD_HPP */