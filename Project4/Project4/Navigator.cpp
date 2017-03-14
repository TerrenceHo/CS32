#include "provided.h"
//#include <string>
//#include <vector>
#include <cctype>
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
        GeoCoord current;
        double distance;
        GeoCoord previous;
    };
    vector<LocNode> processedNode;
    //GET PRIORITY QUEUE, must be less than.  Create Operator overload for LocNode and comparison function for Priority_queue
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
    directions.clear();
    GeoCoord g1;
    GeoCoord g2;
    for(int i = 0; i < start.size(); i++)
        tolower(start[i]);
    for(int i = 0; i < end.size(); i++)
        tolower(end[i]);
    if(!m_attractmap.getGeoCoord(start, g1) or !m_attractmap.getGeoCoord(end, g2))
        return NAV_BAD_SOURCE;
    
    vector<StreetSegment> streetVecStart = m_segmap.getSegments(g1);
//    vector<StreetSegment> streetVec2 = m_segmap.getSegments(g2);
    for(int i = 0; i < streetVecStart.size(); i++){
        
    }
    
    
    return NAV_NO_ROUTE;  // This compiles, but may not be correct
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

