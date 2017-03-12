//
//  mazequeue.cpp
//  HW2
//
//  Created by Kevin on 2/5/17.
//  Copyright © 2017 Kevin. All rights reserved.
//

#include <iostream>
#include <queue>
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
    queue<Coord> coordQueue;
    Coord point(sr, sc);
    coordQueue.push(point);
    maze[sr][sc] = '$';
    
    int row = sr;
    int col = sc;
    
    while(!coordQueue.empty()){
        row = coordQueue.front().r();
        col = coordQueue.front().c();
        coordQueue.pop();
        
        if(row == er and col == ec)
            return true;
        if(maze[row - 1][col] != 'X' and maze[row - 1][col] != '$'){
            Coord moveNorth(row -1, col);
            coordQueue.push(moveNorth);
            maze[row - 1][col] = '$';
        }
        if(maze[row][col + 1] != 'X' and maze[row][col + 1] != '$'){
            Coord moveEast(row, col+1);
            coordQueue.push(moveEast);
            maze[row][col + 1] = '$';
        }
        if(maze[row + 1][col] != 'X' and maze[row + 1][col] != '$'){
            Coord moveSouth(row+1, col);
            coordQueue.push(moveSouth);
            maze[row + 1][col] = '$';
        }
        if(maze[row][col - 1] != 'X' and maze[row][col - 1] != '$'){
            Coord moveWest(row, col-1);
            coordQueue.push(moveWest);
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
