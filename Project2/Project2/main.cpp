//
//  main.cpp
//  Project2
//
//  Created by Kevin on 1/28/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include "Sequence.h"
#include <cassert>

using namespace std;

void test(){
    ItemType h;
    Sequence s1;
    assert(s1.empty() == true);
    assert(s1.insert(0, "hey") == true);
    assert(s1.size() == 1);
    assert(s1.insert(1, "hey2") == true);
    assert(s1.size() == 2);
    assert(s1.insert(0, "hey0") == true);
    assert(s1.insert(2, "mishey") == true);
    assert(s1.insert(4, "hey5") == true);
    assert(s1.insert(111, "hey6") == false);				//Test if inserting out of bounds is false
    assert(s1.insert(-1, "hey7") == false);
    assert(s1.insert("aaa") == 0);
    assert(s1.insert("bbb") == 1);
    assert(s1.insert("zzz") == 7);
    assert(s1.erase(0) == true);
    assert(s1.erase(6) == true);
    assert(s1.erase(3) == true);
    assert(s1.erase(6) == false);
    assert(s1.remove("bbb") == 0);
    assert(s1.remove("hey2") == 2);
    assert(s1.remove("zzz") == -1);
    assert(s1.remove("hey5") == 2);
    assert(s1.get(1,h) == true);
    assert(s1.get(-1, h) == false);
    assert(s1.get(111, h) == false);
    assert(h == "hey");
    assert(s1.set(0, "heyRevised") == true);
    assert(s1.set(2, "hey3") == false);
    assert(s1.find("hey") == 1);
    assert(s1.find("hey2") == -1);
    
    
    Sequence s2;
    assert(s2.empty() == true);
    assert(s2.insert("first") == 0);
    assert(s2.size() == 1);
    assert(s2.erase(0) == true);
    assert(s2.insert("dota") == 0);
    assert(s2.remove("dota") == 0);
    assert(s2.insert("test") == 0);
    
    s1.dump();
    s2.dump();
    assert(s1.size() == 2);
    assert(s2.size() == 1);
    
    s1.swap(s2);
    s1.dump();
    s2.dump();
    assert(s1.size() == 1);
    assert(s2.size() == 2);
    
    Sequence s3(s1);
    s3.dump();
    assert(s3.size() == 1);
    s3 = s2;
    s2.dump();
    s3.dump();
    assert(s2.size() == 2);
    assert(s3.size() == 2);
}

//void test2(){
//    Sequence s1;
//    Sequence s2;
//    Sequence s3;
//    Sequence s4;
//    s1.insert(0, 30);
//    s1.insert(1,21);
//    s1.insert(2, 63);
//    s1.insert(3, 42);
//    s1.insert(4, 17);
//    s1.insert(5, 63);
//    s1.insert(6, 17);
//    s1.insert(7, 29);
//    s1.insert(8, 8);
//    s1.insert(9,32);
//    
//    s2.insert(0, 8);
//    s2.insert(1, 32);
//    s2.insert(2,27);
//    
//    int sequence = subsequence(s1, s2);
//    cout << sequence << endl;
//    s1.dump();
//    s2.dump();
//    cout << endl;
//    
//    sequence = subsequence(s1, s4);
//    assert(sequence == -1);
//    sequence = subsequence(s4, s1);
//    assert(sequence == -1);
//    
//    s3.insert(0,111);
//    interleave(s1,s2,s3);
//    s1.dump();
//    s2.dump();
//    s3.dump();
//    interleave(s1,s3,s3);
//    s1.dump();
//    s3.dump();
//}

int main() {
    test();
    return 0;
}



//#include "Sequence.h"
//#include <type_traits>
//
//#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }
//
//static_assert(std::is_default_constructible<Sequence>::value,
//              "Sequence must be default-constructible.");
//static_assert(std::is_copy_constructible<Sequence>::value,
//              "Sequence must be copy-constructible.");
//
//void thisFunctionWillNeverBeCalled()
//{
//    CHECKTYPE(&Sequence::operator=,  Sequence& (Sequence::*)(const ItemType&));
//    CHECKTYPE(&Sequence::empty,      bool (Sequence::*)() const);
//    CHECKTYPE(&Sequence::size,       int  (Sequence::*)() const);
//    CHECKTYPE(&Sequence::insert,     bool (Sequence::*)(int, const ItemType&));
//    CHECKTYPE(&Sequence::insert,     int (Sequence::*)(const ItemType&));
//    CHECKTYPE(&Sequence::erase,      bool (Sequence::*)(int));
//    CHECKTYPE(&Sequence::remove,     int  (Sequence::*)(const ItemType&));
//    CHECKTYPE(&Sequence::get,	 bool (Sequence::*)(int, ItemType&) const);
//    CHECKTYPE(&Sequence::set,	 bool (Sequence::*)(int, const ItemType&));
//    CHECKTYPE(&Sequence::find,       int  (Sequence::*)(const ItemType&) const);
//    CHECKTYPE(&Sequence::swap,       void (Sequence::*)(Sequence&));
//    CHECKTYPE(subsequence, int  (*)(const Sequence&, const Sequence&));
//    CHECKTYPE(interleave,  void (*)(const Sequence&, const Sequence&, Sequence&));
//}
//
//int main()
//{}
