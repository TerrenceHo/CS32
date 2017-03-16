#include "provided.h"
#include "support.h"
//#include <string>
//#include <vector>
#include <queue>
#include <cctype>
#include <cmath>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader m_mapload;
    AttractionMapper m_attractmap;
    SegmentMapper m_segmap;
    
    struct LocNode{
    public:
        LocNode(GeoCoord curr, double distT, double distR, GeoCoord prev, string name): current(curr), distanceTravelled(distT), distanceRemaining(distR), previous(prev), streetName(name){}
        GeoCoord getCurrent() const { return current; }
        double getTravelled() const { return distanceTravelled; }
        double getRemaining() const { return distanceRemaining; }
        GeoCoord getPrevious() const { return previous; }
        string getStreetName() const { return streetName; }
        
//        bool operator<(const LocNode &rhs) const {
//            double value1 = getTravelled() + getRemaining();
//            double value2 = rhs.getTravelled() + rhs.getRemaining();
//            return  value1 < value2;
//        }
    private:
        GeoCoord current;
        double distanceTravelled;
        double distanceRemaining;
        GeoCoord previous;
        string streetName;
    };
    
    class myComparator
    {
    public:
        bool operator() (const LocNode& n1, const LocNode& n2)
        {
            return (n1.getTravelled() + n1.getRemaining()) > (n2.getTravelled() + n2.getRemaining());
        }
    };
};


NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{//WHAT ELSE CAN FAIL?? TODO
    bool loadCheck = m_mapload.load(mapFile);//Load the mapdata
    if(!loadCheck)
        return false;//if it was wrong return false
    m_attractmap.init(m_mapload);//load in map of attractions
    m_segmap.init(m_mapload);//load in map of segments
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    directions.clear();//Directions should be cleared
    vector<LocNode> processedNodes;//Vector where Nodes we have passed through will go
    priority_queue<LocNode, vector<LocNode>, myComparator> pq; //Queue to determine which Node to process
    
    GeoCoord gStart;//Start coordinate
    GeoCoord gEnd;//End coordinate
    
    //Make all lowercase
//    for(int i = 0; i < start.size(); i++)
//        start[i] = tolower(start[i]);
//    for(int i = 0; i < end.size(); i++)
//        end[i] = tolower(end[i]);
    
    //Try to get attration geocoords, otherwise returns with negative Nav
    if(!m_attractmap.getGeoCoord(start, gStart))
        return NAV_BAD_SOURCE;
    if(!m_attractmap.getGeoCoord(end, gEnd))
        return NAV_BAD_DESTINATION;
    
    //Get street segment the attraction is located at GeoCoord
    vector<StreetSegment> streetVecStart = m_segmap.getSegments(gStart);
    GeoCoord firstGeo = streetVecStart.back().segment.start;
    LocNode geoStart(firstGeo, 0, distanceEarthMiles(firstGeo, gEnd), firstGeo, streetVecStart.back().streetName);
    //Node to be pushed in is a street segement that attraction is affiliated with
    //Push start coordinate to be processed
    pq.push(geoStart);
    
    bool foundRoute = false;
    while(!pq.empty()){
        //Check if this is a GeoCoord we have processed already.  If it is, don't process it
        bool topNotProcessed = true;
        for(int i = 0; i < processedNodes.size(); i++){
            if(pq.top().getCurrent() == processedNodes[i].getCurrent()){
                topNotProcessed = false;
                pq.pop();
                break;
            }
        }
        
        if(!topNotProcessed) continue; //If GeoCoord was already processed, don't repeat
        
        processedNodes.push_back(pq.top()); //Otherwise, push it back to ProcessedNodes
        pq.pop();//remove from prio queue
        
        if(processedNodes.back().getCurrent() == gEnd){
            foundRoute = true;
            break;
        }//If back of processed nodes is the end goal, then stop while loop
        //otherise, we have to go through and add more streetsegments
        //get associated street segments
        streetVecStart = m_segmap.getSegments(processedNodes.back().getCurrent());
        for(int i = 0; i < streetVecStart.size(); i++){
            GeoCoord newGeo;
            bool addedGeoCoord = false;
            string street_name;
            
            if(streetVecStart[i].segment.start == processedNodes.back().getCurrent() and !addedGeoCoord){
                //If it starts are current location, push to end geocoord
                street_name = streetVecStart[i].streetName;
                newGeo = streetVecStart[i].segment.end;
                addedGeoCoord = true;
            } else if(streetVecStart[i].segment.end == processedNodes.back().getCurrent() and !addedGeoCoord){
                //if it starts with end, push the start geocoord
                street_name = streetVecStart[i].streetName;
                newGeo = streetVecStart[i].segment.start;
                addedGeoCoord = true;
            }
            //if the attraction is the end geocord we're looking for push it to queue
            for(int j = 0; j < streetVecStart[i].attractions.size(); j++){
                if(streetVecStart[i].attractions[j].geocoordinates == gEnd){
                    street_name = streetVecStart[i].streetName;
                    newGeo = streetVecStart[i].attractions[j].geocoordinates;
                    addedGeoCoord = true;
                }
            }
            
            if(!addedGeoCoord) continue; //If did not add a GeoCoord, skip rest
            for(int j = 0; j < processedNodes.size(); j++){
                if(newGeo == processedNodes[j].getCurrent())
                    continue;
            }//If we already been to the new node, then don't add it.
            
            
            //Else we are adding a new Node for a location
            //get distances for heuristics
            double distanceT = distanceEarthMiles(processedNodes.back().getCurrent(), newGeo) + processedNodes.back().getTravelled();
            double distanceR = distanceEarthMiles(gEnd, newGeo);
            street_name = streetVecStart[i].streetName; //get street name
            LocNode newNode (newGeo, distanceT, distanceR, processedNodes.back().getCurrent(), street_name);
            pq.push(newNode);//create and push new node
        }
    }
    
    if(!foundRoute)
        return NAV_NO_ROUTE;  // If out of GeoCoords to process, there is no route there

    //Add only nodes that are in correct nav path to correctNodes, and pop everything else
    vector<LocNode> correctNodes;
    GeoCoord nextTarget = processedNodes.back().getCurrent();
    while(!processedNodes.empty()){
        if(nextTarget == processedNodes.back().getCurrent()){
            correctNodes.push_back(processedNodes.back());
//            correctNodes.insert(correctNodes.begin(), processedNodes.back());
            nextTarget = processedNodes.back().getPrevious();
        }//Use getPrevious() to track where Node connects to
        processedNodes.pop_back();
    }
    
    //proceesedNodes is now empty.  Use it as a container now
    //correctNodes is in reverse order, where the back contains the start Node
    
    //This is used to prep the while loop
    processedNodes.push_back(correctNodes.back());
    correctNodes.pop_back();
    
    
    while(!correctNodes.empty()){
        GeoSegment newSeg (processedNodes.back().getCurrent(), correctNodes.back().getCurrent());
        string direction = directionOfLine(newSeg);
        NavSegment newNavSeg (direction, correctNodes.back().getStreetName(), distanceEarthMiles(newSeg.start, newSeg.end), newSeg);
        
        //We are always going to have a NagSeg, but if the street names are different, push a NavTurn first
        if(!directions.empty() and newNavSeg.m_streetName != directions.back().m_streetName){
            double newAngle = angleBetween2Lines(directions.back().m_geoSegment, newNavSeg.m_geoSegment);
            if(newAngle >= 180)
                direction = "right";
            else
                direction = "left";
            NavSegment newNavTurn (direction, newNavSeg.m_streetName);
            directions.push_back(newNavTurn);
        }
        directions.push_back(newNavSeg);
        processedNodes.push_back(correctNodes.back());
        correctNodes.pop_back();
    }
    
        
    
    //    vector<StreetSegment> streetVecEnd = m_segmap.getSegments(gEnd);
    return NAV_SUCCESS; //End condition, have found the last location
}



//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}

