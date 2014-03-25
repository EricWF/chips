#ifndef CHIPS_ENTITY_FILTER_HPP
#define CHIPS_ENTITY_FILTER_HPP

# include "chips/entity/fwd.hpp"
# include "chips/entity/concept.hpp"
# include <elib/aux.hpp>
# include <iterator>

namespace chips
{
    ////////////////////////////////////////////////////////////////////////////
    template<class Iterator, class ConceptType>
    class filter_iterator
    {
    private:
        using self = filter_iterator;
        using Traits = std::iterator_traits<Iterator>;
    public:
        using value_type = typename Traits::value_type;
        using reference = typename Traits::reference;
        using pointer = typename Traits::pointer;
        using difference_type = typename Traits::difference_type;
        using iterator_category = std::forward_iterator_tag;
        
    public:

        filter_iterator(ConceptType p, Iterator b, Iterator e = Iterator())
          : m_pred(p), m_pos(b), m_end(e)
        {
            satify_pred();
        }
        
        ELIB_DEFAULT_COPY_MOVE(filter_iterator);
        
        bool operator==(self const & other) const { return m_pos == other.m_pos; }
        bool operator!=(self const & other) const { return m_pos != other.m_pos; }
        
        reference operator*()  const { return *m_pos; }
        pointer   operator->() const { return m_pos.operator->(); }
        
        self & operator++() { increment(); return *this; }
        
        Iterator position() { return m_pos; }
        
    private:
        void increment() { ++m_pos; satify_pred(); }
        
        void satify_pred() 
        { 
            while (m_pos != m_end && !m_pred(*m_pos))
                ++m_pos; 
        }
        
        ConceptType m_pred;
        Iterator m_pos;
        Iterator m_end;

    private:
        static_assert(
            elib::aux::is_same<
                typename Traits::iterator_category
              , std::random_access_iterator_tag
            >::value
          , "filter_iterator currently only supports random access iterator's"
        );
    };
    
    
    ////////////////////////////////////////////////////////////////////////////
    template <class Sequence, class ConceptT>
    class filter_view
    {
    private:
        static_assert(
            is_concept<ConceptT>::value
          , "Must be a concept type"
        );
        using detected_iter  = decltype(elib::declval<Sequence>().begin());
        using detected_citer = decltype(elib::declval<Sequence>().cbegin());
    public:
        using iterator       = filter_iterator<detected_iter, ConceptT>;
        using const_iterator = filter_iterator<detected_citer, ConceptT>;
        
    public:
        explicit filter_view(Sequence & s)
          : m_seq(elib::addressof(s))
        {}
        
        filter_view(Sequence & s, ConceptT p) 
          : m_seq(elib::addressof(s)), m_pred(p)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(filter_view);
        
        filter_view & operator=(Sequence & s)  
        { 
            m_seq = elib::addressof(s); 
        }
        
        iterator begin() 
        { 
            return iterator(
                m_pred
              , m_seq->begin()
              , m_seq->end()
            ); 
        }
        
        iterator end() 
        { 
            return iterator(
                m_pred
              , m_seq->end()
              , m_seq->end()
            ); 
        }
        
        const_iterator begin() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->cbegin()
              , m_seq->cend()
            ); 
        }
        
        const_iterator end() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->cend()
              , m_seq->cend()
            ); 
        }
        
        const_iterator cbegin() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->cbegin()
              , m_seq->cend()
            ); 
        }
        
        const_iterator cend() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->cend()
              , m_seq->cend()
            ); 
        }
        
        void swap(filter_view & other) noexcept
        {
            using std::swap;
            swap(m_seq, other.m_seq);
            swap(m_pred, other.m_pred);
        }
        
    private:
        Sequence *m_seq;
        ConceptT m_pred;
    };
    
    
    template <class Seq, class ConceptT>
    void swap(
        filter_view<Seq, ConceptT> & lhs
      , filter_view<Seq, ConceptT> & rhs
      ) noexcept
    {
        lhs.swap(rhs);
    }
    
    
    template <class Seq, class ConceptT>
    auto begin(filter_view<Seq, ConceptT> & v) 
    ELIB_AUTO_RETURN( v.begin() )
    
    template <class Seq, class ConceptT>
    auto end(filter_view<Seq, ConceptT> & v)
    ELIB_AUTO_RETURN( v.end() )
    
    template <class Seq, class ConceptT>
    auto begin(filter_view<Seq, ConceptT> const & v) 
    ELIB_AUTO_RETURN( v.begin() )
    
    template <class Seq, class ConceptT>
    auto end(filter_view<Seq, ConceptT> const & v)
    ELIB_AUTO_RETURN( v.end() )
    
    ////////////////////////////////////////////////////////////////////////////
    template <class Sequence, class ConceptT>
    class reverse_filter_view
    {
    private:
        static_assert(
            is_concept<ConceptT>::value
          , "Must be a concept type"
        );
        using detected_iter = decltype(elib::declval<Sequence>().rbegin());
        using detected_citer = decltype(elib::declval<Sequence>().crbegin());
    public:
        using iterator       = filter_iterator<detected_iter, ConceptT>;
        using const_iterator = filter_iterator<detected_citer, ConceptT>;
        
    public:
        explicit reverse_filter_view(Sequence & s)
          : m_seq(elib::addressof(s))
        {}
        
        reverse_filter_view(Sequence & s, ConceptT p) 
          : m_seq(elib::addressof(s)), m_pred(p)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(reverse_filter_view);
        
        reverse_filter_view & operator=(Sequence & s)  
        { 
            m_seq = elib::addressof(s); 
        }
        
        iterator begin() 
        { 
            return iterator(
                m_pred
              , m_seq->rbegin()
              , m_seq->rend()
            ); 
        }
        
        iterator end() 
        { 
            return iterator(
                m_pred
              , m_seq->rend()
              , m_seq->rend()
            ); 
        }
        
        const_iterator begin() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->rbegin()
              , m_seq->rend()
            ); 
        }
        
        const_iterator end() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->rend()
              , m_seq->rend()
            ); 
        }
        
        const_iterator cbegin() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->rbegin()
              , m_seq->rend()
            ); 
        }
        
        const_iterator cend() const 
        { 
            return const_iterator(
                m_pred
              , m_seq->rend()
              , m_seq->rend()
            ); 
        }
        
        void swap(reverse_filter_view & other) noexcept
        {
            using std::swap;
            swap(m_seq, other.m_seq);
            swap(m_pred, other.m_pred);
        }
        
    private:
        Sequence *m_seq;
        ConceptT m_pred;
    };
    
    
    template <class Seq, class ConceptT>
    void swap(
        reverse_filter_view<Seq, ConceptT> & lhs
      , reverse_filter_view<Seq, ConceptT> & rhs
      ) noexcept
    {
        lhs.swap(rhs);
    }
    
    
    template <class Seq, class ConceptT>
    auto begin(reverse_filter_view<Seq, ConceptT> & v) 
    ELIB_AUTO_RETURN( v.begin() )
    
    template <class Seq, class ConceptT>
    auto end(reverse_filter_view<Seq, ConceptT> & v)
    ELIB_AUTO_RETURN( v.end() )
    
    template <class Seq, class ConceptT>
    auto begin(reverse_filter_view<Seq, ConceptT> const & v) 
    ELIB_AUTO_RETURN( v.begin() )
    
    template <class Seq, class ConceptT>
    auto end(reverse_filter_view<Seq, ConceptT> const & v)
    ELIB_AUTO_RETURN( v.end() )
}                                                           // namespace chips
#endif /* CHIPS_ENTITY_FILTER_HPP */