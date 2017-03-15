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
    
    string getDirection(double angle) const;
};


NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{//WHAT ELSE CAN FAIL?? TODO
    bool loadCheck = m_mapload.load(mapFile);
    if(!loadCheck)
        return false;
    m_attractmap.init(m_mapload);
    m_segmap.init(m_mapload);
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
    for(int i = 0; i < start.size(); i++)
        start[i] = tolower(start[i]);
    for(int i = 0; i < end.size(); i++)
        end[i] = tolower(end[i]);
    
    //Try to get attration geocoords, otherwise returns with negative Nav
    if(!m_attractmap.getGeoCoord(start, gStart))
        return NAV_BAD_SOURCE;
    if(!m_attractmap.getGeoCoord(end, gEnd))
        return NAV_BAD_DESTINATION;
        
    //Push start coordinate to be processed
    LocNode geoStart(gStart, 0, distanceEarthMiles(gStart, gEnd), gStart, "");
    pq.push(geoStart);
    
    bool foundRoute = false;
    while(!pq.empty()){
        bool topNotProcessed = true;
        for(int i = 0; i < processedNodes.size(); i++){
            if(pq.top().getCurrent() == processedNodes[i].getCurrent()){
                topNotProcessed = false;
                pq.pop();
                break;
            }
        }
        
        if(!topNotProcessed) continue; //If GeoCoord was already processed, don't repeat
        
        processedNodes.push_back(pq.top());
        pq.pop();
        
        if(processedNodes.back().getCurrent() == gEnd){
            foundRoute = true;
            break;
        }

        vector<StreetSegment> streetVecStart = m_segmap.getSegments(processedNodes.back().getCurrent());
        for(int i = 0; i < streetVecStart.size(); i++){
            GeoCoord newGeo;
            bool addedGeoCoord = false;
            string street_name;
            if(streetVecStart[i].segment.start == processedNodes.back().getCurrent()){
                //If it starts are current location, push to end geocoord
                street_name = streetVecStart[i].streetName;
                newGeo = streetVecStart[i].segment.end;
                addedGeoCoord = true;
                
            } else if(streetVecStart[i].segment.end == processedNodes.back().getCurrent()){
                //if it starts with end, push the start geocoord
                street_name = streetVecStart[i].streetName;
                newGeo = streetVecStart[i].segment.start;
                addedGeoCoord = true;
            } else {
                //if the attraction is the geocord we're looking for push it to queue
                for(int j = 0; j < streetVecStart[i].attractions.size(); j++){
                    if(streetVecStart[i].attractions[j].geocoordinates == gEnd){
                        street_name = streetVecStart[i].streetName;
                        newGeo = streetVecStart[i].attractions[j].geocoordinates;
                        addedGeoCoord = true;
                    }
                }
            }
            
            if(!addedGeoCoord) continue; //If did not add a GeoCoord, skip rest
            for(int j = 0; j < processedNodes.size(); j++){
                if(newGeo == processedNodes[i].getCurrent())
                    continue;
            }//If we already processed the new node, then don't add it.
            
            
            double distanceT = distanceEarthMiles(processedNodes.back().getCurrent(), newGeo);
            double distanceR = distanceEarthMiles(gEnd, newGeo);
            LocNode newNode (newGeo, distanceT, distanceR, processedNodes.back().getCurrent(), street_name);
            pq.push(newNode);
        }
    }
    
    if(!foundRoute)
        return NAV_NO_ROUTE;  // If out of GeoCoords to process, there is no route there

    //Add only nodes that in path to correctNodes, and pop everything else
    vector<LocNode> correctNodes;
    GeoCoord nextTarget = processedNodes.back().getCurrent();
    while(!processedNodes.empty()){
        if(nextTarget == processedNodes.back().getCurrent()){
            correctNodes.push_back(processedNodes.back());
//            correctNodes.insert(correctNodes.begin(), processedNodes.back());
            nextTarget = processedNodes.back().getPrevious();
        }
        processedNodes.pop_back();
    }
    //proceesedNodes is now empty.  Use it as a container now
    //correctNodes is in reverse order, where the back contains the start Node
    
    //This is used to prep the loop
    processedNodes.push_back(correctNodes.back());
    correctNodes.pop_back();
    GeoSegment newSeg (processedNodes.back().getCurrent(), correctNodes.back().getCurrent());
    string direction = getDirection(angleOfLine(newSeg));
    NavSegment newNav (direction, correctNodes.back().getStreetName(), distanceEarthKM(newSeg.start, newSeg.end), newSeg);
    directions.push_back(newNav);
    
    while(!correctNodes.empty()){
        newSeg = GeoSegment (processedNodes.back().getCurrent(), correctNodes.back().getCurrent());
        direction = getDirection(angleOfLine(newSeg));
        newNav = NavSegment (direction, correctNodes.back().getStreetName(), distanceEarthKM(newSeg.start, newSeg.end), newSeg);
        if(newNav.m_direction != directions.back().m_direction){
            double newAngle = angleBetween2Lines(directions.back().m_geoSegment, newNav.m_geoSegment);
            if(newAngle >= 180)
                direction = "right";
            else
                direction = "left";
            NavSegment newTurn (direction, correctNodes.back().getStreetName());
            directions.push_back(newTurn);
        }
        directions.push_back(newNav);
    }
    
        
    
    //    vector<StreetSegment> streetVecEnd = m_segmap.getSegments(gEnd);
    return NAV_SUCCESS; //End condition, have found the last location
}

string NavigatorImpl:: getDirection(double angle) const{
    if(angle >= 0 or angle <= 22.5)
        return "east";
    else if(angle > 22.5 or angle <= 67.5)
        return "northest";
    else if(angle > 67.5 or angle <= 112.5)
        return "north";
    else if(angle > 112.5 or angle <= 157.5)
        return "northwest";
    else if(angle > 157.5 or angle <= 202.5)
        return "west";
    else if(angle > 202.5 or angle <= 247.5)
        return "southwest";
    else if(angle > 247.5 or angle <= 292.5)
        return "south";
    else if(angle > 292.5 or angle <= 337.5)
        return "southeast";
    else //(angle > 337.5 or angle < 360)
        return "east";
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

