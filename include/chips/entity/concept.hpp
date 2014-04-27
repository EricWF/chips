#ifndef CHIPS_ENTITY_CONCEPT_HPP
#define CHIPS_ENTITY_CONCEPT_HPP

# include "chips/entity/fwd.hpp"
# include "chips/entity/entity.hpp"
# include "chips/entity/filter.hpp"
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

/// Used to reset the throw site of Concept.require(...)
# define REQUIRE_CONCEPT(Entity, ...)  \
do {                                   \
    ELIB_RETHROW_BLOCK_BEGIN()         \
    {                                  \
        __VA_ARGS__().require(Entity); \
    }                                  \
    ELIB_RETHROW_BLOCK_END()           \
} while (false)


/** WARNING: the syntax is the exposition is greatly simplified and not
 * valid C++. Do not use it as code reference.
 */
# if defined(CHIPS_EXPOSITION)
#   error CHIPS_EXPOSITION should never be defined.
/**
 * Concepts are a way to construct predicates about the world.
 * Concepts may be defined by inheriting from concept_base.
 * The interface a concept is required to provide to concept_base is:
 *    bool test(entity const &) const;
 * 
 * concept_base will then define the concept interface (via CRTP).
 * 
 * Concepts may be created by creating an instance of Concept<...>;
 * Concept<...> is a meta-concept builder. It takes concept types as either
 * template parameters OR by its constructor. If the concept is given via
 * template parameter, it is default constructed and then tested. Concepts specified
 * via template are tested first. If a concept needs to store data it can be 
 * passed to the constructor. The constructor will internally store a copy
 * of the concept as a shared pointer. 
 * NOTE: the Concept<...> constructor performs type-erasure on the passed concepts
 *      There types SHOULD NOT be used as template parameters.
 */

    /// apply logical and/or to any number of bools.
    bool concept_and(bool...);
    bool concept_or(bool...);

    /// concept_base defines the interface for a concept. 
    /// Derived must provide a non-static public member function:
    ///   bool test(entity const &) const
    template <class Derived>
    struct concept_base
    {
        /// Check if an entity satisfies Derived
        bool check(entity const &) const;
        
        /// Calls check
        bool operator()(entity const &) const;
        
        /// Checks if an entity satifies Derived. If it does not, an exception
        /// is thrown. Use with REQUIRE_CONCEPT for better error messages.
        void require(entity const &) const;
        
        /// Return true if at least one element in the sequence satisfied
        /// Derived.
        bool contains(Sequence &&) const;
        
        /// Return an iterator pointing to the first entity that 
        /// satifies Derived. If there is no such entity, return the end
        /// iterator. NOTE: the iterator type is that of the sequence.
        /// not a filter iterator.
        iterator find(Sequence &&) const;
        
        /// Find the entity is the sequence that satifies Derived. This entity
        /// must be the only entity to satisfy Derived.
        /// Throws if:
        ///  - No entity satifies Derived.
        ///  - More than one entity satifies Derived.
        entity /* const */ & get(Sequence &&) const;
        
        /// Create a "filtered view" of a sequence. filter_view DOES NOT copy
        /// the sequence and the sequence must remain valid for the lifetime
        /// of filter_view. filter_view's job is to provide forward and reverse
        /// filtered iterators over the sequence
        filter_view filter(Sequence &) const;
        filter_view filter(Sequence const &) const;
        
        reverse_filter_view rfilter(Sequence &) const;
        reverse_filter_view rfilter(Sequence const &) const;
        
        /// Create a vector of references to the entities that satisfied the
        /// predicate.
        std::vector</* entity ref */>       apply_filter(Sequence &) const;
        std::vector</* entity const ref */> apply_filter(Sequence const &) const;
        
    };

    /// Concept allows the building of meta-concepts.
    /// Concept has the concept_base interface. It takes concepts either
    /// by template parameter or by constructor.
    template <class ...ChildConcepts>
    class Concept : concept_base<Concept>
    {
    public:
        /// Construct a concept that only checks Predicates.
        Concept();
        
        /// Create a concept with instances of other concept.
        /// This checks ChildConcepts' first and then 
        /// OtherConcepts if required
        Concept(OtherConcepts...);
        
        /// Concept satifies the requirement for concept_base.
        bool test(entity & e) const;
        
        /// Swap two Concept's
        void swap(Concept &);

    private:
        // exposition //
        tuple<OtherConcepts...>  m_other_concepts;
    };
    
# endif /* CHIPS_EXPOSITION */
namespace chips
{

    ////////////////////////////////////////////////////////////////////////
    // Variadic logical AND
    template <class ...Args>
    constexpr bool concept_and(Args &&...)
    {
        static_assert(sizeof...(Args) == 0, "Args list must be empty");
        return true;
    }
        
    template <class ...Rest>
    constexpr bool concept_and(bool first, Rest&&... rest)
    {
        return first ? concept_and(elib::forward<Rest>(rest)...) : false;
    }
        
    ////////////////////////////////////////////////////////////////////////
    // Variadic logical OR
    template <class ...Args>
    constexpr bool concept_or(Args &&...)
    {
        static_assert(sizeof...(Args) == 0, "Args must be empty");
        return false;
    }
        
    template <class ...Rest>
    constexpr bool concept_or(bool first, Rest &&... rest)
    {
        return first ? true : concept_or(elib::forward<Rest>(rest)...);
    }
    
    ////////////////////////////////////////////////////////////////////////
    template <class T, ELIB_ENABLE_IF(is_attribute<T>::value)>
    bool concept_check(entity const & e)
    {
        return e.has<T>();
    }
        
    template <class T, ELIB_ENABLE_IF(is_method<T>::value)>
    bool concept_check(entity const & e)
    {
        return e.has(T{});
    }
        
    template <class T, ELIB_ENABLE_IF(is_concept<T>::value)>
    bool concept_check(entity const & e)
    {
        return T().test(e);
    }


    /// concept_base contains the concept implementation.
    /// 
    template <class Derived>
    struct concept_base 
    {
        bool check(entity const & e) const
        {
            return static_cast<Derived const &>(*this).test(e);
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
        
        template <class Seq>
        bool contains(Seq && s) const
        {
            auto v = filter(s);
            return v.begin().position() != s.end();
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
        
        template <class Sequence>
        filter_view<Sequence, Derived> 
        filter(Sequence & s) const
        {
            return filter_view<Sequence, Derived>(
                s, static_cast<Derived const &>(*this)
              );
        }
        
        template <class Sequence>
        filter_view<Sequence const, Derived> 
        filter(Sequence const & s) const
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
        
        ////////////////////////////////////////////////////////////////////////
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
        std::vector<entity_ref> 
        apply_filter(Sequence & s) const
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
        std::vector<entity_cref> 
        apply_filter(Sequence const & s) const
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
        

        operator detail::concept_tag() const;
    };

    
    ////////////////////////////////////////////////////////////////////////////
    template <class ...Preds>
    class Concept : public concept_base<Concept<Preds...>> 
    {
    public:
        Concept() = default;
        
        /// This is a fun bit of code.
        /// 1. It constructs a vector of predicates using an initializer list.
        /// 2. It preforms type erasure on each of the predicates.
        /// 3. It supports move only types!
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
        
        /// This is the interface required by concept_base.
        bool test(entity const & e) const
        {
            if (! concept_and( concept_check<Preds>(e)... ))
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
       
        /// Base class that provides virtual interface.
        /// This is the interface of the type-erased predicates passed
        /// to the constructor.
        class basic_concept_holder
        {
        public:
            basic_concept_holder() = default;
            basic_concept_holder(basic_concept_holder const &) = delete;
            virtual ~basic_concept_holder() noexcept {}
            
            /// Used to ensure safety and provide debug information.
            virtual std::type_info const & type() const = 0;
            
            /// Test the stored predicate.
            virtual bool test(entity const &) const = 0;
        };
        
        /// Derived class implements type-erasure interface.
        /// TODO: I'm somehow slicing this. using elib::any is 
        /// currently a workaround.
        template <class ConceptType>
        class concept_holder : public basic_concept_holder
        {
        public:
            template <class OtherConcept>
            concept_holder(OtherConcept && o)
              : m_store(ConceptType(elib::forward<OtherConcept>(o))) {}
           
            /// Non-copyable. 
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
            
            virtual ~concept_holder() noexcept {}
            
        private:
            
            /// calling the stored predicate should be const.
            bool test_impl(entity const & e)
            {
                return elib::any_cast<ConceptType &>(m_store)(e);
            }
            
            elib::any  m_store;
        };
        
    private:
        using ptr_vector = std::vector<std::shared_ptr<basic_concept_holder>>;
        ptr_vector m_stored_concepts;
    };
    
    template <class ...Preds>
    void swap(Concept<Preds...> & lhs, Concept<Preds...> & rhs) noexcept
    {
        lhs.swap(rhs);
    }
    
   
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_CONCEPT_HPP */
