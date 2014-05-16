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

typedef struct _queue{
    *path data;
    int head;
    int tail;
} queue;

typedef struct _vertices *vertices;

vertices ownedVertices(void);
void push(queue q, int item);
void pop(queue q);
path peek(queue q);

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

vertices ownedVertices(void){
    vertices out = malloc(sizeof(struct _vertices));
    vertices->campuses = malloc(sizeof(path) * 54);
    
    queue q;
    q.head = 0;
    q.tail = 0;
    q.data = malloc(sizeof(path)*10000);


