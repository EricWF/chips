#ifndef CHIPS_ENTITY_ATTRIBUTE_HPP
#define CHIPS_ENTITY_ATTRIBUTE_HPP

# include "chips/entity/fwd.hpp"
# include <elib/aux.hpp>
# include <utility> /* for std::swap */



namespace chips
{       
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
    
# define ANY_ATTRIBUTE_OP(Op)                                             \
    template <class T, class Dummy>                                       \
    bool operator Op (any_attribute<T, Dummy> const & lhs                 \
                  , any_attribute<T, Dummy> const & rhs)                  \
    {                                                                     \
        return lhs.get() Op rhs.get();                                    \
    }                                                                     \
                                                                          \
    template <class T, class Dummy>                                       \
    bool operator Op (any_attribute<T, Dummy> const & lhs, T const & rhs) \
    {                                                                     \
        return lhs.get() Op rhs;                                          \
    }                                                                     \
                                                                          \
    template <class T, class Dummy>                                       \
    bool operator Op (T const & lhs, any_attribute<T, Dummy> const & rhs) \
    {                                                                     \
        return lhs Op rhs.get();                                          \
    }                                                                     \
                                                                          \
    template <class T, class Dummy1, class Dummy2>                        \
    bool operator Op (any_attribute<T, Dummy1> const &                    \
                    , any_attribute<T, Dummy2> const &) = delete;
    
    ANY_ATTRIBUTE_OP( == )
    ANY_ATTRIBUTE_OP( != )
    ANY_ATTRIBUTE_OP( <  )
    ANY_ATTRIBUTE_OP( <= )
    ANY_ATTRIBUTE_OP( >  )
    ANY_ATTRIBUTE_OP( >= )
    
# undef ANY_ATTRIBUTE_OP

}                                                           // namespace chips
#endif /* CHIPS_ENTITY_ATTRIBUTE_HPP */