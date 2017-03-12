//
//  maze.cpp
//  HW3
//
//  Created by Kevin on 2/12/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    if(sr == er and sc == ec)
        return true;
    maze[sr][sc] = '!';
    if(maze[sr-1][sc] != 'X' and maze[sr-1][sc] != '!'){//North
        bool checkPath = pathExists(maze, nRows, nCols, sr-1, sc, er, ec);
        if(checkPath == true)
            return true;
    }
    if(maze[sr][sc+1] != 'X' and maze[sr][sc+1] != '!'){//EAST
        bool checkPath = pathExists(maze, nRows, nCols, sr, sc+1, er, ec);
        if(checkPath == true)
            return true;
    }
    if(maze[sr+1][sc] != 'X' and maze[sr+1][sc] != '!'){//SOUTH
        bool checkPath = pathExists(maze, nRows, nCols, sr+1, sc, er, ec);
        if(checkPath == true)
            return true;
    }
    if(maze[sr][sc-1] != 'X' and maze[sr][sc-1] != '!'){//WEST
        bool checkPath = pathExists(maze, nRows, nCols, sr, sc-1, er, ec);
        if(checkPath == true)
            return true;
    }
    return false;
}
