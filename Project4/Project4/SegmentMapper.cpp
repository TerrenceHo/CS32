#include "provided.h"
#include "MyMap.h"
//#include <vector>
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
            //try to find streetsegments asscoated with start geocoord
            vector<StreetSegment> * streetPointer = m_map.find(seg.segment.start);
            if(streetPointer != nullptr)//if found, simply push to back of vector
                streetPointer->push_back(seg);
            else {//otherwise, associate start geocoord with new street segment
                vector<StreetSegment> streets1;
                streets1.push_back(seg);
                m_map.associate(seg.segment.start, streets1);
            }
            //try to find streetsegments asscoated with end geocoord
            streetPointer = m_map.find(seg.segment.end);
            if(streetPointer != nullptr)//if found, simply push to back of vector
                streetPointer->push_back(seg);
            else {///otherwise, associate end geocoord with new street segment
                vector<StreetSegment> streets2;
                streets2.push_back(seg);
                m_map.associate(seg.segment.end, streets2);
            }
            //try to find streetsegments asscoated with attraction geocoord
            for(size_t j = 0; j < seg.attractions.size(); j++){//for each attraction
                streetPointer = m_map.find(seg.attractions[j].geocoordinates);
                if(streetPointer != nullptr)//if found, simply push to back of vector
                    streetPointer->push_back(seg);
                else {//otherwise, associate attraction geocoord with new street segment
                    vector<StreetSegment> streets3;
                    streets3.push_back(seg);
                    m_map.associate(seg.attractions[j].geocoordinates, streets3);
                }
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    const vector<StreetSegment> * streetVec = m_map.find(gc);//find vector of streetsegments with geocoord
    if(streetVec != nullptr){
        return *streetVec;//if found, return vector
    }
    vector<StreetSegment> segments;//otherwise return empty vector
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

