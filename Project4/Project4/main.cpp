// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void testMap(){ //TESTING INITIAL MAP
    MyMap<string, int> GPAHolder;
    assert(GPAHolder.size() == 0);
    GPAHolder.associate("Terrence", 4);
    assert(GPAHolder.size() == 1);
    int v = *GPAHolder.find("Terrence");
    assert(v == 4);
    
    GPAHolder.associate("Terrence", 5);
    v = *GPAHolder.find("Terrence");
    assert(v == 5);
    assert(GPAHolder.size() == 1);
    
    GPAHolder.associate("Jacky", 6);
    GPAHolder.associate("Brandon", 7);
    GPAHolder.associate("Chloe", 8);
    GPAHolder.associate("Kristi", 9);
    
    v = *GPAHolder.find("Chloe");
    assert(GPAHolder.size() == 5);
    assert(v == 8);
    int * v2 = GPAHolder.find("Shirley");
//    v = *GPAHolder.find("Shirley");
    assert (v2 == nullptr);
    
    GPAHolder.clear();
    assert(GPAHolder.size() == 0);
    
    cout << "All tests passed" << endl;
}

void testSegmentMapper(){
    MapLoader ml;
    ml.load("/Users/kho/Desktop/CS32/Project4/Project4/mapdata.txt");
    SegmentMapper sm;
    sm.init(ml);
    
    GeoCoord lookMeUp("34.0572000", "-118.4417620");
    std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
    if (vecOfAssociatedSegs.empty())
    {
        cout << "Error - no segments found matching this coordinate\n";
        return;
    }
    cout << "Here are all the segments associated with your coordinate:" << endl;
    for (auto s: vecOfAssociatedSegs)
    {
        cout << "Segment’s street: " << s.streetName << endl;
        cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " <<
        s.segment.start.longitude << endl;
        cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " <<
        s.segment.end.longitude << endl;
        cout << "This segment has " << s.attractions.size() <<
        " attractions on it." << endl;
    }
    
    cout << "All tests passed" << endl;
}

void testAttractionMapper(){
    MapLoader ml;
//    ml.load("/Users/kho/Desktop/CS32/Project4/Project4/mapdata.txt");
    ml.load("mapdata.txt");
    AttractionMapper am;
    am.init(ml); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    GeoCoord fillMe;
    string attraction = "The Coffee Bean & Tea Leaf";
    bool found = am.getGeoCoord(attraction, fillMe);
    if ( ! found)
    {
        cout << "No geolocation found for " << attraction << endl;
        return;
    }
    cout << "The location of " << attraction << " is " << fillMe.latitude << ", " << fillMe.longitude << endl;
}

int main()
{
//    testMap();
//    testSegmentMapper();
    testAttractionMapper();
}
