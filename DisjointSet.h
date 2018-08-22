#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include "Movie.h"
#include "ActorNode.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

#define INF numeric_limits<int>::max()

using namespace std;
class Movie;

class compareSetsMovie{
  public:
    bool operator()(Movie*& firstNode, Movie*& secondNode) const {
      return firstNode->movieYear > secondNode->movieYear;
    }
};

class DisjointSet{

public:
	//initially size is number of actors
	
	set<ActorNode*>disjoints;
	priority_queue<Movie*, vector<Movie*>, compareSetsMovie> movieQueue;
	 	
//member functions

	ActorNode* find(ActorNode* targetActor);
	
	void unionize(ActorNode* parentNode, ActorNode* childNode);		
	

	~DisjointSet();
	
  

private:


};

#endif // DISJOINT_SET_H
