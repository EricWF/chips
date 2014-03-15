#ifndef CHIPS_ENTITY_FWD_HPP
#define CHIPS_ENTITY_FWD_HPP

# include <elib/aux.hpp>

namespace chips
{
    class entity;
    
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
        
    
    namespace detail
    {
        struct method_tag {};
    }                                                       // namespace detail
    
    template <class Tag, class Signature>
    struct method_base;
    
    template <class Tag, class Ret, class ...Args>
    struct method_base<Tag, Ret(Args...)> : public detail::method_tag 
    {
        using tag = Tag;
        using result_type = Ret;
        using signature = Ret(Args...);
        using function_type = Ret(entity &, Args...);
        
        template <class ...CallArgs>
        using can_call = elib::aux::is_same<signature, Ret(CallArgs...)>;
        
        template <class ...>
        using can_const_call = elib::false_;
        
        static constexpr bool is_const = false;
    };
    
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...)>::is_const;
    
    template <class Tag, class Ret, class ...Args>
    struct method_base<Tag, Ret(Args...) const> : public detail::method_tag
    {
        using base = method_base;
        using tag = Tag;
        using result_type = Ret;
        using signature = Ret(Args...);
        using function_type = Ret(entity const &, Args...);
        
        template <class ...CallArgs>
        using can_call = elib::false_;
        
        template <class ...CallArgs>
        using can_const_call = elib::aux::is_same<signature, Ret(CallArgs...)>;
        
        static constexpr bool is_const = true;
    };
    
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...) const>::is_const;
    
    template <class T>
    using is_method = elib::aux::is_convertible<T, detail::method_tag>;
    
    
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_FWD_HPP */