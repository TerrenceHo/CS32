#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor Implementations
Actor::Actor(StudentWorld *world, int startX, int startY, Direction startDir, int imageID, int depth) : GraphObject(imageID, startX, startY, startDir, depth), m_alive(true), m_world(world){}

Actor::~Actor(){}

bool Actor::isAlive() const {
    return m_alive;  //return if actor is alive
}

bool Actor:: blocksMovement() const{
    return false; //return if it is pebble
}

void Actor:: getBitten(int amt){
    return; //return have the actor be bitten
}

void Actor:: getPoisoned(){
    return; //have the actor be poisoned
}

void Actor:: getStunned(){
    return; //have the actor be stunned
}

bool Actor:: isEdible() const{
    return false;  //return if actor is food
}

bool Actor::isPheromone(int colony) const{
    return false; //return if actor is correct Pheromone
}

bool Actor::isEnemy(int colony) const{
    return false; //Return if actor is an enemy
}

bool Actor:: isDangerous(int colony) const{
    return false; //return if actor is dangerous
}

bool Actor:: isAntHill(int colony) const {
    return false;  //return if actor was correct anthill
}

StudentWorld* Actor::getWorld() const {
    return m_world; //return pointer to world
}

void Actor:: setDead(){
    m_alive = false;  //set actor as dead
}

void Actor:: getCoordinates(Direction dir, int &x, int &y){//CHANGE
    if(dir == up){
        x = GraphObject::getX();
        y = GraphObject::getY() + 1;
    } else if(dir == right){
        x = GraphObject::getX() + 1;
        y = GraphObject::getY();
    } else if(dir == down){
        x = GraphObject::getX();
        y = GraphObject::getY() - 1;
    } else if(dir == left){
        x = GraphObject::getX() - 1;
        y = GraphObject::getY();
    } //Get the coordinates the actor is facing, ie their next move
}


/////////////////////////Pebble Implemetations

Pebble::Pebble(StudentWorld *world, int startX, int startY): Actor(world, startX, startY, right, IID_ROCK, 1){}

void Pebble::doSomething(){ return; }
bool Pebble::blocksMovement() const{    return true;    } //return true because pebbles block movement

////////////////////////////EnergyHolder Implementation
EnergyHolder::EnergyHolder(StudentWorld* world, int startX, int startY, Direction startDir, int energy, int imageID, int depth):Actor(world, startX, startY, startDir, imageID, depth), m_energy(energy){}
EnergyHolder::~EnergyHolder(){}
int EnergyHolder::getEnergy() const{    return m_energy;    } // return amount of health/energy actor has

void EnergyHolder::updateEnergy(int amt) { //add/subtract the amount of energy the actor has.
    m_energy += amt;
    if(m_energy <= 0){
        setDead();//if it's health drops to 0 or below, set it to dead.
    }
}

void EnergyHolder::addFood(int amt){
    Actor * haveFood = getWorld()->getEdibleAt(getX(), getY());
    if(haveFood == nullptr){//if there is not food at location
        Actor * tempFood = new Food(getWorld(), getX(), getY(), amt);//generate a food
        getWorld()->insertNode(tempFood, getX(), getY(), false);//insert it into the world
    } else {
        Food * tempFood = dynamic_cast<Food *>(haveFood);//add amt to food
        tempFood->updateEnergy(amt);//update the amount of food
    }
    
}
int EnergyHolder::pickupFood(int amt){// Used by ants only
    int x = getX();
    int y = getY();
    int numActors = getWorld()->numActors(x, y);
    Actor * getFood = nullptr;
    for(int i = 0; i < numActors; i++){
        getFood = getWorld()->accessNodes(getX(), getY(), i);//Find if there is food at location
        if(getFood != nullptr and getFood!=this){//if there is food
            Food * f = dynamic_cast<Food *>(getFood);//use dynamic cast to access food
            if(f!=nullptr){
                if(f->getEnergy() < amt){
                    amt = getEnergy(); //add amt to ant
                    f->updateEnergy(-f->getEnergy()); //subtract amt from food
                    return amt;
                } else {
                    f->updateEnergy(-amt);
                    return amt;
                }
            }
        }
    }

    return 0;
}
//int EnergyHolder::pickupAndEatFood(int amt){//Used by ants only
//    m_energy -= amt;
//    if(m_energy < 0){
//        setDead();
//        return amt + m_energy;//m_energy is negative
//    }
//    return amt;
//}
bool EnergyHolder::becomesFoodUponDeath() const{
    return false; //Only insects become food on death
}

void EnergyHolder::addPheromonoe(int colony){
    Actor * havePheromone = getWorld()->getPheromoneAt(getX(), getY(), colony);
    if(havePheromone == nullptr){//Find it there is a pheromone at location
        //If there is no pheromone, allocate a new one and add it to world.
        Actor * tempPheromone = new Pheromone(getWorld(), getX(), getY(), colony);
        getWorld()->insertNode(tempPheromone, getX(), getY(), false);
    } else {
        //otherwise, use dynamic cast to access it and add to its energy
        Pheromone * tempPheromone = dynamic_cast<Pheromone *>(havePheromone);
        tempPheromone->updateEnergy(256);
        if(tempPheromone->getEnergy() > 768){
            tempPheromone->updateEnergy(-(getEnergy()-768));//Pheromone can't have more than 768 energy, so cap it at 768
        }
    }
}

/////////////////////////Food Implementation

Food::Food(StudentWorld * world, int startX, int startY, int foodNum):EnergyHolder(world, startX, startY, right, foodNum, IID_FOOD, 2){}
void Food::doSomething(){  return; }
bool Food::isEdible() const{ return true;    }

////////////////////////Anthill Implementation
AntHill::AntHill(StudentWorld* world, int startX, int startY, int colony, Compiler* program):EnergyHolder(world, startX, startY, right, 8999, IID_ANT_HILL, 2), m_program(program), m_colony(colony){}
AntHill::~AntHill(){}

void AntHill::doSomething(){
    updateEnergy(-1);//subtract 1 energy
    if(getEnergy() == 0){//if it died, set it to dead
        setDead();
        return;
    }
    int numActors = getWorld()->numActors(getX(), getY());//Get num of actors
    for(int i = 0; i < numActors; i++){
        Actor * ap = getWorld()->accessNodes(getX(), getY(), i);
        if(ap->isEdible() == true){//find if there is a food there
            Food * foodToEat = dynamic_cast<Food *>(ap);//dynamic cast to access food
            int foodAmt = foodToEat->getEnergy();
            if(foodAmt < 10000){
                foodToEat->updateEnergy(-foodAmt);//update energy of food
                updateEnergy(foodAmt);//add energy to anthill
            } else {
                foodToEat->updateEnergy(10000);//cannot take more than 10000 energy
                updateEnergy(10000);//add 10000 energy
            }
            return;
        }
    }
    
    if(getEnergy() >= 2000){//allocate a new ant for it's colony if it has at least 2000 energy
        Actor * newAnt = nullptr;
        if(m_colony == 0)
            newAnt = new Ant(getWorld(), getX(), getY(), m_colony, m_program, IID_ANT_TYPE0);
        else if(m_colony == 1)
            newAnt = new Ant(getWorld(), getX(), getY(), m_colony, m_program, IID_ANT_TYPE1);
        else if(m_colony == 2)
            newAnt = new Ant(getWorld(), getX(), getY(), m_colony, m_program, IID_ANT_TYPE2);
        else if(m_colony == 3){
            newAnt = new Ant(getWorld(), getX(), getY(), m_colony, m_program, IID_ANT_TYPE3);
        }
        getWorld()->insertNode(newAnt, getX(), getY(), false);//insert ant into world
        getWorld()->increaseScore(m_colony);//increase num of ants
        updateEnergy(-1500);//lose 1500 energy
    }
}

bool AntHill::isAntHill(int colony) const{
    return m_colony == colony;//return if anthill belongs to ant
}

///////////////////////Pheromone Implementation
Pheromone::Pheromone(StudentWorld* world, int startX, int startY, int colony):EnergyHolder(world, startX, startY, right, 256, IID_PHEROMONE_TYPE0 + colony, 2), m_colony(colony){}
void Pheromone::doSomething(){//lose one energy each tick
    updateEnergy(-1);
    if(getEnergy() <= 0){//set it to dead if it has 0 or less energy
        setDead();
    }
} 
bool Pheromone::isPheromone(int colony) const{
    return m_colony == colony;//return if pheromone belongs to ant colony

}

/////////////////////////TriggerableActor Implementation
TriggerableActor::TriggerableActor(StudentWorld* world, int x, int y, int imageID):Actor(world, x, y, right, imageID, 2){}
TriggerableActor::~TriggerableActor(){}
bool TriggerableActor::isDangerous(int colony) const{
    return true;//all triggerable actors are dangerous
}

/////////////////////////Pool Implementations
WaterPool::WaterPool(StudentWorld *world, int startX, int startY):TriggerableActor(world, startX, startY, IID_WATER_POOL){}
void WaterPool::doSomething(){
    int numActors = getWorld()->numActors(getX(), getY());
    Actor * toStun = nullptr;
    for(int i = 0; i < numActors; i++){
        toStun = getWorld()->accessNodes(getX(), getY(), i);
        if(toStun != nullptr and toStun!=this){
            Insect * ins = dynamic_cast<Insect *>(toStun);
            if(ins!=nullptr){
                ins->getStunned();
            }
        }
    }
}

//////////////////////////Poison Implementations
Poison::Poison(StudentWorld *world, int startX, int startY):TriggerableActor(world, startX, startY, IID_POISON){}
void Poison::doSomething(){
    int numActors = getWorld()->numActors(getX(), getY());//find num of actors at location
    Actor * toPoison = nullptr;
    for(int i = 0; i < numActors; i++){
        toPoison = getWorld()->accessNodes(getX(), getY(), i);//access actor at node
        if(toPoison != nullptr and toPoison!=this){
            Insect * ins = dynamic_cast<Insect *>(toPoison);//use dynamic cast to access insects
            if(ins!=nullptr){//if it's an insect
                ins->getPoisoned();//poison it
            }
        }
    }
}


/////////////////////////Insect Implementations
Insect::Insect(StudentWorld * world, int startX, int startY, int energy, Direction dir, int imageID):EnergyHolder(world, startX, startY, dir, energy, imageID, 1), m_sleeping(false), m_sleepcount(0), canBeStunned(true){}
Insect::~Insect(){}
void Insect::doSomething(){
    return;
}

void Insect::getBitten(int amt){
    updateEnergy(-amt);//if bitten lose energy equal to amt
    if(getEnergy() <= 0){
        addFood(100);
        setDead();//if died to bite, allocate new food and set dead
    }
}

void Insect:: getPoisoned(){
    updateEnergy(-150);//lose 150 health to poison
    if(getEnergy() <= 0){
        addFood(100);
        setDead();//if died to poison, set dead and add food at location
    }
}

void Insect::getStunned(){
    if(!getStunStatus())
        return;//if it can't be stunned because it's still on the same pool
    addSleepCount(2);
    setAsleep(true);//set asleep for 2 turns
    setStunnedLocation();//set new location that can't be stunned until he goes back to this location
    setStunStatus(false);//make it so it can't be stunned until he moves off square
    return;
}

bool Insect::isEnemy(int colony){
    return true;
}

bool Insect:: isDangerous(int colony) const{
    return true;
}

bool Insect::becomesFoodUponDeath() const{
    return true;
}

bool Insect:: getAsleep() const {
    return m_sleeping;//return if sleping
}

void Insect::setAsleep(bool asleep){
    m_sleeping = asleep;//change sleeping bool
}

int Insect:: getSleepCount() const{
    return m_sleepcount;//return number of turns left to sleep
}

void Insect:: addSleepCount(int amt) {
    m_sleepcount+= amt;//add to sleep count
}

void Insect:: setStunnedLocation(){//set stun location so that it won't be stunned forever on a space
    xStunnedAt = getX();
    yStunnedAt = getY();
}

void Insect:: getStunnedLocation(int &x, int &y){//set x and y equal to the stun location
    x = xStunnedAt;
    y = yStunnedAt;
}

void Insect:: setStunStatus(bool stun){//set it actor can be stunned
    canBeStunned = stun;
}

bool Insect:: getStunStatus(){//return if it can be stunned
    return canBeStunned;
}

//void getXYInFrontOfMe(int x, int y){
//    return;
//}
//
//bool Insect::moveForwardIfPossible(){
//    return false;
//}

/////////////////////////Grasshopper Implementation
Grasshopper::Grasshopper(StudentWorld *world, int startX, int startY, int energy, int imageID): Insect(world, startX, startY, energy, Direction(randInt(1, 4)), imageID), m_steps(randInt(2, 10)){}
Grasshopper::~Grasshopper(){}
void Grasshopper::doSomething(){ return; }

int Grasshopper::getSteps() const{
    return m_steps;//return number of steps left
}
void Grasshopper::adjustStepCount(int amt){
    m_steps+=amt;//adjsut number of steps left to travel
}
void Grasshopper::setStepCount(int amt){
    m_steps = amt;//set the steps to travel
}

bool Grasshopper::isEnemy(int colony) const{
    return true;//grasshopper always enemy of ants
}

///////////////////////BabyGrasshopper implementation
BabyGrasshopper::BabyGrasshopper(StudentWorld *world, int startX, int startY): Grasshopper(world, startX, startY, 500, IID_BABY_GRASSHOPPER){}

void BabyGrasshopper::doSomething() {
    updateEnergy(-1);//lose energy per turn
    if(getEnergy() == 0){
        addFood(100);//if it dies, add food and set to dead
        setDead();
        return;
    }
    
    if(getAsleep()){//if it is asleep, reduce # of turns to be asleep and return
        addSleepCount(-1);
        if(getSleepCount() == 0){
            setAsleep(false);
        }
        return;
    }
    if(getEnergy() >= 1600){//if energy is greater than 1600, allocate new grasshopper and add it to world
        Actor * newAdultGrasshopper = new AdultGrasshopper(getWorld(), getX(), getY());
        getWorld()->insertNode(newAdultGrasshopper, getX(), getY(), false);
        setDead();//set baby to dead
        return;
    }
    
    bool ateMeal = false;
    Actor * getFood = getWorld()->getEdibleAt(getX(), getY());
    if(getFood != nullptr and getFood->isAlive()){
        Food * f = dynamic_cast<Food *>(getFood);//use dynamic cast to access food
        f->updateEnergy(-200);
        updateEnergy(200);
        if(f->getEnergy() < 200){
            updateEnergy(f->getEnergy());//add to energy
            f->updateEnergy(-(f->getEnergy())); //it tries to eat a meal
        }
        else{
            f->updateEnergy(-200);//decrease food energy
            updateEnergy(200);
        }

        ateMeal = true;
    }
    if(ateMeal){
        if(randInt(0, 1) == 1){//50% chance to sleep after eating
            setAsleep(true);
            addSleepCount(2);
            return;
        }
    }
    int x, y;
    getCoordinates(getDirection(), x, y);//get coordinates to move to
    if(getWorld()->canMoveTo(x, y) == true){//try to move to x,y
        moveTo(x, y);
        adjustStepCount(-1);//lower step count
    }
    else {
        setStepCount(randInt(2,10));
        GraphObject::setDirection(Direction(randInt(1, 4)));//it was blocked, so set new direction and number of steps
    }
    
    if(getSteps() == 0){
        setStepCount(randInt(2,10));//has run out of steps, so set new direction and number of steps
        GraphObject::setDirection(Direction(randInt(1, 4)));
    }
    //Check if it can be stunned;
    getStunnedLocation(x, y);//check if it moved off a pool of water
    if(getX() != x or getY()!=y){
        setStunStatus(true);
    }
    
    
    setAsleep(true);//sleep for two turns
    addSleepCount(2);
}

void BabyGrasshopper::getBitten(int amt){
    updateEnergy(-amt);
    if(getEnergy() <= 0){//if it was bitten lose energy.  If it died, then add food and set dead.
        addFood(100);
        setDead();
    }
}

///////////////////////AdultGrasshopper Implementation
AdultGrasshopper::AdultGrasshopper(StudentWorld* world, int startX, int startY):Grasshopper(world, startX, startY, 1600, IID_ADULT_GRASSHOPPER){}
void AdultGrasshopper::doSomething(){
    updateEnergy(-1);//Loses energy each turn
    if(getEnergy() == 0){//If it died, then add food and set dead.
        addFood(100);
        setDead();
        return;
    }
    
    if(getAsleep()){//if it is still asleep
        addSleepCount(-1);//decrease count of sleep left
        if(getSleepCount() == 0){
            setAsleep(false);//set it to false if it should be awake next tick
        }
        return;
    }
    
    if(randInt(1, 3) == 3){
        int numActors = getWorld()->numActors(getX(), getY());
        Actor * tempBite = nullptr;
        for(int i = 0; i < numActors; i++){
            tempBite = getWorld()->accessNodes(getX(), getY(), i);
            if(tempBite != nullptr and tempBite!=this){
                Insect * ins = dynamic_cast<Insect *>(tempBite);
                if(ins!=nullptr){
                    ins->getBitten(50);
                    setAsleep(true);
                    addSleepCount(2);
                    break;
                }
            }
        }
        //Bite random enemy
    }
    
    if(randInt(1,10) == 10){//10% chance to move to a difference spot in a 10 radius circle
        int r = randInt(0, 10);
        int theta = randInt(0, 360);
        int xR = (int) (r * cos(theta * (4 * atan(1.0)) / 180));
        int yR = (int) (r * sin(theta * (4 * atan(1.0)) / 180));
        if(getWorld()->canMoveTo(getX() + xR, getY() + yR)){
            moveTo(getX() + xR, getY() + yR);
        }
        setAsleep(true);
        addSleepCount(2);
        //Jump to random space
    }
    
    bool ateMeal = false;
    Actor * getFood = getWorld()->getEdibleAt(getX(), getY());//return a food
    if(getFood != nullptr and getFood->isAlive()){
        Food * f = dynamic_cast<Food *>(getFood);//use dynamic cast to get food
        if(f->getEnergy() < 200){//if food doesn't have 200 points of energy
            updateEnergy(f->getEnergy());
            f->updateEnergy(-(f->getEnergy()));
        }
        else{
            f->updateEnergy(-200);//decrease food energy
            updateEnergy(200);//add to energy
        }
        ateMeal = true;
    }
    if(ateMeal){//if it ate a meal, it may sleep
        if(randInt(0, 1) == 1){
            setAsleep(true);
            addSleepCount(2);
            return;
        }
    }
    int x, y;
    getCoordinates(getDirection(), x, y);//get coordinates of direction to move
    if(getWorld()->canMoveTo(x, y) == true){
        moveTo(x, y);//attempst to move and decrease move counter
        adjustStepCount(-1);
    }
    else {//if blocked set new direction and amount of steps
        setStepCount(randInt(2,10));
        GraphObject::setDirection(Direction(randInt(1, 4)));
    }
    
    if(getSteps() == 0){//if out of steps, set new direction and amount of steps
        setStepCount(randInt(2,10));
        GraphObject::setDirection(Direction(randInt(1, 4)));
    }
    setAsleep(true);//sleep for two turns
    addSleepCount(2);
}
void AdultGrasshopper::getBitten(int amt){
    updateEnergy(-amt);//lose energy to bite
    if(getEnergy() <= 0){
        addFood(100);//if dead, add food and set dead
        setDead();
    }
    if(randInt(0,1) == 1){//attempts to bite back 50% of the time
        int numActors = getWorld()->numActors(getX(), getY());
        Actor * tempBite = nullptr;
        for(int i = 0; i < numActors; i++){//find an insect
            tempBite = getWorld()->accessNodes(getX(), getY(), i);
            if(tempBite != nullptr and tempBite!=this){
                Insect * ins = dynamic_cast<Insect *>(tempBite);//use dynamic cast to access insect
                if(ins!=nullptr){
                    ins->getBitten(50);//bites other insect
                    setAsleep(true);//sleeps after biting
                    addSleepCount(2);
                    break;
                }
            }
        }
        
    }
}

void AdultGrasshopper::getPoisoned(){
    return;//can't get poisoned
}

void AdultGrasshopper::getStunned(){
    return;//can't get stunned
}


//////////////////////////Ant Implementation
Ant::Ant(StudentWorld* world, int startX, int startY, int colony, Compiler* program, int imageID):Insect(world, startX, startY, 1500, Direction(randInt(1,4)),imageID),m_colony(colony), m_program(program), m_ic(0), m_randomValue(0), m_bitten(false), m_blocked(false), m_foodHeld(0){}

void Ant::doSomething(){
    updateEnergy(-1);//lose energy to ticks
    if(getEnergy() == 0){
        addFood(100);//if dead allocate food
        setDead();//set dead
    }
    if(getAsleep()){//if asleep, decrement count of sleep and check if it can be active next tick
        addSleepCount(-1);
        if(getSleepCount() == 0){
            setAsleep(false);
        }
        return;
    }
    
    if(!interpreter()){//enter interpreter to get next instruction
        setDead();//if it returned false, set ant to dead
        return;
    }
    //Check if it can be stunned;
    int x, y;
    getStunnedLocation(x, y);
    if(getX() != x or getY()!=y){
        setStunStatus(true);
    }
}

bool Ant::interpreter(){
    Compiler::Command cmd;//get commands
    for(int i = 0; i < 10; i++){
        if ( ! m_program->getCommand(m_ic, cmd) )
            return false; // error - no such instruction!
        switch(cmd.opcode)
        {
            case Compiler::moveForward://CHECKED
                int x, y;
                getCoordinates(getDirection(), x, y);//get coordinates to move forward
                if(getWorld()->canMoveTo(x, y)){//attempt to move forward
                    moveTo(x,y);
                    m_blocked = false;
                    m_bitten = false;
                } else {
                    m_blocked = true;//otherwise, it will know it was blocked
                }
                m_ic++;//increment counter for next instruction
                return true;
            case Compiler::eatFood://CHECKED
                if(m_foodHeld < 100){//tries to eat all its food
                    updateEnergy(m_foodHeld);
                    m_foodHeld = 0;
                } else {
                    updateEnergy(100);//eats 100 units of food
                    m_foodHeld -= 100;
                }
                m_ic++;
                return true;
            case Compiler::dropFood://CHECKED
                addFood(m_foodHeld);//drops all of its food to a food object or creates one
                m_foodHeld = 0;//no food left
                m_ic++;
                return true;
            case Compiler::bite://CHECKED
                bite();//insect bite
                m_ic++;
                return true;
            case Compiler::pickupFood://CHECKED
                if(m_foodHeld < 1800){
                    if(m_foodHeld > 1400){
                        m_foodHeld+=pickupFood(1800 - m_foodHeld);
                    } else {
                        m_foodHeld+=pickupFood(400);
                    }
                }
                m_ic++;
                return true;
            case Compiler::emitPheromone://CHECKED
                addPheromonoe(m_colony);//add a Pheromone to location or add to one's strength
                m_ic++;
                return true;
            case Compiler::faceRandomDirection://CHECKED
                setDirection(Direction(randInt(1, 4)));//get random direction
                m_ic++;
                return true;
            case Compiler::rotateClockwise://CHECKED
                if(getDirection() == up)//set direction to 90* clockwise
                    setDirection(right);
                else if(getDirection() == right)
                    setDirection(down);
                else if(getDirection() == down)
                    setDirection(left);
                else if(getDirection() == left)
                    setDirection(up);
                m_ic++;
                return true;
            case Compiler::rotateCounterClockwise://CHECKED
                if(getDirection() == up)//set direction to 90* counterclockwise
                    setDirection(left);
                else if(getDirection() == right)
                    setDirection(up);
                else if(getDirection() == down)
                    setDirection(right);
                else if(getDirection() == left)
                    setDirection(down);
                m_ic++;
                return true;
            case Compiler::generateRandomNumber://CHECKED
                if(stoi(cmd.operand1) == 0){//get a random number between 0 and N-1
                    m_randomValue = 0;
                } else{
                    m_randomValue = randInt(0, stoi(cmd.operand1) - 1);
                }
                m_ic++;
                break;
            case Compiler::goto_command:
                m_ic = stoi(cmd.operand1);//set instruction counter to next goto
                break;
            case Compiler::if_command://evaluate if command, and if true do the goto
                if(evaluateCondition(cmd.operand1))
                    m_ic = stoi(cmd.operand2);
                else
                    m_ic++;
                break;
            case Compiler::invalid:
                return false;
            default:
                return false;
        }
    }
    return true;
}
void Ant::getBitten(int amt){
    updateEnergy(-amt);//take damage from bite
    if(getEnergy() <= 0){
        addFood(100);//if dead, add food and set dead
        setDead();
    }
    m_bitten = true;//remembers it was bitten
}
bool Ant::isEnemy(int colony) const{
    if(m_colony != colony)//returns true if enemy is not of same colony or is insect
        return true;
    return false;}

bool Ant:: isDangerous(int colony) const{
    if(m_colony != colony)//returns true if pool, poison, grasshopper or enemy ant
        return true;
    return false;
}
bool Ant::moveForwardIfPossible(){
    return false;
}

void Ant::bite(){
    int numActors = getWorld()->numActors(getX(), getY());
    Actor * tempBite = nullptr;
    for(int i = 0; i < numActors; i++){//find an insect to bite
        tempBite = getWorld()->accessNodes(getX(), getY(), i);
        if(tempBite != nullptr and tempBite!=this){
            Insect * ins = dynamic_cast<Insect *>(tempBite);//access insect with dynamic cast
            if(ins!=nullptr){
                ins->getBitten(15);//bite insect for 15 damage
                break;
            }
        }
    }
}

bool Ant::evaluateCondition(std::string condition){
    int x, y;
    Actor * temp;
    switch(stoi(condition))
    {
        case -1:
            m_ic = -1;
            return false;
        case 0: //danger CHECKED
            getCoordinates(getDirection(), x, y);
            if(getWorld()->isDangerAt(x, y, m_colony)){//find if there is danger at x, y
                return true;
            }
            break;
        case 1: //Pheromone  CHECKED
            getCoordinates(getDirection(), x, y);
            temp = getWorld()->getPheromoneAt(x, y, m_colony);//find if there is Pheromone at x, y
            if(temp != nullptr){
                return true;
            }
            break;
        case 2: //CHECKED
            if(m_bitten == true)//was it bitten?
                return true;
            break;
        case 3: //CHECKED
            if(m_foodHeld > 0){//is it holding food?
                return true;
            }
            break;
        case 4: //CHECKED
            if(getEnergy() <= 25)//does it have 25 or less energy?
                return true;
            break;
        case 5: //anthill CHECKED
            if(getWorld()->isAntHillAt(getX(), getY(), m_colony)){//is it it's home anthill
                return true;
            }
            break;
        case 6: //food CHECKED
            temp = getWorld()->getEdibleAt(getX(), getY());//is there a food at location
            if(temp != nullptr){
                return true;
            }
            break;
        case 7: //enemy CHECKED
            if(getWorld()->isEnemyAt(getX(), getY(), m_colony)){//is there an enemy at location
                return true;
            }
            break;
        case 8://CHECKED
            if(m_blocked == true)//was it blocked previously
                return true;
            break;
        case 9://CHECKED
            if(m_randomValue == 0)//if last random value was 0
                return true;
            break;
        default:
            m_ic = -1;
            return false;
    }
    return false;
}










