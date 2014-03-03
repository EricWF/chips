#ifndef CHIPS_ENTITY_HPP
#define CHIPS_ENTITY_HPP

# include "chips/attribute.hpp"
# include "chips/entity_id.hpp"
# include "chips/error.hpp"
# include <elib/any.hpp>
# include <elib/aux.hpp>
# include <elib/enumeration.hpp>
# include <elib/fmt.hpp>
# include <elib/functional.hpp> /* for functional::assign */
# include <elib/tuple.hpp>
# include <typeindex>
# include <typeinfo>
# include <unordered_map>
# include <utility> /* for std::make_pair */
# include <cstddef> /* for std::size_t */

# define CHIPS_ASSERT_ATTRIBUTE_TYPE(T) \
    static_assert(                      \
        is_attribute<T>::value          \
      , "T must be an attribute type"   \
    )
    
namespace chips
{
    template <class Attr>
    inline chips_error create_entity_access_error()
    {
        chips_error e(
            elib::fmt("entity access error with type: %s", typeid(Attr).name())
        );
        e << elib::errinfo_type_info_name(typeid(Attr).name());
        return e;
    }
    
    class entity
    {
    public:
        entity() = default;
        
        explicit entity(entity_id xid) 
          : m_id(xid)
        {}
        
        template <
            class ...Attrs
          , ELIB_ENABLE_IF(elib::and_<is_attribute<Attrs>...>::value)
        >
        explicit entity(entity_id xid, Attrs &&... attrs)
          : m_id(xid)
        {
            elib::aux::swallow(
                m_attributes.insert(
                    std::make_pair(
                        std::type_index(typeid(Attrs))
                      , elib::forward<Attrs>(attrs)
                ))...
            );
        }
        
        template <
            class ...Attrs
          , ELIB_ENABLE_IF(elib::and_<is_attribute<Attrs>...>::value)
        >
        entity(entity_id xid, elib::tuple<Attrs...> const & t)
          : m_id(xid)
        {
            *this = t;
        }
        
        ELIB_DEFAULT_COPY_MOVE(entity);        
        
        template <
            class Attr
          , ELIB_ENABLE_IF(is_attribute<Attr>::value)
        >
        entity & operator=(Attr && a)
        {
            m_attributes[std::type_index(typeid(Attr))] = elib::forward<Attr>(a);
            return *this;
        }
        
        template <
            class ...Attrs
          , ELIB_ENABLE_IF(elib::and_<is_attribute<Attrs>...>::value)
        >
        entity & operator=(elib::tuple<Attrs...> const & tp)
        {
            elib::functional::assign<> assign_fn;
            apply_tuple(assign_fn, tp);
            return *this;
        }
        
        entity_id id() const noexcept { return m_id; }
        
        std::size_t size() const
        {
            return m_attributes.size();
        }
        
        template <class Attr>
        bool has_attribute() const
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            return m_attributes.count(std::type_index(typeid(Attr)));
        }
        
        template <class Attr, class ...Args>
        bool emplace_attribute(Args &&... args)
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            return m_attributes.emplace(
                std::make_pair(
                    std::type_index(typeid(Attr))
                  , elib::any(Attr(elib::forward<Args>(args)...))
            )).second;
        }
        
        template <class Attr>
        bool insert_attribute(Attr && attr)
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            return m_attributes.insert(
                std::make_pair(
                    std::type_index(typeid(Attr))
                  , elib::any(elib::forward<Attr>(attr))
            )).second;
        }
        
        template <class Attr>
        void set_attribute(Attr && attr)
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            m_attributes[std::type_index(typeid(Attr))] = 
                elib::forward<Attr>(attr);
        }
        
        template <class Attr>
        bool remove_attribute()
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            return m_attributes.erase(std::type_index(typeid(Attr)));
        }
        
        template <class Attr>
        Attr * get_raw_attribute() const
        {
            CHIPS_ASSERT_ATTRIBUTE_TYPE(Attr);
            auto pos = m_attributes.find(std::type_index(typeid(Attr)));
            if (pos == m_attributes.end()) return nullptr;
            return elib::addressof(
                elib::any_cast<Attr &>(pos->second)
            );
        }
        
        template <class Attr>
        Attr const & get_attribute() const
        {
            auto ptr = (*this).template get_raw_attribute<Attr>();
            if (!ptr)
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<Attr>());
            }
            return *ptr;
        }
        
        template <class Attr>
        Attr & get_attribute()
        {
            auto ptr = (*this).get_raw_attribute<Attr>();
            if (!ptr)
            {
                ELIB_THROW_EXCEPTION(create_entity_access_error<Attr>());
            }
            return *ptr;
        }
        
    private:
        entity_id m_id;
        std::unordered_map<std::type_index, elib::any> m_attributes;
    };
    
    
    template <class Attr>
    bool has_attribute(entity const & e) 
    { 
        return e.template has_attribute<Attr>(); 
    }
    
    template <class Attr, class ...Args>
    bool emplace_attribute(entity & e, Args &&... args)
    {
        return e.template emplace_attribute<Attr>(elib::forward<Args>(args)...);
    }
    
    template <class Attr>
    bool insert_attribute(entity & e, Attr && a)
    {
        return e.insert_attribute(elib::forward<Attr>(a));
    }
    
    template <class Attr>
    void set_attribute(entity & e, Attr && a)
    {
        e.set_attribute(elib::forward<Attr>(a));
    }
    
    template <class Attr>
    bool remove_attribute(entity & e)
    {
        return e.remove_attribute<Attr>();
    }
    
    template <class Attr>
    Attr * get_raw_attribute(entity const & e)
    {
        return e.get_raw_attribute<Attr>();
    }
    
    template <class Attr>
    Attr const & get_attribute(entity const & e)
    {
        return e.get_attribute<Attr>();
    }
    
    template <class Attr>
    Attr & get_attribute(entity & e)
    {
        return e.get_attribute<Attr>();
    }
    
    template <class Attr, class Then>
    bool if_has_attribute(entity & e, Then && then_fn)
    {
        if (!has_attribute<Attr>(e)) return false;
        elib::forward<Then>(then_fn)(e);
        return true;
    }
    
    template <
        class Attr
      , ELIB_ENABLE_IF(is_attribute<Attr>::value)
    >
    entity & operator<<(entity & e, Attr && attr)
    {
        set_attribute(e, elib::forward<Attr>(attr));
        return e;
    }
    
    template <
        class Attr
      , ELIB_ENABLE_IF(is_attribute<Attr>::value)
    >
    entity & operator>>(entity & e, Attr & r)
    {
        r = get_attribute<Attr>(e);
        return e;
    }
    
    template <
        class Attr
      , ELIB_ENABLE_IF(is_attribute<Attr>::value)
    >
    entity const & operator>>(entity const & e, Attr & r)
    {
        r = get_attribute<Attr>(e);
        return e;
    }
    
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_HPP */