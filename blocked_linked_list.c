// this is a form of list of linked list
// each upper level is a block representing 
// a range of income values
// the lower levels correspind to all the people 
// within the range

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCKSIZE 5000
#define RANDOM_NUMBERS 100000

typedef struct Person {
    char name[50];  // the name of the person
    int income;     // the income of the person
    struct Person* next;
} Person;

typedef struct BlockNode {
    int minIncome;  // including minimum income
    int maxIncome;  // excluding maximum income
    struct BlockNode* next;
    struct Person* personHead;
} BlockNode;

BlockNode* head = NULL;

BlockNode* blockinit() {
    // initial three blocks
    // each block is 5000 wide
    BlockNode* block1 = (BlockNode*)malloc(sizeof(BlockNode));
    BlockNode* block2 = (BlockNode*)malloc(sizeof(BlockNode));
    BlockNode* block3 = (BlockNode*)malloc(sizeof(BlockNode));
    // 0 - 5000
    block1->minIncome = 0;
    block1->maxIncome = BLOCKSIZE;
    // 5000 - 10000
    block2->minIncome = block1->maxIncome;
    block2->maxIncome = block1->maxIncome + BLOCKSIZE;
    // 10000 - 15000
    block3->minIncome = block2->maxIncome;
    block3->maxIncome = block2->maxIncome + BLOCKSIZE;
    block1->next = block2;
    block2->next = block3;
    
    return block1; // return the head block
}

// use the last block and add a new block to the 
// next block
void addBlock(BlockNode* blocknode) {
    BlockNode* newBlock = (BlockNode*)malloc(sizeof(BlockNode)); // allocate new block
    blocknode->next = newBlock;  // set next block
    newBlock->minIncome = blocknode->maxIncome;
    newBlock->maxIncome = blocknode->maxIncome + BLOCKSIZE;
}

void insert(char name[], int income) {
    // initialize new Person
    Person* newPerson = (Person*)malloc(sizeof(Person));
    newPerson->income = income;
    strcpy(newPerson->name, name);
    newPerson->next = NULL;

    BlockNode *cur = head;
    // locate the block
    while (cur->maxIncome <= income) {
        if (cur->next == NULL) addBlock(cur);
        cur = cur->next;
    }

    // insert in the block
    Person* curPerson = cur->personHead;
    Person* prevPerson = NULL;

    while (curPerson != NULL) {
        prevPerson = curPerson;
        curPerson = curPerson->next;
    }

    if (prevPerson == NULL) {
        newPerson->next = cur->personHead;
        cur->personHead = newPerson;
    } else {
        prevPerson->next = newPerson;
        newPerson->next = curPerson;
    }
}

void range_search(int value, int flag) {
    if (head == NULL) return;  // if the list is empty
    BlockNode* curBlock = head;
    Person* curPerson;

    if (flag == 0) {  // lower than
        while (curBlock != NULL) {
            if (curBlock->maxIncome <= value) {
                curPerson = curBlock->personHead;
                while (curPerson != NULL) {
                    //printf("%s\n", curPerson->name);
                    curPerson = curPerson->next; // next person in the block 
                }
                curBlock = curBlock->next;  // go to next block
            } else if (curBlock->maxIncome > value) {  // this is the final block
                curPerson = curBlock->personHead;
                while (curPerson != NULL && curPerson->income < value) {
                    //printf("%s\n", curPerson->name);
                    curPerson = curPerson->next;
                }
                break;
            }
        }
    } else if (flag == 1) {  // greater than or equal to
        while (curBlock != NULL) {
            if (curBlock->maxIncome <= value) {
                curBlock = curBlock->next;  // next block
            }
            else if (curBlock->maxIncome > value) {
                curPerson = curBlock->personHead;
                while (curPerson != NULL && curPerson->income >= value) {
                    //printf("%s\n", curPerson->name);
                    curPerson = curPerson->next;
                }
                curBlock = curBlock->next;
            }
        }
    }
}

void range_search2(int start, int end) {
    BlockNode* curBlock = head;
    Person* curPerson;
    while (curBlock != NULL) {
        if (curBlock->maxIncome <= start) curBlock = curBlock->next;
        else if (curBlock->maxIncome > start) {
            // check this block
            curPerson = curBlock->personHead;
            while (curPerson != NULL && curPerson->income >= start && curPerson->income < end) {
                printf("%d\n", curPerson->income);
                curPerson = curPerson->next;
            }
            if (curBlock->maxIncome <= end) {  // we need to go thorugh more blocks
                curBlock = curBlock->next;
            } else {  // no more blocks
                break;
            }
        }    
    }
}

int main() {
    struct timespec begin, end;

    // generate random numbers
    int lower = 0, upper = 500000;
    srand(time(0));  // random seed

    head = blockinit();  // initally has 3 blocks
    
    printf("Inserting 100000 records...\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
    for (int i=0; i<RANDOM_NUMBERS; i++) {
        insert("_", (rand() % (upper - lower + 1)) + lower);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("Total time = %f seconds\n\n", (end.tv_nsec - begin.tv_nsec)/1000000000.0 + (end.tv_sec - begin.tv_sec));

    printf("range search income lower than 50000\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
    range_search(50000, 0);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("Total time = %f seconds\n\n", (end.tv_nsec - begin.tv_nsec)/1000000000.0 + (end.tv_sec - begin.tv_sec));

    //range_search(30000, 1);

    printf("Range Search2 income from 50000 to 400000\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
    range_search2(1000, 80000);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("Total time = %f seconds\n\n", (end.tv_nsec - begin.tv_nsec)/1000000000.0 + (end.tv_sec - begin.tv_sec));

    return 0;
}
