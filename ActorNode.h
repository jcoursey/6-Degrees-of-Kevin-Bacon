#ifndef ACTOR_NODE_H
#define ACTOR_NODE_H

#include "Movie.h"

#include <iostream>
#include <vector>
#include <string>
#include <limits>

#define INF numeric_limits<int>::max()

using namespace std;
class Movie;
class ActorNode{

public:
	//actor's name 
	string actorName;
	//constructor
	ActorNode(string name);

	//vector of actornodes that are adjacent to this actor i.e. actors that have been in a movie with this guy
	vector<ActorNode*> adjActors;

	vector<Movie*>	moviesIn;
	
  	//distance from root node to current node
	int dist=INF;
	int width=-INF;
	ActorNode* prevActor;
	Movie* prevMovie;
	Movie* checkingMovie;
	int weight;	
	bool sentinel = true;
	bool found = false;
		
	bool searched = false;

private:


};

#endif // ACTOR_NODE_H
