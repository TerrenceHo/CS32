//
//  Sequence.cpp
//  Project2
//
//  Created by Kevin on 1/28/17.
//  Copyright © 2017 Kevin. All rights reserved.
//


#include "Sequence.h"
#include <iostream>

Sequence::Sequence() {  //Create a dummy node and set pointers to point it itself
    m_size = 0;
    head = new Node;
    head->m_next = head;
    head->m_previous = head;
}
Sequence::~Sequence(){//delete all pointers while head is not pointing to itself
    while(head->m_next != head){
        Node * p = head->m_previous;
        p->m_previous->m_next = head;
        head->m_previous = p->m_previous;
        delete p;
    }
    delete head;//delete dummy node
}

Sequence::Sequence(const Sequence & obj){//create a new dummy node, set all pointers, and copy values and create new nodes to hold those values from one Sequence to another
    m_size = 0;
    head = new Node;
    head->m_next = head;
    head->m_previous = head;
    for(int i = 0; i < obj.size(); i++){//for each node in other Sequence
        ItemType temp;
        obj.get(i, temp);//get value at that node
        insert(i, temp); //insert a new node with that value
    }
}

Sequence& Sequence::operator=(const Sequence& rhs)//create temp node and then swap to make equal
{
    if(this!=&rhs)
    {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Sequence::empty() const{
    return m_size == 0;//return if size == 0
}

int Sequence::size() const{//return size
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value){
    if(pos > size() or pos < 0)
        return false;
    Node * temp = new Node;//new node
    Node * p = head->m_next;//pointer points to first node
    for(int i = 0; i < pos; i++){//iterate until position, lining up pointers and i
        p = p->m_next;
    }
    temp->m_value = value;//set new node value
    temp->m_next = p;//next points to p
    temp->m_previous=p->m_previous;//new node points to node before p
    p->m_previous->m_next = temp;//set p previous to point to temp
    p->m_previous = temp;//set previous node to point to temp
    m_size++;//increase size
    return true;
}

int Sequence::insert(const ItemType& value) {
    Node* p = head->m_next;
    int count = 0;
    while(p!=head){//Increase it until value is less than the value at pointer p
        if(value <= p->m_value){
            break;//if value <= p->m_value, stop increasing
        }
        count++;//increment count
        p = p->m_next;//increment node
    }
    insert(count,value);//call insert
    return count;//return count, pos of where it was inserted
}

bool Sequence:: erase(int pos){
    if(pos > size() or pos < 0)
        return false;
    Node * p = head->m_next;//set pointer to first value
    for(int i = 0; i<pos;i++){
        p=p->m_next;//increment counter pos times
    }
    p->m_previous->m_next = p->m_next;//unhook node
    p->m_next->m_previous = p->m_previous;
    delete p;//delete p at very end
    m_size--;//decrease size
    return true;
}

int Sequence:: remove(const ItemType& value){
    Node * p = head->m_next;//set pointer equal to first node
    int count = 0;
    while(p!= head){
        if(p->m_value == value){//increment nodes until the correct value is found
            erase(count);//call erase
            return count;//return counter of where it was removed
        }
        p = p->m_next;//increment node
        count++;//increment counter
    }
    return -1;//if can't remove, return -1
}

bool Sequence:: get(int pos, ItemType& value) const{
    if(pos < size() and pos >=0){//if pos is valid
        Node * p = head->m_next;//set pointer to first node
        for(int i = 0; i < pos; i++){//increment nodes until correct value is found
            p = p->m_next;
        }
        value = p->m_value;//set value to value at pos
        return true;
    }
    return false;
}

bool Sequence:: set(int pos, const ItemType& value){
    if(pos < size() and pos >=0){//if pos is valid
        Node * p = head->m_next;//set pointer to first node
        for(int i = 0; i < pos; i++){//increment nodes until correct value is found
            p = p->m_next;
        }
        p->m_value = value;//set value at p to value
        return true;
    }
    return false;
}

int Sequence:: find(const ItemType& value) const{
    Node * p = head->m_next;//set pointer to first node
    int count = 0;
    while(p != head){
        if(p->m_value == value){//if a match is found, return count
            return count;
        }
        p = p->m_next;//increment node
        count++;//increment count
    }
    return -1;//if not found, return -1
}

void Sequence:: swap(Sequence& other){
    Node * tempHead = other.head;//Swap the pointers
    other.head = head;
    head = tempHead;
    
    int tempSize = other.m_size;//swap the sizes
    other.m_size = m_size;
    m_size = tempSize;
}

void Sequence::dump() const{
    Node * p = head->m_next;
    while(p != head){
        std::cerr << p->m_value << ", ";
        p = p->m_next;
    }
    std::cerr << std::endl;
}

int subsequence(const Sequence& seq1, const Sequence& seq2){
    if(seq1.size() < seq2.size() or seq2.empty()){
        return -1;
    }
    ItemType input1;
    ItemType input2;
    int startpoint;
    bool isASequence = false;
    for(int i = 0; i < seq1.size(); i++){//for everything in seq1
        seq1.get(i,input1);//get values at i and seq2(0)
        seq2.get(0,input2);
        if(input1 == input2){//if values are the same
            startpoint = i;//mark where they start
            for(int j = 0, startSequence = i; j < seq2.size(); j++, startSequence++){//increment through both lists, comparing to see if they are the same
                isASequence = true;
                seq1.get(startSequence, input1);
                seq2.get(j, input2);
                if(input1 != input2){
                    isASequence = false;
                    break;//if they are not the same, break this loop
                }
            }
            if(isASequence){//if it was a sequence, return the startpoint
                return startpoint;
            }
        }
    }
    return -1;//otherwise, return -1
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result){
    int length;
    int count = 0;
    bool getCheck;
    ItemType temp;
    Sequence tempResult(result);//prevent aliasing
    while(!tempResult.empty())
        tempResult.erase(0);//erase all in tempresult
    if(seq1.size() > seq2.size())
        length = seq1.size();
    else
        length = seq2.size();//set size to greater length
    
    for(int i = 0; i < length; i++){//for each in the sequences
        getCheck = seq1.get(i,temp);
        if(getCheck){
            tempResult.insert(count,temp);
            count++;
        }//if there is a value to add, add it to result, otherwise skip
        getCheck = seq2.get(i,temp);
        if(getCheck){
            tempResult.insert(count, temp);
            count++;
        }//if there is a value to add, add it to result, otherwise skip
    }
    result = tempResult;//set result back to tempResult
}

