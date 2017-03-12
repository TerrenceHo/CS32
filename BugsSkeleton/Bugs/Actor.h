#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    virtual ~Actor();
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    bool isAlive() const;
    
    void setDead();
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned();
    
    // Cause this actor to be be stunned.
    virtual void getStunned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone(int colony) const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony) const;
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous(int colony) const;
    
    // Is this actor the anthill of the indicated colony?
    virtual bool isAntHill(int colony) const;
    
    // Get this actor's world.
    StudentWorld* getWorld() const;
    
    void getCoordinates(Direction dir, int &x, int &y);
    
private:
    StudentWorld *m_world;
    bool m_alive;
    
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* world, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    
    virtual ~EnergyHolder();
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    int getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
    // Have this actor pick up an amount of food.
    int pickupFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
//    int ickupAndEatFood(int amt);
    
    // Does this actor become food when it dies?
    virtual bool becomesFoodUponDeath() const;
    
    // Add an amount of Pheromone to this actor's location
    void addPheromonoe(int colony);
    
private:
    int m_energy;

};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* world, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* world, int startX, int startY, int colony, Compiler* program);
    virtual void doSomething();
    virtual bool isAntHill(int colony) const;
    virtual ~AntHill();
private:
    Compiler * m_program;
    int m_colony;
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* world, int startX, int startY, int colony);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    // Increase the strength (i.e., energy) of this pheromone.
//    void increaseStrength();
private:
    int m_colony;
};

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* world, int x, int y, int imageID);
    virtual ~TriggerableActor();
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* world, int x, int y);
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* world, int x, int y);
    virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, int energy, Direction dir, int imageID);
    virtual ~Insect();
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    virtual bool isDangerous(int colony) const;
    virtual bool becomesFoodUponDeath() const;
    
    bool getAsleep() const;
    void setAsleep(bool asleep);
    int getSleepCount() const;
    void addSleepCount(int amt);
    
    void setStunnedLocation();
    void getStunnedLocation(int &x, int &y);
    void setStunStatus(bool stun);
    bool getStunStatus();
    
    // Set x,y to the coordinates of the spot one step in front of this insect.
//    void getXYInFrontOfMe(int x, int y) const;
    
    // Move this insect one step forward if possible, and return true;
    // otherwise, return false without moving.
//    virtual bool moveForwardIfPossible();
    
private:
    int m_sleepcount;
    bool m_sleeping;
    int xStunnedAt;
    int yStunnedAt;
    bool canBeStunned;
};

class Ant : public Insect
{
public:
    Ant(StudentWorld* world, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
    virtual bool isDangerous(int colony) const;
    virtual bool moveForwardIfPossible();
    virtual bool interpreter();
    virtual void bite();
    bool evaluateCondition(std::string condition);
private:
    Compiler * m_program;
    int m_colony;
    int m_ic;
    int m_randomValue;
    bool m_bitten;
    bool m_blocked;
    int m_foodHeld;
};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* world, int startX, int startY, int energy, int imageID);
    virtual ~Grasshopper();
    virtual void doSomething();
    virtual bool isEnemy(int colony) const;
    
    int getSteps() const;
    void adjustStepCount(int amt);
    void setStepCount(int amt);
private:
    int m_steps;
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void getBitten(int amt);
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual void getStunned();
};

#endif // ACTOR_H_












