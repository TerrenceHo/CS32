//
//  History.hpp
//  Project1
//
//  Created by Kevin on 1/10/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#ifndef History_hpp
#define History_hpp

#include "globals.h"
class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    
    //helper function
    bool isPosInBounds(int r, int c) const;
};

#endif /* History_hpp */
