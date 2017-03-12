//
//  Game.hpp
//  Project1
//
//  Created by Kevin on 1/14/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <string>
class Arena;
class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    std::string takePlayerTurn();
};

bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);
bool decodeDirection(char ch, int& dir);

#endif /* Game_hpp */
