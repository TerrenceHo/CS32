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



int main(){
    ifstream infile("/Users/kho/Downloads/skeleton/mapdata.txt");
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return 0;
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

