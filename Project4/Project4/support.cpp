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

bool operator<(const GeoCoord &lhs, const GeoCoord &rhs) {
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude < rhs.longitude;
    return lhs.latitude < rhs.latitude;
}

bool operator>(const GeoCoord &lhs, const GeoCoord &rhs){
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude > rhs.longitude;
    return lhs.latitude > rhs.latitude;

}

bool operator ==(const GeoCoord &lhs, const GeoCoord &rhs){
    return (lhs.latitude == rhs.latitude) and (lhs.longitude == rhs.longitude);
}

string directionOfLine(const GeoSegment& gs){
    double angle = angleOfLine(gs);
    if(angle >= 0 or angle <= 22.5)
        return "east";
    else if(angle > 22.5 or angle <= 67.5)
        return "northest";
    else if(angle > 67.5 or angle <= 112.5)
        return "north";
    else if(angle > 112.5 or angle <= 157.5)
        return "northwest";
    else if(angle > 157.5 or angle <= 202.5)
        return "west";
    else if(angle > 202.5 or angle <= 247.5)
        return "southwest";
    else if(angle > 247.5 or angle <= 292.5)
        return "south";
    else if(angle > 292.5 or angle <= 337.5)
        return "southeast";
    else //(angle > 337.5 or angle < 360)
        return "east";

}

