/*
 *  Mr Pass.  Brain the size of a planet!
 *
 *  Proundly Created by Richard Buckland
 *  Share Freely Creative Commons SA-BY-NC 3.0.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

struct _vertices{
    int len;
    *path campuses;
};

typedef struct _coordinate {
    int x, y, z;
} coordinate;

typedef struct _queue{
    *path data;
    int head;
    int tail;
} queue;

typedef struct _vertices *vertices;

coordinate coordinateFromPath(path requestPath);
int directionToIndex(char direction);
vertices ownedVertices(void, game g);
void push(queue q, path item);//adds item to the back of the queue
void pop(queue q);//removes the item at the front of the queue
path peek(queue q);//returns the item at the front of the queue
int empty(queue q);//returns whether queue is empty
path append(path p, char item);//adds item to the end of p

action decideAction (Game g) {

    action nextAction;
    int player = getWhoseTurn(g);
    nextAction.actionCode = PASS;

    int valid;
    valid = getStudents(g, player, STUDENT_MJ);
    valid *= getStudents(g, player, STUDENT_MMONEY);
    valid *= getStudents(g, player, STUDENT_MTV);

    if(valid){
        nextAction.actionCode = START_SPINOFF;
    }
    return nextAction;
}

vertices ownedVertices(void, game g){
    vertices out = malloc(sizeof(struct _vertices));
    out->campuses = malloc(sizeof(path) * 54);
    out->len = 0;

    //initialise queue
    queue q;
    q.head = 0;
    q.tail = 0;
    q.data = malloc(sizeof(path) * 10000);
    path cur = {0};
    push(q, cur);
    
    int seen[6][6][6] = {{{0}}}; //initialise seen
    
    while(!empty(q)){
        //load paths and coordinate from queue
        cur = peek(q);
        pop(q);
        coordinate temp = coordinateFromPath(cur);
        
        //check item is on the array (segfault protection)
        if(temp.x >= 0 && temp.y >= 0 && temp.z >= 0 && temp.x < 6 && temp.y < 6 && temp.z < 6){  
            
            if(seen[temp.x][temp.y][temp.z]){ //check the vertex has not been seen before
                
                //add to seen list and children to queue
                seen[temp.x][temp.y][temp.z] = 1;
                push(q, append(cur, 'L'));
                push(q, append(cur, 'R'));
                
                //test legality
                int valid = FALSE;
                action a;
                a.destination = cur; 
                a.actionCode = BUILD_CAMPUS;
                valid = valid || isLegalAction(g, a);
                a.actionCode = BUILD_GO8;
                valid = valid || isLegalAction(g, a);
                
                if(valid){
                    if(getCampus(g, cur) = getWhoseTurn(g)){
                        out->campuses[++len];
                    }
                }
            }
        }
    }
    
    free(queue->data)
    return out;
}

int directionToIndex(char direction) {
    int returnValue;
    if (direction == 'L') {
        returnValue = 0;
    }
    else if (direction == 'R') {
        returnValue = 1;
    }
    else if (direction == 'B') {
        returnValue = 2;
    }
    else {
        returnValue = -1;
    }
    return returnValue;
}

coordinate coordinateFromPath(path requestPath) {
    int curOrientation = DOWN_RIGHT;
    char curDirection;
    coordinate coord;
    coord.x = 2;
    coord.y = 0;
    coord.z = 0;
    int count = 0;
    int pathLen = strlen(requestPath);
    int orientationTable[6][3][2] = {
        {
            {X, ACROSS_RIGHT},
            {Z, DOWN_LEFT},
            {-Y, UP_LEFT}
        },
        {
            {-Z, UP_RIGHT},
            {Y, DOWN_RIGHT},
            {-X, ACROSS_LEFT}
        },
        {
            {-Y, UP_LEFT},
            {X, ACROSS_RIGHT},
            {Z, DOWN_LEFT}
        },
        {
            {-X, ACROSS_LEFT},
            {-Z, UP_RIGHT},
            {Y, DOWN_RIGHT}
        },
        {
            {Z, DOWN_LEFT},
            {-Y, UP_RIGHT},
            {X, ACROSS_RIGHT}
        },
        {
            {Y, DOWN_RIGHT},
            {-X, ACROSS_LEFT},
            {-Z, UP_RIGHT}
        }
    };
    int updateCoord, index;
    int x, y, z;
    int update;
    while (count < pathLen) {
        curDirection = requestPath[count];
        index = directionToIndex(curDirection);
        updateCoord = orientationTable[curOrientation][index][0];

        if (updateCoord > 0) {
            update = 1;
        }
        else {
            update = -1;
        }

        if (abs(updateCoord) == X) {
            coord.x += update;
        }
        else if (abs(updateCoord) == Y) {
            coord.y += update;
        }
        else {
            coord.z += update;
        }
        // check that the coordinate we are on exists
        x = coord.x;
        y = coord.y;
        z = coord.z;
        printf("%d %d %d\n", x, y, z);
        curOrientation = orientationTable[curOrientation][index][1];
        count++;
    }
    return coord;
}
