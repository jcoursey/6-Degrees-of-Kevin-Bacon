#include "DisjointSet.h"
#include "ActorNode.h"
#include "Movie.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "ActorGraph.h"

#include <iostream>
//this is basically actorgraph but for disjoint sets
//Functions: manipulate DSNode's
using namespace std;

//constructor



//destructor
	DisjointSet::~DisjointSet()
	{}

//Functions


	DisjointSet::void initializeActors()
	{}
	//find function
	//looks for a set
	//the sentinel node(the root) is also the name of the set
	//1. traverse up tree through each nodes parent pointer until you reach the sentinel
	//2. return sentinel node
	DisjointSet::DisjointSet* find(ActorNode* targetNode);
	{
		ActorNode* searchNode = targetNode;
		while(searchNode->prevActor!=NULL)
		{
			searchNode = searchNode->prevActor;	
		}
		
		return searchNode;	

	}
	
	//union function
	//merges two sets
	DisjointSet::void unionize(ActorNode* parentNode, ActorNode* childNode);
	{
		//finding the name of the set. the sentinel node
		ActorNode* currNode = find(parentNode);		
		ActorNode* currNode2 = find(childNode);	

		
		currNode2->prevActor=currNode;
	}






	
