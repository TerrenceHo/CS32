//
//  support.cpp
//  Project4
//
//  Created by Kevin on 3/11/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "support.h"
#include "provided.h"
#include <string>
using namespace std;

//bool operator for < for GeoCoords
bool operator<(const GeoCoord &lhs, const GeoCoord &rhs) {
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude < rhs.longitude;
    return lhs.latitude < rhs.latitude;
}

//bool operator for > for GeoCoords
bool operator>(const GeoCoord &lhs, const GeoCoord &rhs){
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude > rhs.longitude;
    return lhs.latitude > rhs.latitude;

}

//bool operator for == for GeoCoords
bool operator ==(const GeoCoord &lhs, const GeoCoord &rhs){
    return (lhs.latitude == rhs.latitude) and (lhs.longitude == rhs.longitude);
}

//returns string of of direction based on angles
string directionOfLine(const GeoSegment& gs){
    double angle = angleOfLine(gs);
    if(angle > 337.5)
        return "east";
    else if(angle > 292.5)
        return "southeast";
    else if(angle > 247.5)
        return "south";
    else if(angle > 202.5)
        return "southwest";
    else if(angle > 157.5)
        return "west";
    else if(angle > 112.5)
        return "northwest";
    else if(angle > 67.5)
        return "north";
    else if(angle > 22.5)
        return "northeast";
    else /*if(angle >= 0 or angle <= 22.5)*/
        return "east";

}

