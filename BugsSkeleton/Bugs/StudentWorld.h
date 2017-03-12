#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    
    void insertNode(Actor * actor, int j, int i, bool acted);
    void eraseNode(Actor * actor, int i, int j);
    void removeNode(int i, int j, int num);
    
    int numActors(int j, int i);
    Actor * accessNodes(int j, int i, int target);
    
    void setDisplayText();
    void updateTickCount();
    int getCurrentTicks();
    std::string formatText(int ticks);
    
    // Can an insect move to x,y?
    bool canMoveTo(int x, int y);
    
    // Add an actor to the world
    void addActor(Actor* a);
    
    // If an item that can be picked up to be eaten is at x,y, return a
    // pointer to it; otherwise, return a null pointer.  (Edible items are
    // only ever going be food.)
    Actor* getEdibleAt(int x, int y);
    
    // If a pheromone of the indicated colony is at x,y, return a pointer
    // to it; otherwise, return a null pointer.
    Actor* getPheromoneAt(int x, int y, int colony);
    
    // Is an enemy of an ant of the indicated colony at x,y?
    bool isEnemyAt(int x, int y, int colony);
    
    // Is something dangerous to an ant of the indicated colony at x,y?
    bool isDangerAt(int x, int y, int colony);
    
    // Is the anthill of the indicated colony at x,y?
    bool isAntHillAt(int x, int y, int colony);
    
    // Bite an enemy of an ant of the indicated colony at me's location
    // (other than me; insects don't bite themselves).  Return true if an
    // enemy was bitten.
    bool biteEnemyAt(Actor* me, int colony, int biteDamage);
    
    // Poison all poisonable actors at x,y.
    bool poisonAllPoisonableAt(int x, int y);
    
    // Stun all stunnable actors at x,y.
    bool stunAllStunnableAt(int x, int y);
    
    // Record another ant birth for the indicated colony.
    void increaseScore(int colony);
    
    

private:
    class Node{
    public:
        Actor * m_value;
        bool m_acted;
        Node * m_next;
        Node * m_previous;
    };
    
    class Point{
    public:
        Point(int j, int i):x(j), y(i){}
        bool operator<(const Point &rhs) const{
            int value1 = getXPoint() + (VIEW_HEIGHT * getYPoint());
            int value2 = rhs.getXPoint() + (VIEW_HEIGHT * rhs.getYPoint());
            return value1 < value2;
        }
        int getXPoint() const{
            return x;
        }
        int getYPoint() const{
            return y;
        }
    private:
        int x, y;
    };
    
    std::map<Point, Node *>m_board;
    int m_ticks;
    std::string m_colonyNames[4];
    int m_colonyAnts[4] = {0,0,0,0};
    int m_numOfColonies;
    int m_currentWinner = -1;
    int m_currentMax = 0;
    Compiler * m_compileArray[4] = {nullptr,nullptr,nullptr,nullptr};
};

#endif // STUDENTWORLD_H_
