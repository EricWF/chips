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
//                               ANY_ATTRIBUTE
////////////////////////////////////////////////////////////////////////////////
  
    /// Used to easily make attributes of a given type
    /// The dummy is used to make different attributes even if they store the
    /// same type.
    /// Usage: typedef any_attribute<std::string, SomeType> msg_str;
    ///        You can now use the msg_str type as an attribute that wraps
    ///        A string
    template <class T, class Dummy = void> 
    class any_attribute : attribute_base
    {
    public:
        using type = any_attribute;
        
        /// The stored type
        using value_type = T;
            
        static_assert(
            !elib::aux::is_reference<T>::value
            , "T may not be a reference type"
        );
    public:
        any_attribute() = default;
        
        ELIB_DEFAULT_COPY_MOVE(any_attribute);
            
        /// Construct from T
        any_attribute(T const & v) : m_value(v) {}
        any_attribute(T && v) : m_value(v) {}
            
        /// Assign from T
        any_attribute & operator=(T const & v) 
        {
            m_value = v;
            return *this;
        }
            
        any_attribute & operator=(T && v) 
        {
            m_value = elib::move(v);
            return *this;
        }
        
        /// Convert to T
        operator T const &() const noexcept { return m_value; }
        operator T &() noexcept             { return m_value; }
        
        /// Deref to T
        T const & operator*() const noexcept { return m_value; }
        T &       operator*() noexcept       { return m_value; }
        
        /// This magical operator
        T *       operator->()       { return elib::addressof(m_value); }
        T const * operator->() const { return elib::addressof(m_value); }
            
        /// Set T
        void set(T const & v) { m_value = v; }
        void set(T && v)      { m_value = elib::move(v); }
            
        /// Get T by reference
        T const & get() const noexcept { return m_value; }
        T &       get() noexcept       { return m_value; }
        
        /// Get T by value.
        T value() const { return m_value; }
        
        void swap(any_attribute & other) noexcept
        {
            using std::swap;
            swap(m_value, other.m_value);
        }
            
    private:
        T m_value;
    };
    
    template <class T, class Dummy>
    void swap(any_attribute<T, Dummy> & lhs, any_attribute<T, Dummy> & rhs) noexcept
    {
        lhs.swap(rhs);
    }
  
////////////////////////////////////////////////////////////////////////////////
//                                METHODS
////////////////////////////////////////////////////////////////////////////////

    
    struct method_tag {};
    
    /// Define method tags by inherting from method_base.
    /// @see entity.hpp for explanation and sample usage
    template <class Tag, class Signature>
    struct method_base;
    
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
        operator method_tag() const;
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
        
        operator method_tag() const;
    };
    
    /// Define is_const
    template <class Tag, class Ret, class ...Args>
    constexpr bool method_base<Tag, Ret(Args...) const>::is_const;
    
    
    /// Check to see if type T is a method tag
    template <class T>
    using is_method = elib::aux::is_convertible<elib::aux::uncvref<T>, method_tag>;
    
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
    >
    detail::stored_method<MethodTag> 
    method(MethodTag, MethodType m)
    {
        return detail::stored_method<MethodTag>(
            static_cast<typename MethodTag::function_type*>(m)
        );
    }
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_FWD_HPP */