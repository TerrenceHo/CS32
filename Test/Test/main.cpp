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
#include <cctype>
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
    ifstream infile("/Users/kho/Desktop/CS32/Test/Test/testmap.txt");
//    ifstream infile("/Users/kho/Desktop/CS32/Test/Test/mapdata.txt");
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return;
    }
    string s;
    while(getline(infile,s)){
        cout << s << endl;
        
        getline(infile, s);
        string geo1 = s.substr(0, s.find(","));
        if(s[s.find(",") + 1] == ' ')
            s = s.substr(s.find(",") + 2, s.size() -1);
        else
            s = s.substr(s.find(",") + 1, s.size() -1);
        
        string geo2 = s.substr(0, s.find(" "));
        cout << geo1 << ", " << geo2 << endl;
        s = s.substr(s.find(" ") + 1, s.size()-1);
        string geo3 = s.substr(0, s.find(","));
        if(s[s.find(",") + 1] == ' ')
            s = s.substr(s.find(",") + 2, s.size() -1);
        else
            s = s.substr(s.find(",") + 1, s.size() -1);
        
        string geo4 = s;
        cout << geo3 << ", " << geo4 << endl;
        
        int numAttract = 0;
        infile >> numAttract;
        infile.ignore(10000, '\n');
        for(int i = 0; i < numAttract; i++){
            getline(infile,s);
            string attractName = s.substr(0, s.find("|"));
            cout << attractName << endl;
            s = s.substr(s.find("|") + 1, s.size() -1);
            string geo5 = s.substr(0, s.find(","));
            if(s[s.find(",") + 1] == ' ')
                s = s.substr(s.find(",") + 2, s.size() -1);
            else
                s = s.substr(s.find(",") + 1, s.size() -1);
            string geo6 = s;
            cout << geo5 << ", " << geo6 << endl;
        }
    }
    

    
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
    testInfile();
//    GeoSegment test_g_seg;
//    testStructCopy(test_g_seg);
//    cout << test_g_seg.start.latitude << endl;
    
}

