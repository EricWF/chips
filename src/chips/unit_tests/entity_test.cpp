#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "chips/attribute.hpp"
#include "chips/chips_state.hpp"
#include "chips/entity.hpp"
#include "chips/entity_id.hpp"
#include "chips/position.hpp"

using namespace chips;

BOOST_AUTO_TEST_SUITE(chips_entity_test_suite)

BOOST_AUTO_TEST_CASE(chips_entity_ctor_test)
{
    // default ctor
    {
        entity e;
        BOOST_CHECK(e.size() == 0);
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
        BOOST_CHECK(e.has_attribute<position>());
        BOOST_CHECK(e.has_attribute<direction>());
    }
    // ctor (entity_id, Attrs...) rvalues
    {
        entity e(entity_id::chip, position{0, 0}, direction::N);
        BOOST_CHECK(e.id() == entity_id::chip);
        BOOST_CHECK(e.size() == 2);
        BOOST_CHECK(e.has_attribute<position>());
        BOOST_CHECK(e.has_attribute<direction>());
    }
}


BOOST_AUTO_TEST_SUITE_END()