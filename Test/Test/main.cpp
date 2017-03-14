//
//  main.cpp
//  Test
//
//  Created by Kevin on 2/5/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include <string>
#include <utility>
#include <random>
#include <map>
#include <cmath>
#include <fstream>
using namespace std;

struct GeoCoord
{
    GeoCoord(std::string lat, std::string lon)
    : latitudeText(lat), longitudeText(lon), latitude(std::stod(lat)), longitude(std::stod(lon))
    {}
    
    GeoCoord()
    : latitudeText("0"), longitudeText("0"), latitude(0), longitude(0)
    {}
    
    std::string latitudeText;
    std::string longitudeText;
    double      latitude;
    double      longitude;
};

struct GeoSegment
{
    GeoSegment(const GeoCoord& s, const GeoCoord& e)
    : start(s), end(e)
    {}
    
    GeoSegment()
    {}
    
    GeoCoord start;
    GeoCoord end;
};

void testInfile(){
    ifstream infile("/Users/kho/Downloads/skeleton/mapdata.txt");
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return;
    }
    string s;
    string t;
    int k;
    int j;
    getline(infile, s);
    cout << s << endl;
    string g1, g2 = "";
    infile >> g1;
    infile >> g2;
    g1.erase(g1.end() - 1);
    cout << g1 << endl;
    cout << g2 << endl;
    
    infile >> g1;
    g2 = g1.substr(g1.find(",") + 1,g1.size());
    g1 = g1.substr(0, g1.find(","));
    cout << g1 << endl;
    cout << g2 << endl;
    
    infile >> k;
    cout << k << endl;
    for(int i = 0; i < k; i++){
        
    }
    //    while(getline(infile, s)){
    //        cout << s << endl;
    //        getline(infile, t);
    //        cout << t << endl;
    //        infile >> k;
    //        infile.ignore(10000, '\n');
    //        for(int i = 0; i < k; i++){
    //            getline(infile, s);
    //            cout << s << endl;
    //        }
    //    }
    
}

void testStructCopy(GeoSegment & gs){ //Test for object copying
    GeoCoord geo1("111", "111");
    GeoCoord geo2("222", "222");
    GeoSegment geoseg1(geo1, geo2);
    gs = geoseg1;
    
    cout << gs.start.latitude << endl;
    
    cout << "All tests passed" << endl;
}

int main(){
//    testInfile();
    GeoSegment test_g_seg;
    testStructCopy(test_g_seg);
    cout << test_g_seg.start.latitude << endl;
}

