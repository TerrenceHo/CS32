// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED
#include "support.h"
template<typename KeyType, typename ValueType>

class MyMap
{
public:
    MyMap();
    ~MyMap();
    void clear();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    struct Node{
    public:
        KeyType key;
        ValueType value;
        Node * lhs;
        Node * rhs;
        Node(const KeyType& k, const ValueType& v):key(k), value(v), lhs(nullptr), rhs(nullptr){}
    };
    Node * m_root;
    int m_MapSize;
    
    void clearTree(Node * cur);
    const ValueType* search(const KeyType& key, Node * cur) const;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap():m_root(nullptr), m_MapSize(0){} //Constructor, makes root null, size = 0

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap(){
    //Destructor Can call clear tree
    clearTree(m_root);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear(){//calls clearTree to recursively delete entire tree
    clearTree(m_root);
    m_root = nullptr;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {
    return m_MapSize;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType & key, const ValueType& value){//Insert into a binary tree
    if(m_root == nullptr){ //if tree is empty
        Node * p = new Node(key, value); //add a new node to tree
        m_root = p;
        m_MapSize++;
        return;
    }
    
    Node * cur = m_root;
    for(;;){
        if(key == cur->key){
            cur->value = value;
            return;
        }
        if(key < cur->key){
            if(cur->lhs != nullptr)
                cur = cur->lhs;
            else {
                cur->lhs = new Node(key, value);
                m_MapSize++;
                return;
            }
        } else if(cur->key < key){
            if(cur->rhs != nullptr)
                cur = cur->rhs;
            else {
                cur->rhs = new Node(key, value);
                m_MapSize++;
                return;
            }
        }
    }
}

template<typename KeyType, typename ValueType>
const ValueType * MyMap<KeyType, ValueType>::find(const KeyType& key) const{
    return search(key, m_root); //Calls a recursive search function
}//TODO MUST MAKE CASE INSENSITIVE


/////////////////////////////Private functions
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clearTree(Node * cur){//recursively clears the tree
    if(cur == nullptr)
        return;
    clearTree(cur->lhs);
    clearTree(cur->rhs);
    delete cur;
    cur = nullptr;
    m_MapSize--;
}
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>:: search(const KeyType& key, Node * cur) const{
    if(cur == nullptr){
        return nullptr;
    } else if(key == cur->key){
        return &(cur->value);
    } else if(key < cur->key){
        return search(key, cur->lhs);
    } else{
        return search(key, cur->rhs);
    }
    
}


#endif // FUCK YOU SMALLBERG

