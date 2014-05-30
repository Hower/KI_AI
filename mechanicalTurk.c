/*
 *  Adley, Keerti and Kevin's AI
 *  Super OP
 *  Share Freely Creative Commons SA-BY-NC 3.0.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define DOWN_RIGHT   0
#define ACROSS_RIGHT 1
#define UP_RIGHT     2
#define UP_LEFT      3
#define ACROSS_LEFT  4
#define DOWN_LEFT    5

#define X 1
#define Y 2
#define Z 3

struct _vertices{
    int len;
    path **campuses;
};

typedef struct _coordinate {
    int x, y, z;
} coordinate;

typedef struct _queue{
    path **data;
    int head;
    int tail;
} queue;

typedef struct _vertices *vertices;

static coordinate coordinateFromPath(path requestPath);
static int directionToIndex(char direction);
vertices ownedVertices(Game g);
void push(queue *q, path *item);//adds item to the back of the queue
void pop(queue *q);//removes the item at the front of the queue
path* peek(queue *q);//returns the item at the front of the queue
int empty(queue *q);//returns whether queue is empty
path* append(path p, char item);//adds item to the end of p
action makeSpinOff(Game g);
action makeGO8(Game g, vertices campuses);
void freeVertices(vertices Vertices);

action decideAction (Game g) {

    action nextAction;
    nextAction.actionCode = PASS;
    
    vertices campuses = ownedVertices(g); 
    printf("%d\n", campuses->len);
    if(campuses->len > 0 && getGO8s(g, UNI_A) + getGO8s(g, UNI_B) + getGO8s(g, UNI_C) < 8){
        nextAction = makeGO8(g, campuses);
    
    }else{
        nextAction = makeSpinOff(g);
    
    }
    freeVertices(campuses);
    return nextAction;
}

void freeVertices(vertices Vertices){
    
    int count;
    for(count = 0; count < Vertices->len; count++){
        free(Vertices->campuses[count]);
    }
    free(Vertices->campuses);
    free(Vertices);
    return;
}

action makeSpinOff(Game g){
    
    action nextAction;
    int mj, mmoney, mtv;
    int player = getWhoseTurn(g);
    mj = getStudents(g, player, STUDENT_MJ);
    mmoney = getStudents(g, player, STUDENT_MMONEY);
    mtv = getStudents(g, player, STUDENT_MTV);
    
    if(mj && (mmoney && mtv)){
        nextAction.actionCode = START_SPINOFF;

    }else{
        
        int transferrable = 0, missing = max(0, 1 - mj);
        int transByStudent[6] = {0};
        missing += max(0, 1 - mmoney);
        missing += max(0, 1 - mtv);
        
        int student = STUDENT_BPS;
        while(student <= STUDENT_MMONEY){
            if(student == STUDENT_MMONEY){
                transferrable += max(0, (mmoney - 1) / 3);
                transByStudent[student] = max(0, (mmoney - 1) / 3);

            }else if(student == STUDENT_MJ){
                transferrable += max(0, (mj - 1) / 3);
                transByStudent[student] = max(0, (mj - 1) / 3);

            }else if(student == STUDENT_MTV){
                transferrable += max(0, (mtv - 1) / 3);
                transByStudent[student] = max(0, (mtv - 1) / 3);

            }else{
                transferrable += getStudents(g, player, student) / 3;
                transByStudent[student] = getStudents(g, player, student) / 3;

            }
            student++;
        }
        if(transferrable >= missing){
            nextAction.actionCode = RETRAIN_STUDENTS;
            student = STUDENT_BPS;
            while(student <= STUDENT_MMONEY){
                if(transByStudent[student]){
                    nextAction.disciplineFrom = student;
                    break;
                }
                student++;
            }
            if(!mmoney){
                nextAction.disciplineTo = STUDENT_MMONEY;

            }else if(!mj){
                nextAction.disciplineTo = STUDENT_MJ;
            
            }else{
                nextAction.disciplineTo = STUDENT_MTV;
            }
        }else{
        
            nextAction.actionCode = PASS;
        }
    }
    return nextAction;
}

int max(int a, int b){
    if(a > b) return a;
    else return b;
}

action makeGO8(Game g, vertices campuses){
    
    action nextAction;
    int mj, mmoney;
    int player = getWhoseTurn(g);
    mj = getStudents(g, player, STUDENT_MJ);
    mmoney = getStudents(g, player, STUDENT_MTV);
    
    if(mj >= 2 && mmoney >= 3){
        
        nextAction.actionCode = BUILD_GO8;
        strcpy(nextAction.destination, *campuses->campuses[0]); //this is a miracle if it works
    }else{
        
        int transferrable = 0, missing = max(0, 2 - mj);
        int transByStudent[6] = {0};
        missing += max(0, 3 - mmoney);
        
        int student = STUDENT_BPS;
        while(student <= STUDENT_MMONEY){
            if(student == STUDENT_MMONEY){
                transferrable += max(0, (mmoney - 3) / 3);
                transByStudent[student] = max(0, (mmoney - 3) / 3);

            }else if(student == STUDENT_MJ){
                transferrable += max(0, (mj - 2) / 3);
                transByStudent[student] = max(0, (mj - 2) / 3);

            }else{
                transferrable += getStudents(g, player, student) / 3;
                transByStudent[student] = getStudents(g, player, student) / 3;

            }
            student++;
        }
        if(transferrable >= missing){
            nextAction.actionCode = RETRAIN_STUDENTS;
            student = STUDENT_BPS;
            while(student <= STUDENT_MMONEY){
                if(transByStudent[student]){
                    nextAction.disciplineFrom = student;
                    break;
                }
                student++;
            }
            if(mmoney < 3){
                nextAction.disciplineTo = STUDENT_MMONEY;
            }else{
                nextAction.disciplineTo = STUDENT_MJ;
            }
        }else
            nextAction.actionCode = PASS;
    }
    return nextAction;
}

void push(queue *q, path *item){
    q->data[q->tail++] = item;
    return;
}

void pop(queue *q){
    q->head++;
    return;
}

path* peek(queue *q){
    return q->data[q->head];
}

int empty(queue *q){
    return q->head == q->tail;
}

path* append(path p, char item){
    int len = strlen(p);
    path *new = malloc(sizeof(path));
    strcpy(*new,p);
    *new[len++] = item;
    *new[len] = '\0';
    return new;
}

int isValid(coordinate coordinates){
    int validVertices[6][6][6] = {{{0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}, {{0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0}}, {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 1}}, {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}}, {{0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}}, {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}}};
    return validVertices[coordinates.x][coordinates.y][coordinates.z];
}

vertices ownedVertices(Game g){
    vertices out = malloc(sizeof(struct _vertices));
    out->campuses = malloc(sizeof(path) * 54);
    out->len = 0;

    //initialise queue
    queue q;
    q.head = 0;
    q.tail = 0;
    q.data = malloc(sizeof(path) * 10000);
    path *cur = malloc(sizeof(path));
    *cur[0] = 0;
    push(&q, cur);

    int seen[6][6][6] = {{{0}}}; //initialise seen
    printf("beginning search...\n");
    while(!empty(&q)){
        //load paths and coordinate from queue
        cur = peek(&q);
        pop(&q);
        coordinate temp = coordinateFromPath(*cur);

        //check item is on the array (segfault protection)
        if(temp.x >= 0 && temp.y >= 0 && temp.z >= 0 && temp.x < 6 && temp.y < 6 && temp.z < 6){

            if(!seen[temp.x][temp.y][temp.z]){ //check the vertex has not been seen before

                //add to seen list and children to queue
                seen[temp.x][temp.y][temp.z] = 1;
                push(&q, append(*cur, 'L'));
                push(&q, append(*cur, 'R'));

                if(isValid(temp)){
                    //adds item to out if owned
                    if(getCampus(g, *cur) == getWhoseTurn(g)){
                        out->campuses[out->len++] = cur;
                    }
                }
            }
        }
        free(cur);
    }
    printf("finished search\n");
    free(q.data);
    return out;
}

static int directionToIndex(char direction) {
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

static coordinate coordinateFromPath(path requestPath) {
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
        curOrientation = orientationTable[curOrientation][index][1];
        count++;
    }
    return coord;
}
