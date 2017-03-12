//
//  newSequence.cpp
//  HW1
//
//  Created by Kevin on 1/22/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "newSequence.h"
#include <iostream>


Sequence::Sequence(){
    seq = new ItemType[DEFAULT_MAX_ITEMS];
    m_cap = DEFAULT_MAX_ITEMS;
}

Sequence::Sequence(int arraySize){
    seq = new ItemType[arraySize];
    m_cap = arraySize;
}

Sequence::Sequence(const Sequence & other){
    m_length = other.size();
    m_cap = other.m_cap;
    seq = new ItemType[m_cap];
    for(int i = 0; i < m_length; i++){
        seq[i] = other.seq[i];
    }
}

Sequence::~Sequence(){
    delete [] seq;
}

Sequence& Sequence::operator=(const Sequence& rhs){
    if(this != &rhs){
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Sequence::empty() const{
    if (m_length == 0)
        return true;
    return false;
}

int Sequence::size() const{
    return m_length;
}

bool Sequence::insert(int pos, const ItemType &value){
    if(pos < 0 or pos > size() or size() >= m_cap)
        return false;
    for(int i = size(); i > pos; i--){
        seq[i] = seq[i-1];
    }
    seq[pos] = value;
    m_length++;
    return true;
}

int Sequence::insert(const ItemType &value){
    if(size() >= m_cap)
        return -1;
    int p;
    for(p = 0; p < size(); p++){
        if(value <= seq[p]){
            for(int i = size(); i > p; i--){
                seq[i]=seq[i-1];
            }
            break;
        }
    }
    seq[p] = value;
    m_length++;
    return p;
}

bool Sequence::erase(int pos){
    if(pos >= 0 and pos < size()){
        for(int i = pos; i < size() - 1; i++){
            seq[i] = seq[i+1];
        }
        m_length--;
        return true;
    }
    return false;
}

int Sequence::remove(const ItemType &value){
    int count = 0;
    for(int i = 0; i<size(); i++){
        if(seq[i] == value){
            erase(i);
            count++;
            i--;
        }
    }
    return count;
}

bool Sequence::get(int pos, ItemType &value) const{
    if(pos >= 0 and pos < size()){
        value = seq[pos];
        return true;
    }
    return false;
}

bool Sequence::set(int pos, const ItemType &value){
    if(pos >= 0 and pos < size()){
        seq[pos] = value;
        return true;
    }
    return false;
}

int Sequence::find(const ItemType &value) const{
    for(int p = 0; p < size(); p++)
        if(seq[p] == value)
            return p;
    return -1;
}

void Sequence::swap(Sequence &other){
    ItemType* temp_seq;
    
    //Declare all the temp variables
    temp_seq = seq;
    int tempLength = m_length;
    int tempCap = m_cap;
    
    seq = other.seq;
    other.seq = temp_seq;
    
    m_cap = other.m_cap;
    other.m_cap = tempCap;
    
    m_length = other.m_length;
    other.m_length = tempLength;
    
}

void Sequence::dump() const{
    for(int i = 0; i < size(); i++){
        std::cerr << seq[i] << ", ";
    }
    std::cerr << std::endl;
}



