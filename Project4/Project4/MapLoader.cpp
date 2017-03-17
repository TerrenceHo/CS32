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
    }//if error on infile for map, return false
    
    string s;
    while(getline(infile,s)){
        StreetSegment seg;//create streetsegment
        seg.streetName = s;//add streetsegment street name
        
        getline(infile, s);//get next line
        string geo1 = s.substr(0, s.find(","));//first geocoord is before the comma
        if(s[s.find(",") + 1] == ' ')//account for space after comma
            s = s.substr(s.find(",") + 2, s.size() -1);//change s remove first geo from s
        else
            s = s.substr(s.find(",") + 1, s.size() -1);
        
        string geo2 = s.substr(0, s.find(" "));//second geo ends at space
        GeoCoord loc1 (geo1, geo2);//add into loc 1
        
        s = s.substr(s.find(" ") + 1, s.size()-1);//remove second geo from s
        string geo3 = s.substr(0, s.find(","));//third geo is before comma
        if(s[s.find(",") + 1] == ' ')//account for space after comma
            s = s.substr(s.find(",") + 2, s.size() -1);//remove third geo from s
        else
            s = s.substr(s.find(",") + 1, s.size() -1);
        string geo4 = s;//geo 4 is equal to remainder of s
        GeoCoord loc2 (geo3, geo4);//add to loc2
        
        GeoSegment gseg (loc1, loc2);//combine into geosegment
        seg.segment = gseg;
        
        
        int numAttract = 0;
        infile >> numAttract;//take in number of attractions
        infile.ignore(10000, '\n');
        for(int i = 0; i < numAttract; i++){
            getline(infile,s);//get next line(is an attractions)
            string attractName = s.substr(0, s.find("|"));//attract name is before |
            
            s = s.substr(s.find("|") + 1, s.size() -1);//remove name from s
            string geo5 = s.substr(0, s.find(","));//geo 5 is before comma
            if(s[s.find(",") + 1] == ' ')//account for space after comma
                s = s.substr(s.find(",") + 2, s.size() -1);//remove geo 5 from s
            else
                s = s.substr(s.find(",") + 1, s.size() -1);
            string geo6 = s;//geo 6 is equal to rest of s
            
            Attraction attract;
            attract.name = attractName;//set name of attraction
            
            GeoCoord attractGeo (geo5, geo6);//set geocoord of attraction
            attract.geocoordinates = attractGeo;
            seg.attractions.push_back(attract);//push it back to Streetsegments
        }
        
        m_streets.push_back(seg);//push back to the streetsegment vector
    }


    return true;//went through eveything successfully
}

size_t MapLoaderImpl::getNumSegments() const //Is this interpretation correct? TODO
{
    return m_streets.size();//return size
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{//Check if this is all you have to do TODO
    if(segNum < 0 or segNum >= getNumSegments())//if out of bounds, return false
        return false;
    
    seg = m_streets[segNum]; //set seg equal to the correct street segment
    return true;  //return true to recognize that seg has changed
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
