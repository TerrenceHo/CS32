//
//  History.cpp
//  Project1
//
//  Created by Kevin on 1/10/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include "History.h"
#include <iostream>
#include "globals.h"
using namespace std;

History::History(int nRows, int nCols){
    m_rows = nRows;
    m_cols = nCols;
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols;j++){
            m_grid[i][j] = 0;
        }
    }
}

bool History::record(int r, int c){
    if(!isPosInBounds(r, c))
        return false;
    m_grid[r-1][c-1]+=1;
    return true;
}

void History::display() const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if (m_grid[i][j] == 0)
                displayGrid[i][j] = '.';
            else if (m_grid[i][j] > 0 and m_grid[i][j] <26)
                displayGrid[i][j] = (char)('A' + m_grid[i][j] - 1);
            else
                displayGrid[i][j] = 'Z';
        }
    }
    
    
    // Draw the grid
    clearScreen();
    for (r = 1; r <= m_rows; r++)
    {
        for (c = 1; c <= m_cols; c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}


bool History::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}
