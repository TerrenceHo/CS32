//
//  support.hpp
//  Project4
//
//  Created by Kevin on 3/11/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#ifndef support_hpp
#define support_hpp

#include <stdio.h>
#include "Provided.h"

bool operator<(const GeoCoord &lhs, const GeoCoord &rhs) {
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude < rhs.longitude;
    return lhs.latitude < rhs.latitude;
}

bool operator ==(const GeoCoord &lhs, const GeoCoord &rhs){
    return (lhs.latitude == rhs.latitude) and (lhs.longitude == rhs.longitude);
}

#endif /* support_hpp */

