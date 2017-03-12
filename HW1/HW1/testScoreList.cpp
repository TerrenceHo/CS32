//
//  testScoreList.cpp
//  HW1
//
//  Created by Kevin on 1/22/17.
//  Copyright © 2017 Kevin. All rights reserved.
//


#include <iostream>
#include "ScoreList.h"
#include <cassert>

using namespace std;

int main()
{
    ScoreList scores;
    assert(scores.size() == 0);
    for (int i = 0; i <= 2*DEFAULT_MAX_ITEMS ; i++)
    {
        scores.add(i/2);
    }
    assert(!scores.add(110));
    assert(!scores.add(-10));
    
    for (int k = 0; k < 30; k++)
    {
        scores.remove(k);
    }
    scores.add(100);
    scores.remove(100);
    cout << "ScoreList size: " << scores.size() << endl;
    cout << "ScoreList max: " << scores.maximum() << endl;
    cout << "ScoreList min: " << scores.minimum() << endl;
    
    ScoreList scores2;
    for(int i = 0; i< DEFAULT_MAX_ITEMS; i++)
        scores2.add(100);
    assert(scores2.size() == 200);
    assert(scores2.maximum() == 100);
    assert(scores2.minimum() == 100);
    
    cout << "Passed all tests" << endl;
}
