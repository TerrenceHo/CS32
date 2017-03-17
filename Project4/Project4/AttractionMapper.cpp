//
//  AttractionMapper.cpp
//  Project4
//
//  Created by Kevin on 3/11/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "provided.h"
#include "MyMap.h"
//#include <string>
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
    for(size_t i = 0; i < numSegments; i++){//for each street segment
        bool checkSegment = ml.getSegment(i, seg);//get street segment
        if(checkSegment){
            for(size_t j = 0; j < seg.attractions.size(); j++){//for each attraction
                string attractName = seg.attractions[j].name;
                for(int k = 0; k < attractName.size(); k++)//set name to lower case for searching
                    attractName[k] = tolower(attractName[k]);
                GeoCoord attractGeo = seg.attractions[j].geocoordinates;
                //associate attraction name and geocoord
                m_map.associate(attractName, attractGeo);//add it to the map
            }
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    for(int i = 0; i < attraction.size(); i++)
        attraction[i] = tolower(attraction[i]);//make name lower case
    const GeoCoord * geoPointer = m_map.find(attraction);//call map's find function
    if(geoPointer != nullptr){
        gc = *geoPointer;//set geocoord equal to found geocoord
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
