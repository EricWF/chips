#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"

#include <iostream>

using namespace chips;

using IsChip = EntityIs<entity_id::chip>;
using IsTank = EntityIs<entity_id::tank>;

BOOST_AUTO_TEST_SUITE(chips_entity_test_suite)

BOOST_AUTO_TEST_CASE(chips_entity_ctor_test)
{
    // default ctor
    {
        entity e;
   
    }
    // ctor (entity_id)
    {
        entity e(entity_id::chip);
        BOOST_CHECK(e.id() == entity_id::chip);
        BOOST_CHECK(e.size() == 0);
    }
    // ctor (entity_id, Attrs...) lvalues
    {
        position p{0, 0};
        direction d = direction::N;
        entity e(entity_id::chip, p, d);
        BOOST_CHECK(e.id() == entity_id::chip);
        BOOST_CHECK(e.size() == 2);
        BOOST_CHECK(e.has<position>());
        BOOST_CHECK(e.has<direction>());
    }
    // ctor (entity_id, Attrs...) rvalues
    {
        entity e(entity_id::chip, position{0, 0}, direction::N);
        BOOST_CHECK(e.id() == entity_id::chip);
        BOOST_CHECK(e.size() == 2);
        BOOST_CHECK(e.has<position>());
        BOOST_CHECK(e.has<direction>());
    }
}



BOOST_AUTO_TEST_CASE(concept_check)
{
    entity e(entity_id::chip);
    e << position() << direction::N;
    e << toggle_state() << method(toggle_, common::toggle_);
    
    BOOST_CHECK(Alive().check(e));
    BOOST_CHECK(!Dead().check(e));
    BOOST_CHECK(IsChip().check(e));
    BOOST_CHECK(!IsTank().check(e));
    
    using MetaConcept = Concept<Alive, IsChip
                             , position, direction
                             , toggle_state, toggle_m
                             >;
    BOOST_CHECK(MetaConcept().check(e));
}

BOOST_AUTO_TEST_CASE(filter_test)
{
    std::vector<entity> v = 
        {
            entity(entity_id::chip, position())
          , entity(entity_id::wall)
        };
        
    
    auto fv = IsChip().apply_filter(v.begin(), v.end());
    
    BOOST_CHECK(!IsChip().check(v[1]));
    BOOST_CHECK(fv.size() == 1);
    BOOST_CHECK(&((entity &)fv[0]) == &v[0]);
}


BOOST_AUTO_TEST_CASE(filtered_view_test)
{
    std::vector<entity> v = 
        {
            entity(entity_id::chip, position())
          , entity(entity_id::wall)
        };
        
    int count = 0;
    for (auto & e : IsChip().filter(v))
    {
        ++count;
        ELIB_ASSERT(&e == &v[0]);
    }
    BOOST_CHECK(count == 1);
}

BOOST_AUTO_TEST_SUITE_END()