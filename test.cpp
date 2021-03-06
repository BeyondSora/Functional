#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "fn/detail/macro.h"

#include "fn/container_utility.h"
#include "fn/each.h"
#include "fn/filter.h"
#include "fn/fmap.h"
#include "fn/lookup.h"
#include "fn/maybe.h"
#include "fn/reduce.h"
#include "fn/take.h"
#include "fn/to.h"

#include "fit/placeholders.h"

using namespace fit;
using namespace fn;
using namespace std;

template <template <typename...> class C, typename... Ts>
constexpr bool test_specialized_container_t() {
    return
        is_same<
            C<Ts...>,
            ::fn::detail::specialized_container_t<
                decltype(type<C>()),
                Ts...
            >
        >::value;
}

void test_trait() {

    //
    // Test specialized_container_t returns the right contianer type
    //

    STATIC_ASSERT(test_specialized_container_t<vector, int>());
    STATIC_ASSERT(test_specialized_container_t<vector, int*>());

    STATIC_ASSERT(test_specialized_container_t<vector, vector<int>>());
    STATIC_ASSERT(test_specialized_container_t<vector, vector<int*>>());

    STATIC_ASSERT(test_specialized_container_t<vector, vector<int>*>());
    STATIC_ASSERT(test_specialized_container_t<vector, vector<int*>*>());

    STATIC_ASSERT(test_specialized_container_t<vector, const int>());
    STATIC_ASSERT(test_specialized_container_t<vector, const int*>());

    STATIC_ASSERT(test_specialized_container_t<vector, const vector<int>>());
    STATIC_ASSERT(test_specialized_container_t<vector, const vector<int*>>());

    STATIC_ASSERT(test_specialized_container_t<vector, const vector<int>*>());
    STATIC_ASSERT(test_specialized_container_t<vector, const vector<int*>*>());

    STATIC_ASSERT(test_specialized_container_t<set, int>());
    STATIC_ASSERT(test_specialized_container_t<set, int*>());

    STATIC_ASSERT(test_specialized_container_t<set, vector<int>>());
    STATIC_ASSERT(test_specialized_container_t<set, vector<int*>>());

    STATIC_ASSERT(test_specialized_container_t<set, vector<int>*>());
    STATIC_ASSERT(test_specialized_container_t<set, vector<int*>*>());

    STATIC_ASSERT(test_specialized_container_t<set, const int>());
    STATIC_ASSERT(test_specialized_container_t<set, const int*>());

    STATIC_ASSERT(test_specialized_container_t<set, const vector<int>>());
    STATIC_ASSERT(test_specialized_container_t<set, const vector<int*>>());

    STATIC_ASSERT(test_specialized_container_t<set, const vector<int>*>());
    STATIC_ASSERT(test_specialized_container_t<set, const vector<int*>*>());

    STATIC_ASSERT(test_specialized_container_t<map, int, int>());
    STATIC_ASSERT(test_specialized_container_t<map, int, int*>());

    STATIC_ASSERT(test_specialized_container_t<map, string, const vector<int>>());
    STATIC_ASSERT(test_specialized_container_t<map, string, const vector<int*>>());

    STATIC_ASSERT(test_specialized_container_t<map, const string, vector<int>*>());
    STATIC_ASSERT(test_specialized_container_t<map, const string, vector<int*>*>());

}

int main() {

    // vector
    cout << "vector" << endl;
    vector<int> vectorValues = { 1, 2, 3, 4, 5 };

    int vectorSum = vectorValues
        | filter(_ > 1)
        | to(type<set>())
        | to(type<list>())
        | fmap(_ * 10)
        | each([] (auto val) { cout << val << endl; })
        | reduce(_ + _);

    int vectorLookupByVal  = vectorValues | lookup(4) | ok;
    int vectorLookupByFunc = vectorValues | lookup(_ == 4) | ok;
    assert(vectorLookupByVal == vectorLookupByFunc);

    auto vectorTake2 = vectorValues | take(2);
    assert(vectorTake2.size() == 2);

    int vectorFirst = vectorValues | first;
    assert(vectorFirst == 1);

    cout << vectorSum << endl;
    cout << endl;

    vector<int*> pointers;
    auto uniqueAddresses = pointers
        | to(type<set>())
        | to(type<list>());



    // list
    cout << "list" << endl;
    list<int> listValues = { 1, 2, 3, 4, 5 };

    int listSum = listValues
        | filter(_ > 1)
        | fmap(_ * 10)
        | each([] (auto val) { cout << val << endl; })
        | reduce(_ + _, 1000);

    cout << listSum << endl;
    cout << endl;



    // set
    cout << "set" << endl;
    set<int> setValues = { 1, 2, 3, 4, 5 };

    int setSum = setValues
        | filter(_ > 1)
        | fmap(_ * 10)
        | each([] (auto val) { cout << val << endl; })
        | reduce(_ + _);

    int setFirst = setValues | first;
    assert(setFirst == 1);

    cout << setSum << endl;
    cout << endl;



    // map
    cout << "map" << endl;
    map<int, int> mapValues =
        { make_pair(1, 1), make_pair(2, 2), make_pair(3, 3),
          make_pair(4, 4), make_pair(5, 5) };

    int mapSum = mapValues
        | filter(on_map_value(_ > 1))
        | fmap(on_map_value(_ * 10))
        | each(on_map_value([] (auto val) { cout << val << endl; }))
        | reduce(on_map_value(_ + _), 1000);

    auto mapTake2 = mapValues | take(2);
    assert(mapTake2.size() == 2);

    pair<int, int> mapFirst = mapValues | first;
    assert(mapFirst.first == 1);
    assert(mapFirst.second == 1);

    pair<int, int> mapLast = mapValues | last;
    assert(mapLast.first == 5);
    assert(mapLast.second == 5);

    cout << mapSum << endl;
    cout << endl;

}
