//
//  Sequence.hpp
//  Project2
//
//  Created by Kevin on 1/28/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>

typedef std::string ItemType;

class Sequence
{
public:
    Sequence();
    ~Sequence();
    Sequence(const Sequence & obj);
    Sequence& operator=(const Sequence& rhs);
    bool empty() const;
    int size() const;
    bool insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    void dump() const;
private:
    class Node{
        public:
            ItemType m_value;
            Node * m_next;
            Node * m_previous;
    };
    int m_size;
    Node* head;

};
int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
#endif //SEQUENCE_H
