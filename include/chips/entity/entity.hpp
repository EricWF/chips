#ifndef CHIPS_ENTITY_ENTITY_HPP
#define CHIPS_ENTITY_ENTITY_HPP

# include "chips/entity/fwd.hpp"
# include "chips/entity/attribute.hpp"
# include "chips/entity/method.hpp"
# include "chips/core.hpp"
# include <elib/aux.hpp>
# include <elib/any.hpp>
# include <elib/fmt.hpp>
# include <elib/container/flat_map.hpp>
# include <functional>
# include <string>
# include <typeindex>
# include <typeinfo>
# include <utility> /* for std::make_pair */
# include <cstddef>


/// The summary of the entity interface
/// This summary is provided to prevent ANYBODY from having to read the
/// metaprogramming and type-erasure that goes into entity and that 
/// which obsures its interface
# if defined(ENTITY_SUMMARY)
#   error "This should never be defined"
/**
 * The Entity class is a way to store attributes and Methods in a generic class.
 * There are 4 parts to an entity. 
 * 
 * 1) entity_id: Every entity has an entity_id. If the entity_id is not specified
 *    during the construction of the entity. the entity is constructed with a 
 *    BAD id that shows that the entity is bad. This is used for empty entities
 * 
 * 2) life: An entity is alive or dead and it can be killed. When an entity
 *    is dead it should be ignored
 * 
 * 3) Attributes: Entities can store attributes that contain information about
 *    the entity. You can check to see if an entity has an attribute.
 *    you can store and remove attributes and you can access and set attributes.
 * 
 * 4) Methods: Entities have methods that act on them and other data. Methods
 *    are defined by "MethodTags". Method tags are a dummy class type that is
 *    used as the method name and to store the signature of the method.
 *    See @entity/fwd.hpp for example usage of method tags.
 *    NOTE: A method tag only specifies the method name and interface of the method
 *          NOT the actual method. For example a monster may have a different
 *          move_ method than chip does, but they both an a "move_" method.
 * 
 *    Tags are usually defined with the syntax:
 *    constexpr struct method_t : method_base<method_t, MethodSignature> 
 *    {} method_
 * 
 *    All this does is define the MethodTag type and create a global instance
 *    of it in one statement. It is the same as this:
 *  
 *    struct method_t : method_base<method_t, MethodSignature> {};
 *    method_t method_;
 * 
 * Attributes and Methods MUST be marked as an attribute or a method before use
 * with entity. This helps ensure that entities can only carry what they are
 * supposed to carry. It also ensures only meaningful types are used as attributes
 * e.has_attribute<int> doesn't make any sense, but e.has_attribute<position> does.
 * This check is performed at compile time.
 */
    
    /// This type is used to reference entities. It should act like entity &
    using entity_ref = std::reference_wrapper<entity>;
    using entity_cref = std::reference_wrapper<entity const>;

    ////////////////////////////////////////////////////////////////////////////
    //                              ENTITY
    ////////////////////////////////////////////////////////////////////////////
    class entity
    {
    public:
        /// Constructs a dead entity 
        entity();
        
        /// Constructs an alive entity with a given id.
        entity(entity_id);
        
        /// Constructs an alive entity with an id, and a given set of attributes
        entity(entity_id, Attributes...);
        
        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////
        
        /// Get the ID of the entity.
        entity_id id() const;
        
        /// Allow an entity to be convertible to its ID
        operator entity_id() const noexcept;
        
        ////////////////////////////////////////////////////////////////////////
        //                              LIFE
        ////////////////////////////////////////////////////////////////////////
        
        using death_function = void(*)(entity &);
        
        /// Check if the entity is alive
        bool alive() const;
        
        /// Kill the entity. call the death function
        void kill();
        
        /// Set a "destructor method" that is called when the entity is killed
        void on_death(death_function fn);
        
        /// Get the current "destructor method" or null if none.
        death_function on_death() const;
        
        /// Allow alive checks using the syntax
        /// if (entity) { do stuff... }
        /// OR: if (!entity) { do stuff... }
        operator bool() const;
        
        ////////////////////////////////////////////////////////////////////////
        //                            ATTRIBUTES
        ////////////////////////////////////////////////////////////////////////
        
        /// Check if the entity has an attribute
        /// usage: e.has<Attribute>()
        template <class Attribute>
        bool has();
        
        /// Insert an attribute if it does not already exist
        /// Return true if the element was inserted, false otherwise
        template <class Attribute>
        bool insert(Attribute);
        
        /// Set an attribute in the entity. 
        /// Usage: e.insert_attribute(Attribute)
        template <class Attribute>
        void set(Attribute);
        
        /// Get a pointer to an Attribute, or nullptr if the entity does not
        /// have the given attribute
        /// Usage: e.get_raw<Attribute>()
        template <class Attribute>
        Attribute * get_raw();
        
        template <class Attribute>
        Attribute const * get_raw();
        
        /// Get a reference to an Attribute. If the entity does not have that
        /// attribute an exception is thrown.
        /// Usage: e.get<Attribute>()
        template <class Attribute>
        Attribute & get();
        
        template <class Attribute>
        Attribute const & get() const;
        
         /// Remove an attribute from the entity.
        /// Return true if the attribute was found and removed.
        /// Usage: e.remove<Attribute>()
        template <class Attribute>
        bool remove();
        
        /// Remove ALL attributes.
        /// Usage: e.clear_attributes()
        void clear_attributes();
     
        ////////////////////////////////////////////////////////////////////////
        //                           METHODS
        ////////////////////////////////////////////////////////////////////////
        /** NOTE: move_ is used only as an example when specifying usage.
         *  Any other MethodTag may be used in its place */
      
        /// Checks to see if an entity has a given method.
        /// Usage: e.has(move_)
        template <class MethodTag>
        bool has(MethodTag) const;
        
        /// Inserts a method if it does not already exist.
        /// Return true if the method was inserted
        /// Usage: e.insert(move_, move_def);
        template <class MethodTag>
        bool insert(MethodTag, MethodDef);
        
        /// Give an entity a method. Or change an existing one.
        /// NOTE: the type of the Method argument is provided by the MethodTag
        /// Usage: e.set(move_, [](entity &, int x, int y) { do stuff...})
        /// Usage NOTE: in this case the MethodType is provided as a lambda
        template <class MethodTag>
        void set(MethodTag, MethodType);
        
        /// Attempts to get the definition for a given MethodTag.
        /// null is returned if the method is not found.
        /// Usage: e.get_raw(move_);
        template <class MethodTag>
        MethodPointer get_raw(MethodTag);
        
        /// Attempts to get the definition for a given MethodTag
        /// throws if the method is not found.
        /// Usage: e.get(move_);
        template <class MethodTag>
        MethodPointer get(MethodTag);
        
        /// Remove a method if the entity has it, otherwise do nothing.
        /// Usage: e.remove(move_)
        template <class MethodTag>
        void remove(MethodTag);
        
        /// Remove ALL methods.
        /// usage: e.clear_methods()
        void clear_methods();
        
        /// Call a method. If the entity does not have that method call then
        /// throw an exception. NOTE: Methods are either const or non-const. 
        /// Attempting to call a non-const method on a const entity will result
        /// in a compile error.
        /// Usage: e(move_, 0, 0) (calls move_(entity &, 0, 0) internally)
        /// NOTE: This is the "Function call operator" It allows you to call
        /// an object using the syntax:
        ///     Object obj;
        ///     obj(...);
        template <class MethodTag, class ...MethodArguments>
        MethodTag::result_type operator()(MethodTag, MethodArguments...);
        
        template <class MethodTag, class ...MethodArguments>
        MethodTag::result_type operator()(MethodTag, MethodArguments...) const;
        
        /// call is the same as operator()(...) above.
        template <class MethodTag, class ...MethodArguments>
        MethodTag::result_type call(MethodTag, MethodArguments...);
        
        template <class MethodTag, class ...MethodArguments>
        MethodTag::result_type call(MethodTag, MethodArgs...) const;
        
        /// Call a method if the entity has said method. Otherwise do nothing.
        /// Returns true if the method was called, false otherwise.
        /// There are 2 overloads. One that takes a reference to the return type
        /// as the first parameter and one that does not. If the return type
        /// is passed in as the first parameter then the return value is stored 
        /// there
        /// Usage: e.call_if(move_, 0, 0);
        template <class MethodTag, class ...MethodArgs>
        bool call_if(MethodTag, MethodArgs...);
        
        template <class MethodTag, class ...MethodArgs>
        bool call_if(MethodTag, MethodArgs...) const;
        
        /// Overloads with return type as first parameter
        /// NOTE: MethodTag stores the return type information
        /// Usage: bool is_alive_return; e.call_if(is_alive_return, is_alive_); 
        template <class MethodTag, class ...MethodArgs>
        bool call_if(ReturnType &, MethodTag, MethodArgs...);
        
        template <class MethodTag, class ...MethodArgs>
        bool call_if(ReturnType &, MethodTag, MethodArgs...) const;
    
        ////////////////////////////////////////////////////////////////////////
        //                            MISC
        ////////////////////////////////////////////////////////////////////////
        
        /// Remove all methods and attributes.
        void clear();
        
        /// Swap this entity with another entity. This is equivalent to:
        /// entity tmp = *this;
        /// *this = other
        /// other = tmp
        void swap(entity & other);
    };
    
    ////////////////////////////////////////////////////////////////////////////
    //                            MISC FUNCTIONS
    ////////////////////////////////////////////////////////////////////////////

    /// Swap the two entities
    void swap(entity & lhs, entity & rhs)

    ////////////////////////////////////////////////////////////////////////////
    //                          ATTRIBUTE FUNCTIONS
    ////////////////////////////////////////////////////////////////////////////
    
    /// Write an attribute to an entity
    /// Usage: e << Attribute1 << Attribute2 << ...
    template <class Attribute>
    entity & operator<<(entity &, Attribute);
    
    /// Get an attribute from an entity and write it to Attribute
    /// Usage: e >> Attribute1 >> Attribute2 >> ...
    template <class Attribute>
    entity const & operator>>(entity const &, Attribute &);
    
    ////////////////////////////////////////////////////////////////////////////
    //                        METHOD FUNCTIONS
    ////////////////////////////////////////////////////////////////////////////

    template <class MethodTag>
    entity & operator <<(entity &, detail::stored_method<MethodTag>);
    
# endif /* ENTITY_SUMMARY */
    
    
namespace chips
{
    using entity_ref = std::reference_wrapper<entity>;
    using entity_cref = std::reference_wrapper<entity>;
    
    ////////////////////////////////////////////////////////////////////////////
    /// Create an "access error" for a given Attribute or Method.
    /// This type is thrown when the program attempts to access a attribute/method
    /// that is not found.
    template <class Attr>
    inline chips_error create_entity_access_error(entity_id id)
    {
        chips_error err(elib::fmt(
            "entity access error on entity %s with type: %s"
          , to_string(id), elib::aux::demangle(typeid(Attr).name())
        ));
        err << elib::errinfo_type_info_name(typeid(Attr).name());
        return err;
    }

    ////////////////////////////////////////////////////////////////////////////
    class entity
    {
    public:
        ////////////////////////////////////////////////////////////////////////
        entity()
          : m_id(entity_id::BAD_ID)
          , m_alive(false), m_on_death(nullptr)
        {}
        
        ////////////////////////////////////////////////////////////////////////
        explicit entity(entity_id xid) 
          : m_id(xid), m_alive(true), m_on_death(nullptr)
        {
            // Don't allow creation of "bad" entities
            ELIB_ASSERT(xid != entity_id::BAD_ID);
        }
        
        ////////////////////////////////////////////////////////////////////////
        // NOTE: the extra true_ argument is used to ensure and_ is passed
        // at least 2 params
        template <
            class ...Attrs
          , ELIB_ENABLE_IF(elib::and_<elib::true_, is_attribute<Attrs>...>::value)
        >
        explicit entity(entity_id xid, Attrs &&... attrs)
          : m_id(xid), m_alive(true), m_on_death(nullptr)
        {
            ELIB_ASSERT(xid != entity_id::BAD_ID);
            
            elib::aux::swallow(
                m_attributes.insert(
                    std::make_pair(
                        std::type_index(typeid(Attrs))
                      , elib::forward<Attrs>(attrs)
                ))...
            );
        }
        
        ////////////////////////////////////////////////////////////////////////
        ELIB_DEFAULT_COPY_MOVE(entity);        
        
        ////////////////////////////////////////////////////////////////////////
        entity_id id() const noexcept 
        { 
            return m_id; 
        }
        
        void id(entity_id xid) noexcept { m_id = xid; }
        operator entity_id() const noexcept { return m_id; }
        
        ////////////////////////////////////////////////////////////////////////
        using death_function = void(*)(entity &);
        
        bool alive() const noexcept { return m_alive; }
        
        explicit operator bool() const noexcept { return m_alive; }
        
        void kill()
        { 
            if (m_alive && m_on_death) m_on_death(*this);
            m_alive = false; 
        }
        
        void on_death(death_function fn) { m_on_death = fn; }
        
        death_function on_death() const { return m_on_death; }
        
        ////////////////////////////////////////////////////////////////////////
        // TODO remove this
        std::size_t size() const { return m_attributes.size(); }
    
        //====================================================================//
        //                          ATTRIBUTES                                //
        //====================================================================//
    
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        bool has() const
        {
            return m_attributes.count(std::type_index(typeid(Attr)));
        }
    
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        bool insert(Attr && attr)
        {
            auto ret = m_attributes.insert(std::make_pair(
                std::type_index(typeid(Attr))
              , elib::any(elib::forward<Attr>(attr))
            ));
            return ret.second;
        }
    
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        void set(Attr && attr)
        {
            m_attributes[std::type_index(typeid(Attr))] = 
                elib::forward<Attr>(attr);
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        Attr const * get_raw() const
        {
            return const_cast<entity &>(*this).get_raw<Attr>();
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        Attr * get_raw()
        {
            auto pos = m_attributes.find(std::type_index(typeid(Attr)));
            if (pos == m_attributes.end()) return nullptr;
            return elib::addressof(
                elib::any_cast<Attr &>(pos->second)
            );
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        Attr const & get() const
        {
            auto ptr = (*this).get_raw<Attr>();
            if (!ptr)
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<Attr>(*this));
            }
            return *ptr;
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        Attr & get()
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            auto ptr = (*this).get_raw<Attr>();
            if (!ptr)
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<Attr>(*this));
            }
            return *ptr;
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        bool remove()
        {
            return m_attributes.erase(std::type_index(typeid(Attr)));
        }
        
        void clear_attributes() { m_attributes.clear(); }
        
        //====================================================================//
        //                           METHODS                                  //
        //====================================================================//
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class MethodTag
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        bool has(MethodTag) const
        {
            return m_methods.count(std::type_index(typeid(MethodTag)));
        }
        
        template <
            class MethodTag, class MethodDef
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
          , ELIB_ENABLE_IF(elib::aux::is_convertible<
              MethodDef, typename MethodTag::function_type*
            >::value)
        >
        bool insert(MethodTag, MethodDef def)
        {
            using FnPtr = typename MethodTag::function_type*;
            auto ret = m_methods.insert(std::make_pair(
                std::type_index(typeid(MethodTag))
              , elib::any(static_cast<FnPtr>(def))
            ));
            return ret.second;
        }
        
        template <
            class MethodTag, class MethodDef
          , ELIB_ENABLE_IF(is_method<MethodTag>::value) 
          , ELIB_ENABLE_IF(
              elib::aux::is_convertible<
                MethodDef, typename MethodTag::function_type*
              >::value
            )
        >
        void set(MethodTag, MethodDef def)
        {            
            using FnPtr = typename MethodTag::function_type*;
            m_methods[std::type_index(typeid(MethodTag))] = 
                elib::any( static_cast<FnPtr>(def) );
        }
        
        template <
            class MethodTag
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        typename MethodTag::function_type*
        get_raw(MethodTag) const
        {
            auto pos = m_methods.find(std::type_index(typeid(MethodTag)));
            if (pos == m_methods.end()) return nullptr;
            return elib::any_cast<typename MethodTag::function_type*>(pos->second);
        }
        
        template <
            class MethodTag
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        typename MethodTag::function_type*
        get(MethodTag tag) const
        {
            CHIPS_ASSERT_METHOD_TYPE(MethodTag);
            typename MethodTag::function_type* fn_ptr = this->get_raw(tag);
            if (!fn_ptr)
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<MethodTag>(*this));
            }
            return fn_ptr;
        }
        
        template <
            class MethodTag
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        void remove(MethodTag)
        {
            CHIPS_ASSERT_METHOD_TYPE(MethodTag);
            m_methods.erase(std::type_index(typeid(MethodTag)));
        }
        
        void clear_methods() { m_methods.clear(); }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class MethodTag , class ...MethodArgs
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        typename MethodTag::result_type
        operator()(MethodTag, MethodArgs &&... args)
        {
            auto pos = m_methods.find(std::type_index(typeid(MethodTag)));
            if (pos == m_methods.end())
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<MethodTag>(*this));
            }
            
            using FnPtr = typename MethodTag::function_type*;
            FnPtr fn_ptr = elib::any_cast<FnPtr>(pos->second);
            
            return fn_ptr(*this, elib::forward<MethodArgs>(args)...);
        }
        
        
        // TODO: reduce code duplication for const/non-const case
        template <
            class MethodTag, class ...MethodArgs
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        typename MethodTag::result_type
        operator()(MethodTag, MethodArgs &&... args) const
        {
            static_assert(
                MethodTag::is_const
              , "Attempting to class a non-const method on a const entity"
            );
            
            auto pos = m_methods.find(std::type_index(typeid(MethodTag)));
            if (pos == m_methods.end())
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<MethodTag>(*this));
            }
            
            using FnPtr = typename MethodTag::function_type*;
            FnPtr fn_ptr = elib::any_cast<FnPtr>(pos->second);
            return fn_ptr(*this, elib::forward<MethodArgs>(args)...);
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class MethodTag, class ...Args
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        typename MethodTag::result_type
        call(MethodTag tag, Args &&... args)
        {
            return (*this)(tag, elib::forward<Args>(args)...);
        }
        
        template <
            class MethodTag, class ...Args
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        typename MethodTag::result_type
        call(MethodTag tag, Args &&... args) const
        {
            return (*this)(tag, elib::forward<Args>(args)...);
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <
            class MethodTag, class ...Args
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        bool call_if(MethodTag tag, Args &&... args)
        {
            if (!alive() || !has(tag)) return false;
            call(tag, elib::forward<Args>(args)...);
            return true;
        }
        
        template <
            class MethodTag, class ...Args
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        bool call_if(MethodTag tag, Args &&... args) const
        {
            if (!alive() || !has(tag)) return false;
            call(tag, elib::forward<Args>(args)...);
            return true;
        }
        
        template <
            class MethodTag, class ...Args
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        bool call_if(typename MethodTag::result_type & res, MethodTag tag
                   , Args &&... args)
        {
            if (!alive() || !has(tag)) return false;
            res = call(tag, elib::forward<Args>(args)...);
            return true;
        }
        
        template <
            class MethodTag, class ...Args
          , ELIB_ENABLE_IF(is_method<MethodTag>::value)
        >
        bool call_if(typename MethodTag::result_type & res, MethodTag tag
                   , Args &&... args) const
        {
            if (!alive() || !has(tag)) return false;
            res = call(tag, elib::forward<Args>(args)...);
            return true;
        }
        
        
       
        ////////////////////////////////////////////////////////////////////////
        void clear()
        {
            clear_attributes();
            clear_methods();
            m_on_death = nullptr;
        }
        
        void swap(entity & other) noexcept
        {
            using std::swap;
            swap(m_id, other.m_id);
            swap(m_alive, other.m_alive);
            swap(m_on_death, other.m_on_death);
            swap(m_attributes, other.m_attributes);
            swap(m_methods, other.m_methods);
        }
        
    private:
        entity_id m_id;
        bool m_alive;
        death_function m_on_death;
        elib::flat_map<std::type_index, elib::any> m_attributes;
        elib::flat_map<std::type_index, elib::any> m_methods;
    };                                                      // class entity
    
    ////////////////////////////////////////////////////////////////////////////
    inline void swap(entity & lhs, entity & rhs) noexcept
    {
        lhs.swap(rhs);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <
        class Attr
      , ELIB_ENABLE_IF(is_attribute<Attr>::value)
    >
    entity & operator<<(entity & e, Attr && attr)
    {
        e.set(elib::forward<Attr>(attr));
        return e;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template <
        class Attr
      , ELIB_ENABLE_IF(is_attribute<Attr>::value)
    >
    entity const & operator>>(entity const & e, Attr & r)
    {
        r = e.get<Attr>();
        return e;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    template<
        class MethodTag
      , ELIB_ENABLE_IF(is_method<MethodTag>::value)
    >
    entity & operator<<(entity & e, detail::stored_method<MethodTag> m)
    {
        e.set(m.tag(), m.method());
        return e;
    }
    
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_ENTITY_HPP */