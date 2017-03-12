#include "StudentWorld.h"
#include "Field.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir) {
    m_ticks = 0;
}

StudentWorld::~StudentWorld(){
}

int StudentWorld:: init(){
    vector<std::string> fileNames = getFilenamesOfAntPrograms();
    m_numOfColonies = fileNames.size();//vector size is number of ant colonies
    
    for(int i = 0; i < m_numOfColonies; i++){
        m_compileArray[i] = new Compiler;
    }//initialize compilers
    
    std::string errorMessage;
    for(int i = 0; i < m_numOfColonies; i++){
        if ( ! m_compileArray[i]->compile(fileNames[i], errorMessage) )  {
            setError(fileNames[i] + " " + errorMessage);
            return GWSTATUS_LEVEL_ERROR;
        }//compile the ant programs
    }
    for(int i = 0; i < m_numOfColonies; i++){
        m_colonyNames[i] = m_compileArray[i]->getColonyName();
    }//store names to be shown later at top
    
    //LOAD FIELD
    Field f;
    string fieldFile = getFieldFilename();
    Field::LoadResult error = f.loadField(fieldFile);
    if(error != Field::LoadResult::load_success){
        //        setError(fieldFile + " " + error);  FIX THIS
        return false; // something bad happened!
    }//load the field and it's name

    //initialize datastructure
    for(int i = 0; i < VIEW_HEIGHT; i++){//y coordinate
        for(int j = 0; j < VIEW_WIDTH; j++){//x coordiante
            Point temp = Point(j,i);//allocate new point
            Node * p = new Node();//allocate new dummy node node
            p->m_value = nullptr;
            p->m_acted = false;
            p->m_next = p;
            p->m_previous = p;
            m_board[temp] = p;//add point to map
            
            Field::FieldItem item = f.getContentsOf(j, i);//add pebble
            if(item == Field::FieldItem::rock){
                Actor * temp = new Pebble(this, j, i);
                insertNode(temp, j, i, false);
            }
            if(item == Field::FieldItem::grasshopper){//add grasshopper
                Actor * temp = new BabyGrasshopper(this, j, i);
                insertNode(temp, j, i, false);
            }
            if(item == Field::FieldItem::food){//add food
                Actor * temp = new Food(this, j, i, 6000);
                insertNode(temp, j, i, false);
            }
            if(item == Field::FieldItem::water){//add pool
                Actor * temp = new WaterPool(this, j,i);
                insertNode(temp, j, i, false);
            }
            if(item == Field::FieldItem::poison){//add poison
                Actor * temp = new Poison(this, j, i);
                insertNode(temp, j, i, false);
            }
            
            //add anthill 0
            if(item == Field::FieldItem::anthill0 and m_compileArray[0] != nullptr){
                Actor * temp = new AntHill(this, j, i, 0, m_compileArray[0]);
                insertNode(temp, j, i, false);
            }
            //add anthill 1
            if(item == Field::FieldItem::anthill1 and m_compileArray[1] != nullptr){
                Actor * temp = new AntHill(this, j, i, 1, m_compileArray[1]);
                insertNode(temp, j, i, false);
            }
            //add anthill 2
            if(item == Field::FieldItem::anthill2 and m_compileArray[2] != nullptr){
                Actor * temp = new AntHill(this, j, i, 2, m_compileArray[2]);
                insertNode(temp, j, i, false);
            }
            //add anthill 3
            if(item == Field::FieldItem::anthill3 and m_compileArray[3] != nullptr){
                Actor * temp = new AntHill(this, j, i, 3, m_compileArray[3]);
                insertNode(temp, j, i, false);
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
    updateTickCount(); // update the current tick # in the simulation
    
    //Call do something if it hasn't acted yet
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            Point temp = Point(j,i);
            map<Point, Node *>::iterator item;
            item = m_board.find(temp);//access the location on the map
            
            Node * p = item->second->m_next;
            while(p != item->second){//for each node in location
                if(p->m_acted == false and p->m_value->isAlive()){
                    p->m_value->doSomething();//call do something
                    p->m_acted = true;//make sure it won't do something twice
                    
                    //if Actor object changed location, update location on map
                    if(p->m_value->GraphObject::getX() != item->first.getXPoint() or p->m_value->GraphObject::getY() != item->first.getYPoint()){
                        Actor * actorMove = p->m_value;//transfer object
                        
                        int x = p->m_value->GraphObject::getX();
                        int y = p->m_value->GraphObject::getY();
                        
                        insertNode(actorMove, x, y, true);//insert at new location
                        p = p->m_next;
                        eraseNode(actorMove, j, i);//delete node at old location
                        continue;
                    }
                }
                
                p = p->m_next;
            }
        }
    }
    //Reset all acted bools
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            Point reset = Point(j,i);
            map<Point, Node *>::iterator item;
            item = m_board.find(reset);
            Node * p = item->second->m_next;
            while(p != item->second){
                p->m_acted = false;//make sure it can act next tick
                p = p->m_next;
            }
        }
    }
//    Delete all dead objects
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            Point deleteNode = Point(j,i);
            map<Point, Node *>::iterator item;
            item = m_board.find(deleteNode);//access map
            
            Node * p = item->second->m_next;
            while(p != item->second and p->m_value != nullptr){//for each node in linked list
                if(p->m_value->isAlive() == false){//if it's dead
                    Node * tempDelete = p;
                    tempDelete->m_previous->m_next = tempDelete->m_next;
                    tempDelete->m_next->m_previous = tempDelete->m_previous;
                    p = p->m_next;//uncouple node
                    delete tempDelete->m_value;//delete Actor *
                    delete tempDelete;//then delete node
                    continue;
                }
                p = p->m_next;
            }
        }
    }
    setDisplayText();//display text
    
    
    if(m_ticks == 2000){//FIX
        if(m_currentWinner == -1){//if the ants never got above 5
            return GWSTATUS_NO_WINNER;
        }
        setWinner(m_colonyNames[m_currentWinner]);//set winner
        return GWSTATUS_PLAYER_WON;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    map<Point, Node *>::iterator item;//iterator for map
    for(item = m_board.begin(); item != m_board.end(); item++){
        Node * p = item->second->m_next;
        while(p != item->second){
            p = p->m_next;
            delete p->m_previous->m_value;//delete the Actor pointer object
            delete p->m_previous;//delete the node
        }
        delete item->second;//delete dummy node
    }
    for(int i = 0; i < m_numOfColonies; i++){
        delete m_compileArray[i];//delete compiler objects used to drive ants
    }
}

//Linked List Nodes
void StudentWorld::insertNode(Actor * actor, int j, int i, bool acted){
    Point insert = Point(j,i);
    map<Point, Node *>::iterator item;
    item = m_board.find(insert);//Access map
    Node * p = item->second->m_previous;//set pointer to last item
    
    Node * temp = new Node();//Add node
    temp->m_value = actor;
    temp->m_acted = acted;
    temp->m_next = item->second;
    temp->m_previous = p;
    p->m_next = temp;
    item->second->m_previous = temp;//Add node to linkedlist
}

void StudentWorld:: eraseNode(Actor * actor, int j, int i){
    Point erase = Point(j,i);
    map<Point, Node *>::iterator item;
    item = m_board.find(erase);
    Node * p = item->second->m_next;
    while(p != item->second and p->m_value != actor){//find node with particular actor pointer
        p = p->m_next;
    }
    p->m_previous->m_next = p->m_next;
    p->m_next->m_previous = p->m_previous;
    delete p;//delete node
}

void StudentWorld:: removeNode(int j, int i, int num){
    Point remove = Point(j,i);//remove node # num in linked list
    map<Point, Node *>::iterator item;
    item = m_board.find(remove);
    Node * p = item->second->m_next;
    int count = 0;
    while(p != item->second and count < num){
        p = p->m_next;
    }
    p->m_previous->m_next = p->m_next;
    p->m_next->m_previous = p->m_previous;
    delete p;//delete node
}

int StudentWorld::numActors(int j, int i){//returns num of actors
    Point count = Point(j,i);
    map<Point, Node *>::iterator item;
    item = m_board.find(count);//access map
    Node * p = item->second->m_next;//pointer to linkedlist
    
    int numNodes = 0;
    while(p!= item->second){
        numNodes++;
        p= p->m_next;
    }
    return numNodes;//return num of nodes at location in map
}

Actor * StudentWorld::accessNodes(int j, int i, int target){//returns pointer to node value
    Point retrieve = Point(j,i);
    map<Point, Node *>::iterator item;
    item = m_board.find(retrieve);
    Node * p = item->second->m_next;
    for(int i = 0; i < target; i++){
        p = p->m_next;
    }
    Actor * ap = p->m_value;
    return ap;//return pointer at particular node
}

void StudentWorld:: setDisplayText()
{
    ostringstream oss;
    int ticks = getCurrentTicks();//get ticks
    int numTicks = 2000 - ticks;
    oss << "Ticks:" << setw(5) << numTicks;//add ticks to oss
    if(m_numOfColonies > 0)// if there is more than 0 colonies
        oss<< " -";
    
    for(int i = 0; i < m_numOfColonies; i++){
        if(m_colonyAnts[i] > 5 and m_colonyAnts[i] > m_currentMax){
            m_currentMax = m_colonyAnts[i];
            m_currentWinner = i;
        }//find the current winner.  If there is a tie, whoever got there first is still winning
    }
    
    oss.fill('0');
    for(int i = 0; i<m_numOfColonies; i++){
        if(m_currentWinner == i){//set winner string
            oss << "  " << m_colonyNames[i] << "*" << ":" << " " << setw(2) << m_colonyAnts[i];
        } else {//set other ant strings
            oss << "  " << m_colonyNames[i] << ":" << " " << setw(2) << m_colonyAnts[i];
        }
    }
    string s = oss.str();
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}
void StudentWorld:: updateTickCount(){
    m_ticks++;
}

int StudentWorld:: getCurrentTicks(){
    return m_ticks;
}


bool StudentWorld::canMoveTo(int x, int y){
    if(x < 0 or y < 0 or x >= VIEW_WIDTH or y >= VIEW_HEIGHT)
        return false;//if out of bounds, cannot possibly move there
    
    Point temp = Point(x,y);
    map<Point, Node *>::iterator item;
    item = m_board.find(temp);//access map
    Node * p = item->second->m_next;
    while(p!=item->second){
        if(p->m_value->blocksMovement() == true)//find a pebble
            return false;
        p = p->m_next;
    }
    return true;//returns if it is possible to move at x, y
}

Actor* StudentWorld:: getEdibleAt(int x, int y) {
    Point temp = Point(x,y);
    map<Point, Node *>::iterator item;
    item = m_board.find(temp);//Access map
    
    Node * p = item->second->m_next;
    while(p!=item->second){//loop through linked list
        if(p->m_value->isEdible()){
            return p->m_value; //Returns pointer to a food object
        }
        p = p->m_next;
    }
    return nullptr;//else no food pointer
}

Actor* StudentWorld:: getPheromoneAt(int x, int y, int colony) {
    Point temp = Point(x,y);
    map<Point, Node *>::iterator item;
    item = m_board.find(temp);//accesp ma[
    
    Node * p = item->second->m_next;
    while(p!=item->second){//look through linkedlist
        if(p->m_value->isPheromone(colony)){
            return p->m_value;//returns pointer to a correct Pheromone
        }
        p = p->m_next;
    }
    return nullptr;//else no correct Pheromone
}

bool StudentWorld::isEnemyAt(int x, int y, int colony){
    Point temp = Point(x,y);
    map<Point, Node *>::iterator item;
    item = m_board.find(temp);//access map
    Node * p = item->second->m_next;
    while(p!=item->second){//loop through linkedlist
        if(p->m_value->isEnemy(colony)){//return true if there is an enemy to ant at location
            return true;
        }
        p = p->m_next;
    }
    return false;
}

bool StudentWorld::isDangerAt(int x, int y, int colony){
    Point temp = Point(x,y);
    map<Point, Node *>::iterator item;
    item = m_board.find(temp);//access map at location
    Node * p = item->second->m_next;
    while(p!=item->second){//loop through linkedlist
        if(p->m_value->isDangerous(colony)){//return true if there is something dangerous there
            return true;
        }
        p=p->m_next;
    }
    return false;//no danger
}

bool StudentWorld:: isAntHillAt(int x, int y, int colony){
    Point temp = Point(x,y);
    map<Point, Node *>::iterator item;
    item = m_board.find(temp);//access map at location
    Node * p = item->second->m_next;
    while(p!=item->second){//loop through linkedlist
        if(p->m_value->isAntHill(colony)){//returns true if there is correct anthill object
            return true;
        }
        p = p->m_next;
    }
    return false;//otherwise return false
}

void StudentWorld::increaseScore(int colony){
    m_colonyAnts[colony] += 1;//increase number of ants for a colony
}



















