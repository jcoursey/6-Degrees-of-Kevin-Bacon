#include "ActorNode.h"
#include "Movie.h"

#include <iostream>

using namespace std;

ActorNode::ActorNode(string name) : actorName(name), prevActor(nullptr),
				    prevMovie(nullptr){}
