//
//  Rat.hpp
//  Project1
//
//  Created by Kevin on 1/10/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#ifndef Rat_hpp
#define Rat_hpp

class Arena;
class Rat
{
public:
    // Constructor
    Rat(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* Rat_hpp */


