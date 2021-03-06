#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stdlib.h>
#include <algorithm>
#include <limits>
#include <set>
#include "functions.h"
#include "print_func.h"


using namespace std;

// Defining the function _computeAugmentingPath()
void _computeAugmentingPath(struct node** adj_list, int** adj_mat, vector<int>* aug_path,int nVertex,int mEdges, int source,int sink)
{

	int x,y;
	int aug_flow = 0;
	vector<int>::iterator i;
	unordered_map<int,int> path;
	aug_path->clear();

	_breadthFirstSearch(adj_list, &path , nVertex, mEdges,source,sink);
//	_modifiedDijkstraForWidestPath(adj_list, &path , nVertex, mEdges,source,sink);

	if( path.count(sink) <= 0)	
		aug_path->push_back(-1);
	else
	{
		aug_path->push_back(sink);
		x = path.at(sink);
		while(x != -1)
		{
			aug_path->push_back(x);
			if(x == source)
				break;
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
			{
				adj_list[*it] = adj_list[*it]->next;
				temp->next = NULL;
				free(temp);
			}
			else  // if its not the first item
			{
				while(temp->next->vertex != *(it-1))
		                	temp = temp->next;
				prev_temp = temp->next;
			        temp->next = temp->next->next;
				prev_temp->next = NULL;
				free(prev_temp);
			}

		}
		else  // have to augment the flow of the vertex
		{
			// Update the capacity of the forward edge
			temp = adj_list[*it];
			while(temp->vertex != *(it-1))
				temp = temp->next;
			temp->capacity -= flow;
		}
	
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






// Defining the function _calculateMinCut()
void  _calculateMinCut(int** adj_mat,struct node** adj_list,int** graph,int nVertex,int mEdges,int source,int sink,FILE* ofp)
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
	fprintf(ofp,"\n\nMin-cut capacity = %d \n",min_cut_cap);
	

	fprintf(ofp,"Min-cut is \n");
       	for ( auto it = path.begin(); it != path.end(); ++it )
		if(it == path.begin())
			fprintf(ofp,"%d",it->first); 
		else
			fprintf(ofp,", %d",it->first);
	fprintf(ofp,"\n");
	


	for(i = 1; i <= nVertex; i++)
	{
		if(path.count(i) == 0)
		{
			if(x == 0)
				fprintf(ofp,"%d",i);
			else
				fprintf(ofp,", %d",i);
			x++;
		}

	}
	fprintf(ofp,"\n");
	
}


// Defining the function _modifiedDijkstraForWidestPath()
void _modifiedDijkstraForWidestPath(struct node** adj_list, unordered_map<int,int>* path, int nVertex,int mEdges,int source ,int sink)
{
	int i = 0;
	int count = 0;
	int weight[nVertex];	
	unordered_map<int,int> Q;
	int min_vertex = source;
	int min_value = std::numeric_limits<int>::max();
	int alt;
	struct node* temp;
	


	path->clear();	
	_breadthFirstSearch(adj_list, path , nVertex, mEdges,source,sink);
	if( path->count(sink) == 0)
		return;
	path->clear();

	

	weight[0]  = std::numeric_limits<int>::max();
	for(i = 1;i <= nVertex;i++)
	{
		weight[i] = std::numeric_limits<int>::max();
		Q.insert(unordered_map<int,int>::value_type(i,i));
	}
	path->insert(unordered_map<int,int>::value_type(source,-1));
	
	while(!Q.empty())
	{
		
		min_value = 0;
		if(count == 0)
		{
			min_vertex = source;
			min_value  = weight[min_vertex];
			count++;
		}
		else
		{
			for(auto it = Q.begin();it != Q.end();it++)
			{
				if(min_value < weight[it->first] && weight[it->first] < std::numeric_limits<int>::max())
				{
					min_vertex = it->first;
					min_value = weight[min_vertex];
				}
			}
		}
	
		Q.erase(Q.find(min_vertex));
		
		if(min_vertex == sink)
			break;
		
		
		
		temp = adj_list[min_vertex];
		while(temp != NULL)
		{
			
			alt = min(weight[min_vertex] , temp->capacity);
			if(weight[temp->vertex] == std::numeric_limits<int>::max())
			{
				weight[temp->vertex] = alt;
				if(path->count(temp->vertex) == 0)
					path->insert(unordered_map<int,int>::value_type(temp->vertex,min_vertex));
				else
				{
					path->erase(path->find(temp->vertex));
					path->insert(unordered_map<int,int>::value_type(temp->vertex,min_vertex));
				}

		
			}
			else if( alt > weight[temp->vertex])
			{		
				weight[temp->vertex] = alt;
				if(path->count(temp->vertex) == 0)
					path->insert(unordered_map<int,int>::value_type(temp->vertex,min_vertex));
				else
				{
					path->erase(path->find(temp->vertex));
					path->insert(unordered_map<int,int>::value_type(temp->vertex,min_vertex));
				}

			}	
			temp = temp->next;
		}
	

	}
	
	
}
