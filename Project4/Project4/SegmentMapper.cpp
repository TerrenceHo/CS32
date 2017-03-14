#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment>> m_map;
};

SegmentMapperImpl::SegmentMapperImpl()
{ //Do we need to construct anything?  TODO
}

SegmentMapperImpl::~SegmentMapperImpl()
{ //Do we need to destruct anything? TODO
}

void SegmentMapperImpl::init(const MapLoader& ml)
{   //initializes the tree.  For every GeoCoordinate it has a vector of StreetSegments that that all have had something to do with that GeoCoordinate.
    //if the geopoint already exists, then add to the vector a StreetSegment
    //For each streetsegment initialized, have to look in start, end, and each attraction
    
    StreetSegment seg;
    size_t numSegments = ml.getNumSegments();
    for(size_t i = 0; i < numSegments; i++){
        bool checkSegment = ml.getSegment(i, seg);
        if(checkSegment){
            vector<StreetSegment> * streetPointer = m_map.find(seg.segment.start);
            if(streetPointer != nullptr)
                streetPointer->push_back(seg);
            else {
                vector<StreetSegment> streets1;
                streets1.push_back(seg);
                m_map.associate(seg.segment.start, streets1);
            }
            streetPointer = m_map.find(seg.segment.end);
            if(streetPointer != nullptr)
                streetPointer->push_back(seg);
            else {
                vector<StreetSegment> streets2;
                streets2.push_back(seg);
                m_map.associate(seg.segment.start, streets2);
            }
            for(size_t j = 0; j < seg.attractions.size(); j++){
                streetPointer = m_map.find(seg.attractions[j].geocoordinates);
                if(streetPointer != nullptr)
                    streetPointer->push_back(seg);
                else {
                    vector<StreetSegment> streets3;
                    streets3.push_back(seg);
                    m_map.associate(seg.segment.start, streets3);
                }
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    const vector<StreetSegment> * streetVec = m_map.find(gc);
    if(streetVec != nullptr){
        return *streetVec;
    }
    vector<StreetSegment> segments;
    return segments; // This compiles, but may not be correct
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}

