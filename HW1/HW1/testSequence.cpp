//
//  main.cpp
//  HW1
//
//  Created by Kevin on 1/21/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;


//int main()
//{
//    Sequence s;
//    assert(s.empty());
//    assert(s.find("laobing") == -1);
//    s.insert("laobing");
//    assert(!s.empty());
//    assert(s.size() == 1  &&  s.find("laobing") == 0);
//    assert(s.insert(0, "two") == true);
//    assert(s.size() == 2);
//    assert(s.find("two")== 0);
//    s.insert(2, "three");
//    assert(s.size()==3);
//    assert(s.find("three") == 2);
//    s.insert("aaa");
//    assert(s.size() == 4);
//    assert(s.find("aaa") == 0);
//    s.insert("bbb");
//    assert(s.size() == 5);
//    assert(s.find("bbb") == 1);
//    assert(s.erase(5) == false);
//    s.erase(1);
//    assert(s.size() == 4);
//    assert(s.find("bbb") == -1);
//    assert(s.erase(-1) == false);
//    s.insert(4, "two");
//    assert(s.remove("two") == 2);
//    assert(s.size() == 3);
//    assert(s.find("two") == -1);
//    s.insert("aaa");
//    assert(s.remove("aaa") == 2);
//    assert(s.size() == 2);
//    assert(s.find("aaa") == -1);
//    assert(s.remove("ccc") == 0);
//    string temp;
//    assert(s.get(1, temp) == true);
//    assert(temp == "three");
//    assert(s.get(2, temp) == false);
//    assert(s.set(1, "one") == true);
//    assert(s.find("one") == 1);
//    assert(s.set(2, "five") == false);
//    
//    Sequence s2;
//    s2.insert(0, "zero");
//    s2.insert(1, "one");
//    s2.insert(2, "two");
//    s2.insert(3, "three");
//    s.dump();
//    s2.dump();
//    s.swap(s2);
//    assert(s.size() == 4);
//    assert(s2.size() == 2);
//
//    s.dump();
//    s2.dump();
//    cout << "Passed all tests" << endl;
//}

int main(){
    Sequence s;
    assert(s.empty());
    assert(s.find(0) == -1);
    s.insert(0);
    assert(!s.empty());
    assert(s.size() == 1  &&  s.find(0) == 0);
    assert(s.insert(0, 1) == true);
    assert(s.size() == 2);
    assert(s.find(1)== 0);
    s.insert(2, 2);
    assert(s.size()==3);
    assert(s.find(2) == 2);
    s.insert(3);
    assert(s.size() == 4);
    assert(s.find(3) == 3);
    s.insert(4);
    assert(s.size() == 5);
    assert(s.find(4) == 4);
    assert(s.erase(5) == false);
    s.erase(1);
    assert(s.size() == 4);
    assert(s.find(0) == -1);
    assert(s.erase(-1) == false);
    s.insert(4, 1);
    assert(s.remove(1) == 2);
    assert(s.size() == 3);
    assert(s.find(1) == -1);
    s.insert(4);
    assert(s.remove(4) == 2);
    assert(s.size() == 2);
    assert(s.find(4) == -1);
    assert(s.remove(6) == 0);
    unsigned long temp;
    assert(s.get(1, temp) == true);
    assert(temp == 3);
    assert(s.get(2, temp) == false);
    assert(s.set(1, 7) == true);
    assert(s.find(7) == 1);
    assert(s.set(2, 8) == false);

    Sequence s2;
    s2.insert(0, 0);
    s2.insert(1, 1);
    s2.insert(2, 2);
    s2.insert(3, 3);
    s.dump();
    s2.dump();
    s.swap(s2);
    assert(s.size() == 4);
    assert(s2.size() == 2);

    s.dump();
    s2.dump();
    
    
    cout << "Passed all tests" << endl;
}
