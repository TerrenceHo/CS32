//
//  Sequence.cpp
//  HW1
//
//  Created by Kevin on 1/21/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include "Sequence.h"
#include <array>

Sequence::Sequence(){
    
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
    if(pos < 0 or pos > size() or size() >= DEFAULT_MAX_ITEMS)
        return false;
    for(int i = size(); i > pos; i--){
        seq[i] = seq[i-1];
    }
    seq[pos] = value;
    m_length++;
    return true;
}

int Sequence::insert(const ItemType &value){
    if(size() >= DEFAULT_MAX_ITEMS)
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
    if(pos < 0 or pos >= size())
        return false;
    for(int i = pos; i < size() - 1; i++){
        seq[i] = seq[i+1];
    }
    m_length--;
    return true;
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
    int length;
    if(size() > other.size()){
        length = size();
    } else {
        length = other.size();
    }
    for(int i = 0; i < length; i++){
        ItemType temp;
        get(i,temp);
        other.get(i,seq[i]);
        other.set(i,temp);
    }
    int tempLength = m_length;
    m_length = other.m_length;
    other.m_length = tempLength;
}

void Sequence::dump() const{
    for(int i = 0; i < size(); i++){
        std::cerr << seq[i] << ", ";
    }
    std::cerr << std::endl;
}



