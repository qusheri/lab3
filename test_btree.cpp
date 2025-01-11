#include "DataStructures/BTree.h"
#include "test_btree.h"

#include <iostream>
#include <string>
#include <cassert>
#include <chrono>

void func_tests_bTree() {

    BTree<int, std::string> table;

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

    std::cout << "Functional BTree tests completed.\n";
}