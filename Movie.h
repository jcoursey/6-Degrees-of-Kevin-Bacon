#ifndef MOVIE_H
#define MOVIE_H

#include "ActorNode.h"

#include <iostream>
#include <vector>
#include <string>
#define CURR_YEAR 2015
 
using namespace std;

class ActorNode;
class Movie {

public:
	
	string movieName;
	int movieYear;			
	std::vector<ActorNode*> actorsInMovie;
	//int weight = 1+(CURR_YEAR - movieYear);
	
	Movie(string name);

	bool searched = false;
							


//private:

};

#endif




