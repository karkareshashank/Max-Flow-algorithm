#include <stdio.h>
#include <vector>
#include "print_func.h"

using namespace std;

// Defining the function _printGraph()
void _printGraph(int** graph,int nVertex, int mEdges)
{
	int i ,j;
	for(i = 0; i <= nVertex; i++)
	{
		for(j = 0; j <= nVertex; j++)
			printf("%d \t",graph[i][j]);

		printf("\n");
	}
}


// Defining the function _printAdjList()
void _printAdjList(struct node** adj_list,int nVertex, int mEdges)
{
	struct node* temp;
	int i;

	for(i = 0;i <= nVertex;i++)
	{
		temp = adj_list[i];
		printf("%d --- ",i);
		while(temp != NULL)
		{
			printf("%d -> ",temp->vertex);
			temp = temp->next;
		}
		printf("\n");
	}
}

// Defining the function _printAugmentingPath()
void _printAugmentingPath(vector<int>* aug_path,int kValue,FILE* ofp)
{
	vector<int>::iterator it;
	it = aug_path->end();
	it--;

	fprintf(ofp,"The augmenting path at iteration %d is \n",kValue);
	for(;it >= aug_path->begin();it--)
		fprintf(ofp,"%d ",*it);
	fprintf(ofp,"\n");	
}



// Defining the function _printMaxFlowAndValues()
void _printMaxFlowAndValues(int** res_graph_am,int** graph_array,int max_flow,int nVertex,FILE* ofp)
{
	int i,j;
	int flow;
	fprintf(ofp,"\n\nFlow value=%d\n",max_flow);
	fprintf(ofp,"The max-flow is\n");
	
	for(i = 1;i<= nVertex;i++)
	{
		for(j=1;j <= nVertex;j++)
		{
			if(graph_array[i][j] != 0)
			{
				flow = graph_array[i][j] - res_graph_am[i][j];
				if(flow != 0)
					fprintf(ofp,"%d %d %d\n",i,j,flow);
			}

		}
	}

}



