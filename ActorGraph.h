/*
 * ActorGraph.h
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include "ActorNode.h"
#include "Movie.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>


// Maybe include some data structures here

using namespace std;

//redefines what the priority queue compares its objects to
//Priority queue will compare distance from the root node(beginning of the search)
class compareNodes{
  public:
    bool operator()(ActorNode*& firstNode, ActorNode*& secondNode) const {
      return firstNode->dist > secondNode->dist;
    }
};

class movieCompare{
public:
	bool operator()(Movie*& firstNode, Movie*& secondNode) const{
		return firstNode->movieYear < secondNode->movieYear;
	}
};

class movieCompareWidth{
public:
	bool operator()(Movie*& firstNode, Movie*& secondNode) const{
		return firstNode->movieYear < secondNode->movieYear;
	}
};


class compareNodesWidth{
	public:
		bool operator()(ActorNode*& firstNode, ActorNode*& secondNode) const{
			return firstNode->width < secondNode->width;	
			}

};

class ActorGraph {
protected:

public:
  ActorGraph(void);
	
	int numActors = 0;
	int numMovies = 0;
	int numEdges = 0;
	int runcount = 0;	
	unordered_map<string, ActorNode*> actorMap;
	unordered_map<string, Movie*> movieMap;
	
	ActorNode* foundNode;	
        ActorNode* dijkSearch(ActorNode* rootNode, ActorNode* targetNode);
	ActorNode* widestPathSearch(ActorNode* rootNode, ActorNode* targetNode);

	void deleteMaps();
	vector<ActorNode*> setAdjVect(ActorNode* workingActorNode, string actor2, string weightType);

	bool calcDist(int distToCurr, ActorNode* visitingNode, ActorNode* refNode);
	std::pair<Movie*, int> calcWeight(ActorNode* refNode, ActorNode* visitingNode);	
	std::pair<Movie*, int> calcWeightWidth(ActorNode* refNode, ActorNode* visitingNode);	
        /** You can modify this method definition as you wish
         *
         * Load the graph from a tab-delimited file of actor->movie relationships.
         *
  * in_filename - input filename
  * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
  *
  * return true if file was loaded sucessfully, false otherwise
  */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);
	
	~ActorGraph();
	
};


#endif // ACTORGRAPH_H
