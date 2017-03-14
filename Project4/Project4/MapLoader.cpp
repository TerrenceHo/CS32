//
//  AttractionMapper.hpp
//  Project4
//
//  Created by Kevin on 3/11/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "provided.h"
#include <iostream>
#include <fstream>
#include <cctype>
//#include <vector>
//#include <string>
using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> m_streets;
};

MapLoaderImpl::MapLoaderImpl()
{
    //Not sure what we have to create lol TODO
}

MapLoaderImpl::~MapLoaderImpl()
{
    //Do we have to delete anything??? TODO
}

bool MapLoaderImpl::load(string mapFile) //CHECK THIS OUT LATER
{//Seems a bit too complicated, I think I got it right
    ifstream infile(mapFile);
    if(!infile){
        return false;
    }

    string s;
    while(getline(infile, s)){
        StreetSegment seg;//create streetsegment
        seg.streetName = s;//add streetsegment street name
        
        string g1, g2 = "";
        infile >> g1;//take in first two coordinates
        infile >> g2;
        g1.erase(g1.end() - 1);
        GeoCoord loc1 (g1, g2);//add into loc 1
        
        infile >> g1;//Use substr to separate out coordiantes
        g2 = g1.substr(g1.find(",") + 1,g1.size());
        g1 = g1.substr(0, g1.find(","));
        GeoCoord loc2 (g1, g2);//add into loc 2
        
        GeoSegment gseg (loc1, loc2);//combine into geosegment
        seg.segment = gseg;
        
        int k;
        infile >> k; //get attraction
        infile.ignore(10000, '\n');
        for(int i = 0; i < k; i++){//for each attraction there
            Attraction attract;
            getline(infile, s);
            attract.name = s.substr(0, s.find("|"));
            string location = s.substr(s.find("|") + 1, s.size());
            GeoCoord attractGeo (location.substr(0, location.find(",")),
                                 location.substr(location.find(",") + 2, location.size()));
            attract.geocoordinates = attractGeo;
            
            seg.attractions.push_back(attract);
        }
        
        m_streets.push_back(seg);
        
    }

    return true;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const //Is this interpretation correct? TODO
{
    return m_streets.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{//Check if this is all you have to do TODO
    if(segNum < 0 or segNum >= getNumSegments())
        return false;
    
    seg = m_streets[segNum]; //Is this correct?
    
//    Do we need this??
//    seg.streetName = m_streets[segNum].streetName;
//    seg.segment.start = m_streets[segNum].segment.start;
//    seg.segment.end = m_streets[segNum].segment.end;
//    for(int i = 0; i < seg.attractions.size(); i++){
//        
//    }
    return true;  // This compiles, but may not be correct
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
    m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
    delete m_impl;
}

bool MapLoader::load(string mapFile)
{
    return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
    return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
    return m_impl->getSegment(segNum, seg);
}
