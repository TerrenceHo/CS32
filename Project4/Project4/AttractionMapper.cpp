//
//  AttractionMapper.cpp
//  Project4
//
//  Created by Kevin on 3/11/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> m_map;
};

AttractionMapperImpl::AttractionMapperImpl()
{} //I don't think I have to do anything?

AttractionMapperImpl::~AttractionMapperImpl()
{} //Not sure what I have to destroy

void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment seg;
    size_t numSegments = ml.getNumSegments();
    for(size_t i = 0; i < numSegments; i++){
        bool checkSegment = ml.getSegment(i, seg);
        if(checkSegment){
            for(size_t j = 0; j < seg.attractions.size(); j++){
                string attractName = seg.attractions[j].name;
                GeoCoord attractGeo = seg.attractions[j].geocoordinates;
                m_map.associate(attractName, attractGeo);
            }
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    const GeoCoord * geoPointer = m_map.find(attraction);
    if(geoPointer != nullptr){
        gc = *geoPointer;
        return true;
    }
    return false;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
    m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
    delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
    return m_impl->getGeoCoord(attraction, gc);
}
