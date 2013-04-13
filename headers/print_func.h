#ifndef __print_func_H__
#define __print_func_H__
#include <vector>


// Declaring the structures
struct node{
                int vertex;
                int capacity;
                struct node* next;
           };

void _printGraph(int**,int ,int);
void _printAdjList(struct node**,int , int);
void _printAugmentingPath( std::vector<int>* ,int,FILE*);
void _printMaxFlowAndValues(int**,int**,int,int,FILE*);

#endif
