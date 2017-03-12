//
//  main.cpp
//  HW2
//
//  Created by Kevin on 2/5/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    stack<Coord> coordStack;
    
    Coord point(sr,sc);
    coordStack.push(point);
    maze[sr][sc] = '$';
    
    int row = sr;
    int col = sc;
    
    while(!coordStack.empty()){
        row = coordStack.top().r();
        col = coordStack.top().c();
//        cout << row << ", " << col << endl;
        coordStack.pop();
        if(row == er and col == ec)
            return true;
        if(maze[row - 1][col] != 'X' and maze[row - 1][col] != '$'){//North
            Coord moveNorth(row -1, col);
            coordStack.push(moveNorth);
            maze[row - 1][col] = '$';
        }
        if(maze[row][col + 1] != 'X' and maze[row][col + 1] != '$'){//East
            Coord moveEast(row, col+1);
            coordStack.push(moveEast);
            maze[row][col + 1] = '$';
        }
        if(maze[row + 1][col] != 'X' and maze[row + 1][col] != '$'){//South
            Coord moveSouth(row+1, col);
            coordStack.push(moveSouth);
            maze[row + 1][col] = '$';
        }
        if(maze[row][col - 1] != 'X' and maze[row][col - 1] != '$'){//West
            Coord moveWest(row, col-1);
            coordStack.push(moveWest);
            maze[row][col - 1] = '$';
        }
    }
    
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
