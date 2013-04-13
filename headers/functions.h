#ifndef __functions_H__
#define __functions_H__

void _computeAugmentingPath(struct node** ,int** ,std::vector<int>* , int, int,int,int);
void _breadthFirstSearch(struct node** , std::unordered_map<int,int>* , int,int,int,int);
int  _calculateBottleneckFlow(int** ,std::vector<int>* , int,int,int,int);
void _augmentResidualGraph(int**,struct node**,std::vector<int>* , int,int ,int , int ,int);
void _calculateMinCut(int**,struct node**, int**,int,int ,int ,int,FILE*);


#endif
