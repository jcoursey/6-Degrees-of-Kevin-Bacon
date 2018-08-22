/*
 * actorconnections.cpp
 * Author: Daniel Barone & Jordan Coursey
 * Date: June 2018
 *
 */

#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"
//ldfjl
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <queue>

const string WIDEST_PATH = "widestp";
const string UNION_FIND = "ufind";

using namespace std;

void unionFind(){
	return;
}

int main(int argc, char* argv[]){

	bool use_ufind = true;

	// Check for correct number of command line arguments
	if(argc > 5 || argc < 4){
		cout << argv[0] << "\nError: Incorrect Number Of Arguments" << endl;
		return -1;
	}
	
	if(argc == 5){
		if(argv[4] == WIDEST_PATH){
			use_ufind = false;
		} else if(argv[4] != UNION_FIND) {
			cout << argv[0] << "\nError: Incorrect Number Of Arguments" << endl;
			return -1;
		}
	}

	const char* movieCastFile = argv[1]; // Name of file containing movie cast information

	ifstream testPairsFile; // Initialize file stream
	ofstream pathOutput; // Initialize output file
	testPairsFile.open(argv[2]); // Open the file containing the actor pairs
	pathOutput.open(argv[3]); // Open output file

	pathOutput << "Actor1	Actor2	Year" << endl;
	ActorGraph actorGraph; // Initialize the actor graph
	actorGraph.loadFromFile(movieCastFile, true); // Populate the graph	?-unweighted edges-?

	bool haveHeader = false;

	// Read from input file
	while(testPairsFile){
		
		string s;
		
		if(!getline(testPairsFile, s)) break;

		vector<string> record;

		if(!haveHeader){
			haveHeader = true; // Skip the header
			continue;
		}

		istringstream ss( s );
		
		// Read tab separated values from line
		while(ss){
			string next;
			
			if(!getline(ss, next, '\t')) break;

			record.push_back(next);
			
		}
		if(record.size() != 2){
			cout << "\nError, need two actors to create a path." << endl;
			continue;
		}
		
		string actor1(record[0]);
		string actor2(record[1]);

		if(use_ufind){
			unionFind();
		} else {

			
							
			unordered_map<string, ActorNode*>::const_iterator root = actorGraph.actorMap.find(actor1);		 	
			unordered_map<string, ActorNode*>::const_iterator target = actorGraph.actorMap.find(actor2);		 	   	
			ActorNode* rootNode = root->second;
			ActorNode* targetNode = target->second;	
			
			actorGraph.widestPathSearch(rootNode,targetNode);	
			
				
			ActorNode* pushingNode = targetNode;
			
			priority_queue<int, vector<int>, std::greater<int>> stackPath;
		
			while(pushingNode->actorName != rootNode->actorName){

				stackPath.push(pushingNode->prevMovie->movieYear);
				
				pushingNode = pushingNode->prevActor;
				
			}

			pathOutput << actor1 << "	" << actor2 << "	" << stackPath.top() << endl;
					
			
			actorGraph.deleteMaps();
			actorGraph.loadFromFile(movieCastFile, true);

			
	
		}


	} // End read from input file while loop  
	

	//beginning of algorithm
	//
	/*
 *		list of actors (vector)
 *		list of movies 	(PQ sorted by year)
 *		1. all actors are disonnected at first(sentinel nodes)
 *		2. take all movies from year Y
 *			for( auto movie : moviesThisYear)
 *				merge each actor for a particular movie
 *		
 *				DisjointSet::disjointSet.search(target1, target2)
 *					if they find it then return the year
 *				
 *		
 *
 *
 */




	return 0;
}



