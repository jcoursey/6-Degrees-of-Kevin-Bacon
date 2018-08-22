/*
 * pathfinder.cpp
 * Author: Daniel Barone & Jordan Coursey
 * Date: June 2018
 *
 */

#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
//commit
using namespace std;

/*void printPath(ActorGraph* actorGraph, string actor1, string actor2, ofstream& pathOutput){
	
	stack<string> stackPath;
				
	stackPath.push(actor2);
	
	ActorNode* currNode = actorGraph.foundNode;
						
	while(currNode->actorName != actor1){
		Movie* currMovie = currNode->prevMovie;

		stackPath.push(currMovie->movieName);

		currNode = currNode->prevActor;
		
		stackPath.push(currNode->actorName);

	}

	string thePath;
	thePath = stackPath.top();
	stackPath.pop();

	while(!stackPath.empty()){
		pathOutput << "(" << thePath << ")--";
		
		thePath = stackPath.top();
		stackPath.pop();
		pathOutput << "[" << thePath << "]-->";
		
		thePath = stackPath.top();
		stackPath.pop();				
	}
	pathOutput << "(" << thePath << ")" << endl;

	

}*/

int main(int argc, char* argv[]){
	
	// Check for correct number of command line arguments
	if(argc != 5){
		cout << argv[0] << "\nError: Incorrect Number Of Arguments" << endl;
		return -1;
	}

	const char* movieCastFile = argv[1]; // Name of file containing movie cast information 
	
	string edgeWeightType = argv[2]; // Weighted or Unweighted edges
	bool weightedEdges = false;
	if(edgeWeightType == "w"){
		weightedEdges = true;
	}
	
	ifstream testPairsFile; // Initialize file stream
	ofstream pathOutput; // Initialize output file
	testPairsFile.open(argv[3]); // Open the file containing the actor pairs
	pathOutput.open(argv[4]); // Open output file
	pathOutput << "(actor)--[movie#@year]-->(actor)--..." << endl;
	ActorGraph actorGraph; // Initialize the actor graph
	actorGraph.loadFromFile(movieCastFile, weightedEdges); // Populate the graph
	
	bool haveHeader = false;
	bool breakOut = false;

	// Read from input file
	while(testPairsFile){
		cout<<"infile"<<endl;

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

		/* ----- Find the path ----- */

		if(!weightedEdges){

		// Get the root node from the unordered map
		unordered_map<string, ActorNode*>::const_iterator root = actorGraph.actorMap.find(actor1);		 	


		ActorNode* workingActorNode = root->second;	

		vector<ActorNode*> currentNodes;
		currentNodes.push_back(workingActorNode);			
		
		vector<ActorNode*> nextNodes;		

		bool foundIt = false;
	
		while(foundIt == false){
			
			if(currentNodes.size() == 0){
				//cout << "\nDidn't find actor2, exiting while loop." << endl;
				break;
			}
			
			for (auto actor_at_i : currentNodes){

				actorGraph.setAdjVect(actor_at_i, actor2, edgeWeightType);
				actor_at_i->searched = true;
				
				if(actor_at_i->found == true){
					foundIt = true;
					
					
					stack<string> stackPath;
				
					stackPath.push(actor2);
					
					ActorNode* currNode = actorGraph.foundNode;
										
					while(currNode->actorName != actor1){
						Movie* currMovie = currNode->prevMovie;

						stackPath.push(currMovie->movieName);

						currNode = currNode->prevActor;
						
						stackPath.push(currNode->actorName);

					}

					string thePath;
					thePath = stackPath.top();
					stackPath.pop();
					
					while(!stackPath.empty()){
						pathOutput << "(" << thePath << ")--";
						
						thePath = stackPath.top();
						stackPath.pop();
						pathOutput << "[" << thePath << "]-->";
						
						thePath = stackPath.top();
						stackPath.pop();				
					}
					pathOutput << "(" << thePath << ")" << endl;
					breakOut = true;
					break;
				}		
				if(breakOut){
					break;
				}
				
				for(auto next_node : actor_at_i->adjActors){
						nextNodes.push_back(next_node);
				}
			}
			
			if(breakOut){
				break;
			}
			currentNodes = nextNodes;
			nextNodes.clear();

		}	

		//actorGraph.actorMap.clear();
		actorGraph.deleteMaps();
		actorGraph.loadFromFile(movieCastFile, weightedEdges); 
		breakOut = false;


	} else if(weightedEdges){// end unweighted
		//std::priority_queue<ActorNode*, std::vector<ActorNode*>, compareNodes> visitedNodes;	
		// Get the first node
		unordered_map<string, ActorNode*>::const_iterator root = actorGraph.actorMap.find(actor1);		 	
		unordered_map<string, ActorNode*>::const_iterator target = actorGraph.actorMap.find(actor2);		 	   	
		ActorNode* rootNode = root->second;
		ActorNode* targetNode = target->second;
		
		//doesn't check for not found
		//start of my shit
		actorGraph.dijkSearch(rootNode,targetNode);	
		ActorNode* pushingNode = targetNode;	
		cout << "after dijkSearch" <<endl;

		
		//ActorNode* pushingNode = targetNode;
		stack<string> stackPath;
		stackPath.push(pushingNode->actorName);
		
		
		while(pushingNode->actorName != rootNode->actorName){
			//push movie
			stackPath.push(pushingNode->prevMovie->movieName);
			pushingNode = pushingNode->prevActor;
			stackPath.push(pushingNode->actorName);
						

		}
		

		string thePath;
		thePath = stackPath.top();
		stackPath.pop();
		
		while(!stackPath.empty()){
			pathOutput << "(" << thePath << ")--";
			thePath = stackPath.top();
			stackPath.pop();
			pathOutput << "[" << thePath << "]-->";
		
			thePath = stackPath.top();
			stackPath.pop();				
		}
		pathOutput << "(" << thePath << ")" << endl;
		

		actorGraph.deleteMaps();
		actorGraph.loadFromFile(movieCastFile, weightedEdges);
		//visitedNodes = priority_queue<ActorNode*, std::vector<ActorNode*>, compareNodes>();
	}	
	
			
	} // End outer while
	
	//cout << "#nodes: " << actorGraph.numActors << endl;
	//cout << "#movies: " << actorGraph.numMovies << endl;

	return 0;
}

/*
 *int main(int argc, char* argv[]){
	
	// Check for correct number of command line arguments
	if(argc != 5){
		cout << argv[0] << "\nError: Incorrect Number Of Arguments" << endl;
		return -1;
	}

	const char* movieCastFile = argv[1]; // Name of file containing movie cast information 
	
	string edgeWeightType = argv[2]; // Weighted or Unweighted edges
	bool weightedEdges = false;
	if(edgeWeightType == "w"){
		weightedEdges = true;
	}
	
	ifstream testPairsFile; // Initialize file stream
	ofstream pathOutput; // Initialize output file
	testPairsFile.open(argv[3]); // Open the file containing the actor pairs
	pathOutput.open(argv[4]); // Open output file

	pathOutput << "(actor)--[movie#@year]-->(actor)--..." << endl;
	ActorGraph actorGraph; // Initialize the actor graph
	actorGraph.loadFromFile(movieCastFile, weightedEdges); // Populate the graph
	
	bool haveHeader = false;
	bool breakOut = false;

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

		//----- Find the path -----

		if(!weightedEdges){

		// Get the root node from the unordered map
		unordered_map<string, ActorNode*>::const_iterator root = actorGraph.actorMap.find(actor1);		 	


		ActorNode* workingActorNode = root->second;	

		vector<ActorNode*> currentNodes;
		currentNodes.push_back(workingActorNode);			
		
		vector<ActorNode*> nextNodes;		

		bool foundIt = false;
	
		while(foundIt == false){
			
			if(currentNodes.size() == 0){
				//cout << "\nDidn't find actor2, exiting while loop." << endl;
				break;
			}
			
			for (auto actor_at_i : currentNodes){

				actorGraph.setAdjVect(actor_at_i, actor2, edgeWeightType);
				actor_at_i->searched = true;
				
				if(actor_at_i->found == true){
					foundIt = true;
					
					
					stack<string> stackPath;
				
					stackPath.push(actor2);
					
					ActorNode* currNode = actorGraph.foundNode;
										
					while(currNode->actorName != actor1){
						Movie* currMovie = currNode->prevMovie;

						stackPath.push(currMovie->movieName);

						currNode = currNode->prevActor;
						
						stackPath.push(currNode->actorName);

					}

					string thePath;
					thePath = stackPath.top();
					stackPath.pop();
					
					while(!stackPath.empty()){
						pathOutput << "(" << thePath << ")--";
						
						thePath = stackPath.top();
						stackPath.pop();
						pathOutput << "[" << thePath << "]-->";
						
						thePath = stackPath.top();
						stackPath.pop();				
					}
					pathOutput << "(" << thePath << ")" << endl;
					breakOut = true;
					break;
				}		
				if(breakOut){
					break;
				}
				
				for(auto next_node : actor_at_i->adjActors){
						nextNodes.push_back(next_node);
				}
			}
			
			if(breakOut){
				break;
			}
			currentNodes = nextNodes;
			nextNodes.clear();

		}	

		//actorGraph.actorMap.clear();
		actorGraph.deleteMaps();
		actorGraph.loadFromFile(movieCastFile, weightedEdges); 
		breakOut = false;


	} else if(weightedEdges){// end unweighted
		std::priority_queue<ActorNode*, std::vector<ActorNode*>, compareNodes> visitedNodes;	
		// Get the first node
		unordered_map<string, ActorNode*>::const_iterator root = actorGraph.actorMap.find(actor1);		 	
		ActorNode* currNode = root->second;
		
		currNode->dist = 0;
		currNode->searched = true;	
		vector<ActorNode*> nodesToEnque = actorGraph.setAdjVect(currNode, actor2, edgeWeightType);
		
		for(auto curr : nodesToEnque){
			visitedNodes.push(curr);
		}
		nodesToEnque.clear();
		
		while(currNode->found == false){
			currNode = visitedNodes.top();
			visitedNodes.pop();
			currNode->searched = true;
			nodesToEnque = actorGraph.setAdjVect(currNode, actor2, edgeWeightType);
			
			for(auto curr : nodesToEnque){
				visitedNodes.push(curr);
			}
			//cout << visitedNodes.size() << endl;
			nodesToEnque.clear();
			
		}
		
		stack<string> stackPath;
				
		stackPath.push(actor2);
	
		ActorNode* foundAct2 = actorGraph.foundNode;
				
		while(foundAct2->actorName != actor1){
			Movie* currMovie = foundAct2->prevMovie;

			stackPath.push(currMovie->movieName);

			foundAct2 = foundAct2->prevActor;
		
			stackPath.push(foundAct2->actorName);

		}

		string thePath;
		thePath = stackPath.top();
		stackPath.pop();

		while(!stackPath.empty()){
			pathOutput << "(" << thePath << ")--";
		
			thePath = stackPath.top();
			stackPath.pop();
			pathOutput << "[" << thePath << "]-->";
		
			thePath = stackPath.top();
			stackPath.pop();				
		}
		pathOutput << "(" << thePath << ")" << endl;
		

		actorGraph.deleteMaps();
		actorGraph.loadFromFile(movieCastFile, weightedEdges);
		visitedNodes = priority_queue<ActorNode*, std::vector<ActorNode*>, compareNodes>();
	}	
	
			
	} // End outer while
	
	//cout << "#nodes: " << actorGraph.numActors << endl;
	//cout << "#movies: " << actorGraph.numMovies << endl;

	return 0;

 *
 */

