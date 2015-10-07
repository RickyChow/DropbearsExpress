#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MainTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( BoostTestSanityCheck )
{
    BOOST_CHECK(true);
}

