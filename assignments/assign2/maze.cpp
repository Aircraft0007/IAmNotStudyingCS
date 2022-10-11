// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function takes two arguments : a Grid<bool> referenced maze represents the selected maze and a GridLocation cur represents the current location.
 * We aim to find all the feasible neighboring locations of the current location within the bound of the selected maze and
 * excluding any neighbors which goes on walls.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    //obtain the x-y coordinates of the current location
    int x = cur.row;
    int y = cur.col;
    //We introduce arrays dx and dy to store all the directions that the currect location could possibly go next
    int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
    // Loop through all four possible directions
    for (int i = 0; i < 4; ++i) {
        //obtain the x-y coordinates of after going through a specific direction
        int tx = x+dx[i];
        int ty = y+dy[i];
        //check whether the selected direction is feasible to go through, if so , then add it as neighbors location.
        if(maze.inBounds(tx,ty)&&maze[tx][ty]==true){
            neighbors.add(GridLocation(tx,ty));
//            maze[tx][ty]=false;
        }
    }
    return neighbors;
}

/* This function takes two arguments: the Grid<bool> referenced maze represents the selected maze , the Stack<GridLocation> path
 * represents one possible path to solve the maze.
 * The aim of the function is to check whether the given path is a correct path of the selected maze.
 * We testing the path with 5 types of error : 1. not end at maze exit 2. not start at maze entry 3. go through wall
 * 4. Teleport 5. Revisit
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEnter ={0,0};
    Set<GridLocation> neighbors;
    Set<GridLocation> visitedLocations;
    GridLocation curLocation;
    GridLocation preLocation;

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    // Loop through all of the Stack elements until it reaches the entrance or bottom.
    while((curLocation=path.pop())!=mazeEnter||path.size()!=0){

        //obtain the x-y coordinates of the current location
        int cur_x = curLocation.row;
        int cur_y = curLocation.col;
        // check whether the current location is a wall or a passway
        // If the current location is a wall, then report error.
        if(maze[cur_x][cur_y]==false){
            error("Path go through wall");
        }


        if(visitedLocations.contains(curLocation)){
            error("Revisit");
        }else{
            visitedLocations.add(curLocation);
        }
        //Check whether the path is empty after pop the current location, if so, then break here.
        //Since the preLocation does not exist in an empty Stack.
        if(path.size()==0){
            break;
        }
        preLocation = path.peek();
        neighbors = generateValidMoves(maze,preLocation);
        if(!neighbors.contains(curLocation)){
            error("Teleport");
        }



    }
    //check if the bottom element equals to the entry location
    if(curLocation!=mazeEnter){
        error("The path not begin at maze entry");
    }
}

/* This function takes one argument Grid<bool> & maze which represents the chosen maze to solve.
 * The aim of this function is find the shortest path solution of the given maze deploying BFS Algorithm
 * while raising error cases to exclude invalid maze and report unsolvable maze.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    Stack<GridLocation> curPath;
    Stack<GridLocation> temp;
    Queue<Stack<GridLocation>> allpaths;
    Set<GridLocation> neighbors;
    Set<GridLocation> visited;
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEnter = {0,0};
    if(!(maze.numRows()>=2&&maze.numCols()>=2)){
        error("invalid maze");
    }
    if(maze[0][0]==false||maze[maze.numRows()-1][maze.numCols()-1]==false){
        error("the entry or the exit is not accessible");
    }

    // Create a starting path : includes only the entry point

    Stack<GridLocation> entryPath = {mazeEnter};
    // add the starting path to queue
    allpaths.enqueue(entryPath);
    while(!allpaths.isEmpty()){
        //obtain the front queue as current path
        curPath = allpaths.peek();
        MazeGraphics::highlightPath(curPath, "blue", 3);
        //obtain the end point of the current path
        GridLocation cur = curPath.peek();
        //if the end point of the current path is the mazeExit , then set the current path as the final path and end the search.
        if(cur==mazeExit){
            path = curPath;
            break;
        }
        // if the current path is not end with mazeExit, then continue searching for its neighoring locations.
        neighbors= generateValidMoves(maze,cur);
        for(GridLocation neighbor: neighbors){
            temp = curPath;
            // check whether the  chosen neighbor location is valid or not ,
            // if so, add the neighbor location to the current path and enqueue the path.
            if(!visited.contains(neighbor)){
                temp.push(neighbor);
                allpaths.enqueue(temp);
                visited.add(neighbor);
            }
        }
        allpaths.dequeue();
    }

    if(path.isEmpty()){
        error("the maze is not solvable");
    }



    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location in the center of 3x3 grid with all walls") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, false}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location in the center of 1x1 grid with no walls") {
    Grid<bool> maze = {{true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location at the edge of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {2, 2};
    Set<GridLocation> expected = {{1,2},{2,1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("failed to solve an invalid 1x1 maze") {
     Grid<bool> invalid_maze = {{true}};
     EXPECT_ERROR(solveMaze(invalid_maze));

}
STUDENT_TEST("inaccessible entry or exit") {
     Grid<bool> inaccessible_entry = {{false, true, true},
                                     {true, true, true},
                                     {true, true, true}};
     Grid<bool> inaccessible_exit = {{true, true, true},
                                     {true, true, true},
                                     {true, true, false}};
     Grid<bool> inaccessible_entrynexit = {{false, true, true},
                                     {true, true, true},
                                     {true, true, false}};
     EXPECT_ERROR(solveMaze(inaccessible_entry));
     EXPECT_ERROR(solveMaze(inaccessible_exit));
     EXPECT_ERROR(solveMaze(inaccessible_entrynexit));

}
STUDENT_TEST("unsovable maze") {
     Grid<bool> unsovable_maze =    {{true, false, false},
                                     {false, false, false},
                                     {false, false, true}};

     EXPECT_ERROR(solveMaze(unsovable_maze));
}
//STUDENT_TEST("Big Maze: solveMaze on file 33x41") {
//    Grid<bool> maze;
//    readMazeFile("res/33x41.maze", maze);
//    Stack<GridLocation> soln = solveMaze(maze);

//    EXPECT_NO_ERROR(validatePath(maze, soln));
//}

STUDENT_TEST("Small Maze: solveMaze on file 2x2") {
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


