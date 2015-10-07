#define BOOST_TEST_MODULE RouteFinderTest

#include <boost/test/unit_test.hpp>
#include "router/RouteFinder.h"
#include "loaders/IConnectionLoader.h"
#include "utils/SystemTypeGetter.h"

class MockTypeGetter : public SystemTypeGetter
{
public:
    std::string getType(int id)
    {
        return "";
    }
};

class MockLoader : public IConnectionLoader
{
protected:
    void loadConnection(VertexMap& map)
    {
        MockTypeGetter typeGetter = MockTypeGetter();
        addConnection(map, &typeGetter, 1, 0);
        addConnection(map, &typeGetter, 1, 2);
        addConnection(map, &typeGetter, 2, 3);
        addConnection(map, &typeGetter, 0, 4);
    }
};

std::vector<IConnectionLoader*> getLoaders()
{
    MockLoader loader;
    std::vector<IConnectionLoader*> loaders;
    loaders.push_back(&loader);
    return loaders;
}  

BOOST_AUTO_TEST_CASE( SimpleRoute )
{
    std::vector<IConnectionLoader*> loaders = getLoaders();
    RouteFinder routeFinder(loaders);

    std::vector<int> out = routeFinder.findRoute(0,1);
    
    BOOST_CHECK_EQUAL(2, out.size());
    BOOST_CHECK_EQUAL(0, out[0]);
    BOOST_CHECK_EQUAL(1, out[1]);
}

BOOST_AUTO_TEST_CASE( LongerRoute )
{
    std::vector<IConnectionLoader*> loaders = getLoaders();
    RouteFinder routeFinder(loaders);

    std::vector<int> out = routeFinder.findRoute(0,3);

    BOOST_CHECK_EQUAL(4, out.size());
    BOOST_CHECK_EQUAL(0, out[0]);
    BOOST_CHECK_EQUAL(1, out[1]);
    BOOST_CHECK_EQUAL(2, out[2]);
    BOOST_CHECK_EQUAL(3, out[3]);
}

BOOST_AUTO_TEST_CASE( NoRoute )
{
    std::vector<IConnectionLoader*> loaders = getLoaders();
    RouteFinder routeFinder(loaders);

    std::vector<int> out = routeFinder.findRoute(0,23);
    
    BOOST_CHECK_EQUAL(0, out.size());
}

BOOST_AUTO_TEST_CASE( StartIsFinish )
{
    std::vector<IConnectionLoader*> loaders = getLoaders();
    RouteFinder routeFinder(loaders);

    std::vector<int> out = routeFinder.findRoute(2,2);

    BOOST_CHECK_EQUAL(1, out.size());
    BOOST_CHECK_EQUAL(2, out[0]);
}   
