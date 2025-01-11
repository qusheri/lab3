#include "test_hashtable.h"
#include "DataStructures/HashTable.h"

#include <iostream>
#include <string>
#include <random>
#include <cassert>

void func_tests_hashtable() {
    HashTable<int, std::string> table;

    table.Add(1, "test1");
    table.Add(2, "test2");
    table.Add(3, "test3");
    table.Add(4, "test4");

    assert(table.Get(1) == "test1");
    assert(table.Get(2) == "test2");
    assert(table.Get(3) == "test3");
    assert(table.Get(4) == "test4");
    assert(table.ContainsKey(2));

    table.Update(2, "test52");

    assert(table.Get(2) == "test52");

    assert(table.ContainsKey(3));
    table.Remove(3);
    assert(!table.ContainsKey(3));
    assert(table.Get(1) == "test1");
    assert(table.Get(2) == "test52");
    assert(table.Get(4) == "test4");

    assert(table.GetCount() == 3);
    assert(table.GetCapacity() == 16);

    std::cout << "Basic HashTable tests completed.\n";
}