#ifndef CHIPS_ENTITY_CONCEPT_HPP
#define CHIPS_ENTITY_CONCEPT_HPP

# include "chips/entity/fwd.hpp"
# include "chips/entity/entity.hpp"
# include "chips/entity/filter.hpp"
# include "chips/core.hpp"
# include <elib/aux.hpp>
# include <elib/any.hpp>
# include <elib/fmt.hpp>
# include <algorithm>
# include <functional>
# include <iterator>
# include <memory>
# include <string>
# include <typeinfo>
# include <vector>

# define REQUIRE_CONCEPT(Entity, ...)  \
do {                                   \
    ELIB_RETHROW_BLOCK_BEGIN()         \
    {                                  \
        __VA_ARGS__().require(Entity); \
    }                                  \
    ELIB_RETHROW_BLOCK_END()           \
} while (false)


namespace chips
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////////////////
        // Variadic logical AND
        template <class ...Args>
        constexpr bool test_and(Args &&...)
        {
            static_assert(sizeof...(Args) == 0, "Args list must be empty");
            return true;
        }
        
        template <class ...Rest>
        constexpr bool test_and(bool first, Rest&&... rest)
        {
            return first ? test_and(rest...) : false;
        }
        
        ////////////////////////////////////////////////////////////////////////
        // Variadic logical OR
        template <class ...Args>
        constexpr bool test_or(Args &&...)
        {
            static_assert(sizeof...(Args) == 0, "Args must be empty");
            return false;
        }
        
        template <class ...Rest>
        constexpr bool test_or(bool first, Rest &&... rest)
        {
            return first ? true : test_or(rest...);
        }
        
        ////////////////////////////////////////////////////////////////////////
        template <class T, ELIB_ENABLE_IF(is_attribute<T>::value)>
        bool test_impl(entity const & e)
        {
            return e.has<T>();
        }
        
        template <class T, ELIB_ENABLE_IF(is_method<T>::value)>
        bool test_impl(entity const & e)
        {
            return e.has(T{});
        }
        
        template <class T, ELIB_ENABLE_IF(is_concept<T>::value)>
        bool test_impl(entity const & e)
        {
            return T().test(e);
        }
    }                                                       // namespace detail

    ////////////////////////////////////////////////////////////////////////////
    template <class Derived>
    struct concept_base 
    {
        bool check(entity const & e) const
        {
            return static_cast<Derived const*>(this)->test(e);
        }
        
        bool operator()(entity const & e) const
        {
            return check(e);
        }
        
        void require(entity const & e) const
        {
            if (!check(e))
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "Entity %s does not meet the concept %s"
                  , to_string(e.id()), elib::aux::demangle(typeid(Derived).name())
                )));
            }
        }
        
        template <class Iter>
        auto apply_filter(Iter b, Iter e) const
          -> std::vector<decltype(std::ref(*b))> 
        {
            using ref_type = decltype(std::ref(*b));
            std::vector<ref_type> filtered;
            std::copy_if(
                b, e
              , std::back_inserter(filtered)
              , static_cast<Derived const &>(*this)
            );
            return filtered;
        }
        
        template <class Sequence>
        std::vector<entity_ref> apply_filter(Sequence & s) const
        {
            using std::begin; using std::end;
            std::vector<entity_ref> filtered;
            std::copy_if(
                begin(s), end(s)
              , std::back_inserter(filtered)
              , static_cast<Derived const &>(*this)
            );
            return filtered;
        }
        
        template <class Sequence>
        std::vector<entity_cref> apply_filter(Sequence const & s) const
        {
            using std::begin; using std::end;
            std::vector<entity_cref> filtered;
            std::copy_if(
                begin(s), end(s)
              , std::back_inserter(filtered)
              , static_cast<Derived const &>(*this)
            );
            return filtered;
        }
        
        template <class Sequence>
        filter_view<Sequence, Derived> filter(Sequence & s) const
        {
            return filter_view<Sequence, Derived>(
                s, static_cast<Derived const &>(*this)
              );
        }
        
        
        
        template <class Sequence>
        filter_view<Sequence const, Derived> filter(Sequence const & s) const
        {
            return filter_view<Sequence const, Derived>(
                s, static_cast<Derived const &>(*this)
              );
        }
        
        template <class Sequence>
        reverse_filter_view<Sequence, Derived>
        rfilter(Sequence & s) const
        {
            return reverse_filter_view<Sequence, Derived>(
                s, *static_cast<Derived const &>(*this)
              );
        }
        
        template <class Sequence>
        reverse_filter_view<Sequence const, Derived> 
        rfilter(Sequence const & s) const
        {
            return reverse_filter_view<Sequence const, Derived>(
                s, static_cast<Derived const &>(*this)
              );
        }
        
        template <class Sequence>
        auto find(Sequence && s) const -> decltype( s.begin() )
        {
            return filter(s).begin().position();
        }
        
        template <class Sequence>
        auto get(Sequence && s) const -> decltype( s.front() )
        {
            auto view = filter(s);
            auto b = view.begin(); 
            auto e = view.end();
            if (b == e)
            {
                ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                    "Failed to find entity matching concept %s"
                  , elib::aux::demangle(typeid(Derived).name())
                )));
            }
            
            return *b;
        }
        
        
        template <class Seq>
        bool contains(Seq && s) const
        {
            auto v = filter(s);
            return v.begin().position() != s.end();
        }
        
        operator detail::concept_tag() const;
    };

    
    ////////////////////////////////////////////////////////////////////////////
    template <class ...Preds>
    class Concept : public concept_base<Concept<Preds...>> 
    {
    public:
        Concept() = default;
        
        template <
            class ...OtherPreds
          , ELIB_ENABLE_IF(sizeof...(OtherPreds) > 0)
        >
        Concept(OtherPreds &&... opreds)
          : m_stored_concepts{
              std::static_pointer_cast<basic_concept_holder>(
                  std::make_shared<concept_holder<OtherPreds>>(
                      elib::forward<OtherPreds>(opreds)
                    ))...
           }
        {
        }
        
        ELIB_DEFAULT_COPY_MOVE(Concept);
        
        bool test(entity const & e) const
        {
            if (! detail::test_and( detail::test_impl<Preds>(e)... ))
                return false;

            for (auto & s : m_stored_concepts)
            {
                ELIB_ASSERT(s.get());
                if (!s->test(e)) return false;
            }
            return true;
        }
        
        void swap(Concept & other) noexcept
        {
            m_stored_concepts.swap(other.m_stored_concepts);
        }
        
    private:
       
        class basic_concept_holder
        {
        public:
            basic_concept_holder() = default;
            basic_concept_holder(basic_concept_holder const &) = delete;
            virtual ~basic_concept_holder() {}
            virtual std::type_info const & type() const = 0;
            virtual bool test(entity const &) const = 0;
        };
        
        // TODO 
        template <class ConceptType>
        class concept_holder : public basic_concept_holder
        {
        public:
            template <class OtherConcept>
            concept_holder(OtherConcept && o)
              : m_store(ConceptType(elib::forward<OtherConcept>(o))) {}
           
            concept_holder(concept_holder const &) = delete;
            concept_holder & operator=(concept_holder const &) = delete;
            concept_holder & operator=(concept_holder &&) = delete;

            std::type_info const & type() const
            {
                return typeid(ConceptType);
            }
            
            // workaround in for issue with const in any.
            bool test(entity const & e) const
            {
               return const_cast<concept_holder *>(this)->test_impl(e);
            }
            
            ~concept_holder() {}
            
        private:
            bool test_impl(entity const & e)
            {
                return elib::any_cast<ConceptType &>(m_store)(e);
            }
            
            elib::any  m_store;
        };
        
    private:
        std::vector<std::shared_ptr<basic_concept_holder>> m_stored_concepts;
    };
    
    template <class ...Preds>
    void swap(Concept<Preds...> & lhs, Concept<Preds...> & rhs) noexcept
    {
        lhs.swap(rhs);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    struct Alive : public concept_base<Alive>
    {
        bool test(entity const & e) const
        {
            return bool(e);
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    struct Dead : concept_base<Dead>
    {
        bool test(entity const & e) const
        {
            return !bool(e);
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <entity_id ...Ids>
    struct EntityIs : concept_base<EntityIs<Ids...>>
    {
        bool test(entity const & e) const
        {
            return detail::test_or(
                (e.id() == Ids)...
            );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <entity_id ...Ids>
    struct EntityIsNot : concept_base<EntityIsNot<Ids...>>
    {
        bool test(entity const & e) const
        {
            return detail::test_and(
                (e.id() != Ids)...
            );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    template <class ...AttrOrMeth>
    struct EntityHas : concept_base<EntityHas<AttrOrMeth...>>
    {
        bool test(entity const & e) const
        {
            return detail::test_and(
                detail::test_impl<AttrOrMeth>(e)...
              );
        }
    };
    
    template <class ...AttrOrMeth>
    struct EntityHasAny : concept_base<EntityHasAny<AttrOrMeth...>>
    {
        bool test(entity const & e) const
        {
            return detail::test_or(
                detail::test_impl<AttrOrMeth>(e)...
              );
        }
    };
    
    template <class ...AttrOrMeth>
    struct EntityHasNone : concept_base<EntityHasNone<AttrOrMeth...>>
    {
        bool test(entity const & e) const
        {
            return !detail::test_or(
                detail::test_impl<AttrOrMeth>(e)...
              );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        using query_fn_ptr = bool(*)(entity_id);
    }

    template <detail::query_fn_ptr ...Querys>
    struct EntityMatches : concept_base<EntityMatches<Querys...>>
    {
        bool test(entity const & e) const
        {
            return detail::test_and(
                Querys(e.id())...
            );
        }
    };
    
    template <detail::query_fn_ptr ...Querys>
    struct EntityMatchesAny : concept_base<EntityMatchesAny<Querys...>>
    {
        bool test(entity const & e) const
        {
            return detail::test_or(
                Querys(e.id())...
            );
        }
    };

    template <detail::query_fn_ptr ...Querys>
    struct EntityMatchesNone : concept_base<EntityMatchesNone<Querys...>>
    {
        bool test(entity const & e) const
        {
            return !detail::test_or(
                Querys(e.id())...
            );
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        using pred_type = bool(*)(entity const &);
    }
    
    struct Predicate : concept_base<Predicate>
    {
        Predicate(std::function<bool(entity const &)> fn)
          : m_fn(fn)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(Predicate);
        
        bool test(entity const & e) const
        {
            return m_fn(e);
        }

    private:
        std::function<bool(entity const &)> m_fn;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    struct AtPosition : concept_base<AtPosition>
    {
        AtPosition(position p) : m_pos(p) {}
        
        ELIB_DEFAULT_COPY_MOVE(AtPosition);
        
        bool test(entity const & e) const
        {
            return (bool(e) && e.has<position>() 
                  && e.get<position>() == m_pos);
        }

    private:
        position m_pos;
    };
    
    struct AtEntity : concept_base<AtEntity>
    {
        AtEntity(entity const & e)
        {
            ELIB_ASSERT(e.has<position>());
            m_pos = e.get<position>();
        }

        bool test(entity const & e) const
        {
            if (!e || !e.has<position>()) return false;
            return e.get<position>() == m_pos;
        }
        
    private:
        position m_pos;
    };
    
    struct HasID : concept_base<HasID>
    {
        HasID(entity_id id) : m_id(id) {}
        
        ELIB_DEFAULT_COPY_MOVE(HasID);
        
        bool test(entity const & e) const
        {
            return e.id() == m_id;
        }

    private:
        entity_id m_id;
    };
    
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_CONCEPT_HPP */
