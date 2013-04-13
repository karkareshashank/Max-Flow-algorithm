#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include "./headers/print_func.h"
#include "./headers/functions.h"

using namespace std;

// Global variable declarations


// Declaring the functions
void _initArray(int**,int ,int);
void _readingInput(int**, FILE*,int ,int);
void _initResidualGraph(int**,struct node**,int**,int , int);



// Main function 
int main(int argc,char** argv)
{
	// Defining the variables
	int nVertex;
	int mEdges;
	int source;
	int sink;
	int kValue;
	int max_flow = 0;

	int i, j;
	int iterationCount = 0;
	int flow;
	FILE* fp;
	FILE* ofp;
	int** graph_array;
	int** res_graph_am;
	struct node**   res_graph_al;
	bool flag = true;

	vector<int> aug_path;
	vector<int>::iterator it;
	

	// Opening the input file 
	fp = fopen("./input.txt","r");
	if(fp == NULL)
	{
		fprintf(stderr,"Error: %s \n",strerror(errno));
		exit(1);
	}

	ofp = fopen("./output.txt","w+");
	if(ofp == NULL)
	{
		fprintf(stderr,"Error: %s \n",strerror(errno));
		exit(1);
	}

	// Scaning number of vertex and edges
	fscanf(fp,"%d %d",&nVertex,&mEdges);
	
	
	// Allocating the memory for the graph array
	graph_array = (int**)calloc(nVertex+1, sizeof(int*));
	for(i = 0; i <= nVertex; i++)
		graph_array[i] = (int*)calloc(nVertex+1, sizeof(int));
	
	// Allocating the memory for residual_graph_adjacency_matrix
	res_graph_am = (int**)calloc(nVertex+1,sizeof(int*));
	for(i = 0; i <= nVertex; i++)
		res_graph_am[i] = (int*)calloc(nVertex+1,sizeof(int));

	// Allocating the memory for residual_graph_adjacency_list
	res_graph_al = (struct node**)calloc(nVertex+1,sizeof(struct node*));
	for(i = 0; i <= nVertex; i++)
		res_graph_al[i] = NULL;


	// Scanning source, sink and kValues
	fscanf(fp,"%d %d %d",&source,&sink,&kValue);
	
	// initializing the array
	_initArray(graph_array,nVertex,mEdges);
	_initArray(res_graph_am,nVertex,mEdges);
	_readingInput(graph_array,fp,nVertex,mEdges);
	_initResidualGraph(res_graph_am,res_graph_al,graph_array,nVertex,mEdges);



	while(true)
	{
		iterationCount++;
		_computeAugmentingPath(res_graph_al, res_graph_am, &aug_path, nVertex, mEdges, source, sink);
		if(*(aug_path.end() - 1) == -1)
			break;

		flow = _calculateBottleneckFlow(res_graph_am,&aug_path,nVertex,mEdges,source,sink);
		max_flow += flow;
		_augmentResidualGraph(res_graph_am,res_graph_al,&aug_path,flow,nVertex,mEdges,source,sink);

		if(iterationCount == kValue)
			_printAugmentingPath(&aug_path,kValue,ofp);		
	}

	// Printing the max flow and the flow values
	_printMaxFlowAndValues(res_graph_am,graph_array,max_flow,nVertex,ofp);


	// Calculating the min-cut and min-cut capacity	
	_calculateMinCut(res_graph_am,res_graph_al,graph_array,nVertex,mEdges,source,sink,ofp);

	
	
	// closing the file
	fclose(ofp);
	fclose(fp);
	return 0;
}



// Defining the function _initArray()
void _initArray(int** array,int nVertex, int mEdges)
{
	int i , j;
	for(i = 0; i <= nVertex; i++)
		for(j = 0; j <= nVertex; j++)
			array[i][j] = 0;
}


// Defining the function _readingInput()
void _readingInput(int** array, FILE* fp,int nVertex, int mEdges)
{
	int i;
	int head,tail;
	int capacity;

	for(i = 0; i < mEdges; i++)
	{
		fscanf(fp,"%d %d %d",&tail,&head,&capacity);
		array[tail][head] = capacity;
	}
}




// Defining the fucntion _initResidualGraph()
void _initResidualGraph(int** adj_mat,struct node** adj_list,int** graph,int nVertex, int mEdges)
{
	int i, j;
	struct node*  temp1;
	struct node*  temp2;

	for(i = 1;i <=nVertex;i++)
	{
		for(j=1;j <= nVertex;j++)
		{
			if(graph[i][j] > 0)
			{
				adj_mat[i][j] = graph[i][j];	
				temp1 = (struct node*)calloc(1,sizeof(struct node));
				temp1->next = NULL;
				temp1->capacity = graph[i][j];
				temp1->vertex = j;
	
				temp1->next = adj_list[i];
				adj_list[i] = temp1;
				
			}
		}
	}
	

}

