#ifndef CHIPS_ENTITY_METHOD_HPP
#define CHIPS_ENTITY_METHOD_HPP

# include "chips/entity/fwd.hpp"
# include <elib/aux.hpp>

namespace chips
{    
    template <class Tag, class Ret, class ...Args>
    struct method_base<Tag, Ret(Args...)> 
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
        
        /// This is magic that allows me to ensure that
        /// A given type implements method_base.
        operator detail::method_tag() const;
    };
    
    /// Define is_const
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...)>::is_const;
    
    
    /// @see Above
    template <class Tag, class Ret, class ...Args>
    struct method_base<Tag, Ret(Args...) const> 
    {
        using tag = Tag;
        using result_type = Ret;
        using signature = Ret(Args...);
        using function_type = Ret(entity const &, Args...);
        
        static constexpr bool is_const = true;
        
        operator detail::method_tag() const;
    };
    
    /// Define is_const
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...) const>::is_const;
    
    
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
    {} move_{};
    
    /// A second example of a method tag. In this case the method is CONST.
    /// meaning it does not change the entity it belongs too.
    /// NOTE: once again the actual signature of is_alive_ must be
    ///       void(entity const &)
    constexpr struct is_alive_t : method_base<is_alive_t, void() const>
    {} is_alive_{};
# endif /* CHIPS_EXAMPLE */
    
////////////////////////////////////////////////////////////////////////////////
//                          METHOD(MethodTag, Method)
////////////////////////////////////////////////////////////////////////////////
    
    namespace detail
    {
        /// The output type of chips::method(MethodTag, Method).
        /// This is an implementation detail. It's just a way to bind
        /// information so we can overload 
        /// operator<<(entity, method(MethodTag, Method))
        template <class MethodTag>
        class stored_method
        {
        public:
            using tag_type = MethodTag;
            using function_type = typename MethodTag::function_type;
        public:
            stored_method(function_type* fn_ptr)
              : m_fn_ptr(fn_ptr)
            {}
            
            ELIB_DEFAULT_COPY_MOVE(stored_method);
            
            MethodTag tag() const noexcept { return MethodTag(); }
            function_type* method() const noexcept { return m_fn_ptr; }
            
        private:
            function_type *m_fn_ptr;
        };
    }                                                       // namespace detail
    
    /// /* impl detail */ method(MethodTag, MethodPtr)
    /// this function is used to call << on entities 
    /// with methods as well as attributes. 
    /// Usage: entity << method(MethodTag, Method)
    /// NOTE: GCC workaround requires explicit cast from lambda.
    ///       Remove when no longer needed, move conversion into argument list
    template <
        class MethodTag
      , class MethodType
      , ELIB_ENABLE_IF(is_method<MethodTag>::value)
      , ELIB_ENABLE_IF(elib::aux::is_convertible<
          MethodType, typename MethodTag::function_type*
        >::value)
    >
    detail::stored_method<MethodTag> 
    method(MethodTag, MethodType m)
    {
        return detail::stored_method<MethodTag>(
            static_cast<typename MethodTag::function_type*>(m)
        );
    }
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_METHOD_HPP */