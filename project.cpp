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
/*
void _computeAugmentingPath(struct node** ,int** ,vector<int>* , int, int,int,int);
void _breadthFirstSearch(struct node** , unordered_map<int,int>* , int,int,int,int);
int  _calculateBottleneckFlow(int** ,vector<int>* , int,int,int,int);
void _augmentResidualGraph(int**,struct node**,vector<int>* , int,int ,int , int ,int);
void _calculateMinCut(int**,struct node**, int**,int,int ,int ,int);
*/



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
			_printAugmentingPath(&aug_path,kValue);		
	}

	// Printing the max flow and the flow values
	_printMaxFlowAndValues(res_graph_am,graph_array,max_flow,nVertex);


	// Calculating the min-cut and min-cut capacity	
	_calculateMinCut(res_graph_am,res_graph_al,graph_array,nVertex,mEdges,source,sink);

	
	
	// closing the file
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




/*

// Defining the function _computeAugmentingPath()
void _computeAugmentingPath(struct node** adj_list, int** adj_mat, vector<int>* aug_path,int nVertex,int mEdges, int source,int sink)
{

	int x,y;
	int aug_flow = 0;
	vector<int>::iterator i;
	unordered_map<int,int> path;
	aug_path->clear();

	_breadthFirstSearch(adj_list, &path , nVertex, mEdges,source,sink);


	if( path.count(sink) <= 0)	
		aug_path->push_back(-1);
	else
	{
		aug_path->push_back(sink);
		x = path.at(sink);
		while(x != -1)
		{
			aug_path->push_back(x);
			x = path.at(x);
		}

	}
	

}



// Defining the function _breadthFirstSearch()
void _breadthFirstSearch(struct node** adj_list, unordered_map<int,int>* path, int nVertex,int mEdges,int source ,int sink)
{
	queue<int> q;
	int visited[nVertex+1];
	struct node* temp = NULL;
	int parent = -1;
	int node,i;

	for(i = 0; i <= nVertex; i++)
		visited[i] = 0;

	q.push(source);
	visited[source] = 1;
	path->insert(unordered_map<int,int>::value_type(source,-1));

	while(!q.empty())
	{
		node = q.front();
		q.pop();
		temp = adj_list[node];
		while(temp != NULL)
		{
			if(visited[temp->vertex] == 0) 
			{
				q.push(temp->vertex);
				visited[temp->vertex] = 1;
		                path->insert(unordered_map<int,int>::value_type(temp->vertex,node));
			}
			
			temp = temp->next;
		}
	}
}




// Defining the function _calculateBottleneckFlow()
int _calculateBottleneckFlow(int** adj_mat, vector<int>* path, int nVertex, int mEdges, int source , int sink)
{
	vector<int>::iterator it;
	int min_cap = 0;
	it = path->end();
	it--;


	min_cap = adj_mat[*it][*(it-1)];
	for(it = path->end() -1; it > path->begin(); it--)
	{
		if(adj_mat[*it][*(it-1)] < min_cap)
			min_cap = adj_mat[*it][*(it-1)];

	}

	return min_cap;
}




// Defining the function _augmentResidualGraph()
void _augmentResidualGraph(int** adj_mat,struct node** adj_list,vector<int>* aug_path,int flow,int nVertex,int mEdges,int source,int sink)
{
	vector<int>::iterator it;
	struct node* temp = NULL;	
	struct node* prev_temp = NULL;
	struct node* new_node = NULL;

	it = aug_path->end();
	it--;

	for( ; it > aug_path->begin();it--)
	{
		// Augmenting Matrix flow
		adj_mat[*it][*(it-1)] -= flow;
		adj_mat[*(it-1)][*it] += flow;
	
		// Augmenting Adjacency Flow
		temp = adj_list[*it];
		
		// if you have to remove the vertex
		if(adj_mat[*it][*(it-1)] == 0)
		{
			//if its first item to remove
			if(adj_list[*it]->vertex == *(it-1))
				adj_list[*it] = adj_list[*it]->next;
			else  // if its not the first item
			{
				while(temp->next->vertex != *(it-1))
		                	temp = temp->next;
		                temp->next = temp->next->next;
			}

		}
		else  // have to augment the flow of the vertex
		{
			// Update the capacity of the forward edge
			temp = adj_list[*it];
			while(temp->vertex != *(it-1))
				temp = temp->next;
			temp->capacity -= flow;

	
			// Update capacity of backward edge
			temp = adj_list[*(it-1)];
			while(temp != NULL && temp->vertex != *it)
				temp = temp->next;

			// if backward edge found
			if(temp != NULL)
			{
				temp->capacity += flow;
			}
			else  // if np backward edge found
			{
				new_node = (struct node*)calloc(1,sizeof(struct node));
				new_node->capacity = flow;
				new_node->vertex = *it;
				new_node->next = adj_list[*(it-1)];
				adj_list[*(it-1)] = new_node;
			}
		}
	}
}






// Defining the function _calculateMinCut()
void  _calculateMinCut(int** adj_mat,struct node** adj_list,int** graph,int nVertex,int mEdges,int source,int sink)
{
	int x = 0;
	int i;
	int min_cut_cap = 0;
        unordered_map<int,int> path;

        _breadthFirstSearch(adj_list, &path , nVertex, mEdges,source,sink);

	for(auto it = path.begin(); it != path.end() ; ++it)
	{
		for(i = 1 ; i <= nVertex ; i++)
		{
			if(path.count(i) == 0 )
				min_cut_cap += graph[it->first][i];
		}
	}
	printf("\n\nMin-cut capacity = %d \n",min_cut_cap);
	

	printf("Min-cut is \n");
       	for ( auto it = path.begin(); it != path.end(); ++it )
		if(it == path.begin())
			printf("%d",it->first); 
		else
			printf(", %d",it->first);
	printf("\n");
	


	for(i = 1; i <= nVertex; i++)
	{
		if(path.count(i) == 0)
		{
			if(x == 0)
				printf("%d",i);
			else
				printf(", %d",i);
			x++;
		}

	}
	printf("\n");
	
}
*/
