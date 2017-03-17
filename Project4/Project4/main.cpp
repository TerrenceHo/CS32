// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
using namespace std;

void testMap();
void testMapLoader();
void testSegmentMapper();
void testAttractionMapper();
void testNavigator();

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
void printDirections(string start, string end, vector<NavSegment>& navSegments);

//int main()
//{
////    testMap();
////    testMapLoader();
////    testSegmentMapper();
////    testAttractionMapper();
//    testNavigator();
//}

int main(int argc, char *argv[])
{
    bool raw = false;
    if (argc == 5  &&  strcmp(argv[4], "-raw") == 0)
    {
        raw = true;
        argc--;
    }
    if (argc != 4)
    {
        cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
        << "or" << endl
        << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
        return 1;
    }
    
    Navigator nav;
    
    if ( ! nav.loadMapData(argv[1]))
    {
        cout << "Map data file was not found or has bad format: " << argv[1] << endl;
        return 1;
    }
    
    if ( ! raw)
        cout << "Routing..." << flush;
    
    string start = argv[2];
    string end = argv[3];
    vector<NavSegment> navSegments;
    
    NavResult result = nav.navigate(start, end, navSegments);
    if ( ! raw)
        cout << endl;
    
    switch (result)
    {
        case NAV_NO_ROUTE:
            cout << "No route found between " << start << " and " << end << endl;
            break;
        case NAV_BAD_SOURCE:
            cout << "Start attraction not found: " << start << endl;
            break;
        case NAV_BAD_DESTINATION:
            cout << "End attraction not found: " << end << endl;
            break;
        case NAV_SUCCESS:
            if (raw)
                printDirectionsRaw(start, end, navSegments);
            else
                printDirections(start, end, navSegments);
            break;
    }
}


///////////////////////Testing Functions for Navigator
void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
{
    cout << "Start: " << start << endl;
    cout << "End:   " << end << endl;
    cout.setf(ios::fixed);
    cout.precision(4);
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                cout << ns.m_geoSegment.start.latitudeText << ","
                << ns.m_geoSegment.start.longitudeText << " "
                << ns.m_geoSegment.end.latitudeText << ","
                << ns.m_geoSegment.end.longitudeText << " "
                << ns.m_direction << " "
                << ns.m_distance << " "
                << ns.m_streetName << endl;
                break;
            case NavSegment::TURN:
                cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
                break;
        }
    }
}

void printDirections(string start, string end, vector<NavSegment>& navSegments)
{
    cout.setf(ios::fixed);
    cout.precision(2);
    
    cout << "You are starting at: " << start << endl;
    
    double totalDistance = 0;
    string thisStreet;
    GeoSegment effectiveSegment;
    double distSinceLastTurn = 0;
    
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                if (thisStreet.empty())
                {
                    thisStreet = ns.m_streetName;
                    effectiveSegment.start = ns.m_geoSegment.start;
                }
                effectiveSegment.end = ns.m_geoSegment.end;
                distSinceLastTurn += ns.m_distance;
                totalDistance += ns.m_distance;
                break;
            case NavSegment::TURN:
                if (distSinceLastTurn > 0)
                {
                    cout << "Proceed " << distSinceLastTurn << " miles "
                    << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
                    thisStreet.clear();
                    distSinceLastTurn = 0;
                }
                cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
                break;
        }
    }
    
    if (distSinceLastTurn > 0)
        cout << "Proceed " << distSinceLastTurn << " miles "
        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
    cout << "You have reached your destination: " << end << endl;
    cout.precision(1);
    cout << "Total travel distance: " << totalDistance << " miles" << endl;
}






/////////////Testing Functions For Map, SegmentMapper, AttractionMapper

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

void testMapLoader(){
    MapLoader ml;
    assert(ml.load("/Users/kho/Desktop/CS32/Project4/Project4/testmap.txt"));
    size_t numSegments = ml.getNumSegments();
    assert(numSegments == 7);
    bool foundAttraction = false;
    for (size_t i = 0; i < numSegments; i++)
    {
        StreetSegment seg;
        assert(ml.getSegment(i, seg));
        if (seg.streetName == "Picadilly")
        {
            assert(seg.attractions.size() == 1);
            assert(seg.attractions[0].name == "Eros Statue");
            foundAttraction = true;
            break;
        }
    }
    assert(foundAttraction);
    
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

void testNavigator() {
    Navigator nav;
    assert(nav.loadMapData("/Users/kho/Desktop/CS32/Project4/Project4/testmap.txt"));
    vector<NavSegment> directions;
    assert(nav.navigate("Eros Statue", "Hamleys Toy Store", directions) == NAV_SUCCESS);
    assert(directions.size() == 6);
    struct ExpectedItem
    {
        NavSegment::NavCommand command;
        string direction;
        double distance;
        string streetName;
    };
    const ExpectedItem expected[6] = {
        { NavSegment::PROCEED, "northwest", 0.0138, "Picadilly" },
        { NavSegment::TURN, "left", 0, "" },
        { NavSegment::PROCEED, "west", 0.0119, "Regent Street" },
        { NavSegment::PROCEED, "west", 0.0845, "Regent Street" },
        { NavSegment::PROCEED, "west", 0.0696, "Regent Street" },
        { NavSegment::PROCEED, "northwest", 0.1871, "Regent Street" },
    };
    for (size_t i = 0; i < 6; i++)
    {
        const NavSegment& ns = directions[i];
        const ExpectedItem& exp = expected[i];
        assert(ns.m_command == exp.command);
        assert(ns.m_direction == exp.direction);
        if (ns.m_command == NavSegment::PROCEED)
        {
            assert(abs(ns.m_distance - exp.distance) < 0.00051);
            assert(ns.m_streetName == exp.streetName);
        }
    }
    
    cout << "All tests passed." << endl;
}
