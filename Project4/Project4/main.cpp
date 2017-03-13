// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void testMap(){ //TESTING INITIAL MAP
    MyMap<string, int> GPAHolder;
    assert(GPAHolder.size() == 0);
    GPAHolder.associate("Terrence", 4);
    assert(GPAHolder.size() == 1);
    int v = *GPAHolder.find("Terrence");
    assert(v == 4);
    
    GPAHolder.associate("Terrence", 5);
    v = *GPAHolder.find("Terrence");
    assert(v == 5);
    assert(GPAHolder.size() == 1);
    
    GPAHolder.associate("Jacky", 6);
    GPAHolder.associate("Brandon", 7);
    GPAHolder.associate("Chloe", 8);
    GPAHolder.associate("Kristi", 9);
    
    v = *GPAHolder.find("Chloe");
    assert(GPAHolder.size() == 5);
    assert(v == 8);
    v = *GPAHolder.find("Shirley");
    assert (v == 0);
    
    GPAHolder.clear();
    assert(GPAHolder.size() == 0);
    
    cout << "All tests passed" << endl;
}

int main()
{
    testMap();
}
