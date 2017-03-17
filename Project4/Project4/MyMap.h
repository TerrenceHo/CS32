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
    m_root = nullptr;//set to nullptr so destructor does not trigger it
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const {
    return m_MapSize; //returns size
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType & key, const ValueType& value){//Insert into a binary tree
    if(m_root == nullptr){ //if tree is empty
        Node * p = new Node(key, value); //add a new node to tree
        m_root = p;
        m_MapSize++;
        return;
    }
    
    Node * cur = m_root;//set Node pointer to iterate tree
    for(;;){//forever loop until it returns
        if(key == cur->key){//if we have found the correct key, replace the Node
            cur->value = value;
            return;
        }
        if(key < cur->key){//key is less than current key,
            if(cur->lhs != nullptr)//if it isn't a nullptr, then go down left branch
                cur = cur->lhs;
            else {//otherwise, add a node at that left branch
                cur->lhs = new Node(key, value);
                m_MapSize++;
                return;
            }
        } else if(cur->key < key){//key is greater than current key,
            if(cur->rhs != nullptr)//if it isn't nullptr, go down right branch
                cur = cur->rhs;
            else {//otherwise add new node at right branch
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
    if(cur == nullptr)//if at child node, return
        return;
    clearTree(cur->lhs);//call clearTree on left branch
    clearTree(cur->rhs);//call clearTree on right branch
    delete cur;//delete the current Node
    cur = nullptr;//set pointer to null for safety
    m_MapSize--;
}
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>:: search(const KeyType& key, Node * cur) const{
    if(cur == nullptr){
        return nullptr;//if we hit the leaf nodes and did not find it, return nullptr
    } else if(key == cur->key){//if key is equal, return the current value
        return &(cur->value);
    } else if(key < cur->key){//if key is less than current, call search on left branch
        return search(key, cur->lhs);
    } else{//if key is greater than current key, call search on right branch
        return search(key, cur->rhs);
    }
    
}


#endif // MYMAP_INCLUDED

