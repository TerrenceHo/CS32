//
//  main.cpp
//  HW1.2
//
//  Created by Kevin on 1/23/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include "newSequence.h"
#include <cassert>
using namespace std;

int main() {
    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 5;

    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(k, v));

    // Failure if we try to insert a sixth item into b
    assert(!b.insert(5, v));

    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    assert(!a.insert(5, v));
    for (int k = 0; k < 1000; k++)
        assert(b.insert(k, v));
    a = b;
    assert(a.size() == 1000 and b.size() == 1000);
    Sequence d = b;
    assert(d.size() == 1000);
    assert(d.insert(5) == -1);

    cout << "Passed all tests" << endl;
}
