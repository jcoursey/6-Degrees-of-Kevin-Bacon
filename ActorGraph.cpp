/*
 * ActorGraph.cpp
 * Author: Daniel Barone & Jordan Coursey
 * Date: June 2018
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Movie.h"
#define INF numeric_limits<int>::max()
#define CURR_YEAR 2015

using namespace std;

ActorGraph::ActorGraph(void) : foundNode(0) {
} // Constructor

ActorGraph::~ActorGraph(){
	deleteMaps();
}
void ActorGraph::deleteMaps(){
	for(auto node : actorMap){
		delete node.second;
	}
	for(auto node : movieMap){
		delete node.second;
	}
	actorMap.clear();
	movieMap.clear();
	
}




//update widest path search
ActorNode* ActorGraph::widestPathSearch(ActorNode* rootNode, ActorNode* targetNode)
{
	priority_queue<ActorNode*, vector<ActorNode*>, compareNodes> searchQueue;
	int runningWidth;
	rootNode->width=INF;
	

	searchQueue.push(rootNode);
	ActorNode* currNode = rootNode;
	
	while(!searchQueue.empty())
	{
		
		currNode = searchQueue.top();
		searchQueue.pop();
		
		if(!currNode->searched){
		
			setAdjVect(currNode, targetNode->actorName, "w");	
		
			currNode->searched= true;
			
			for(auto actor : currNode->adjActors){
				
				if(!actor->searched)
				{
					std::pair<Movie*, int> edge = calcWeightWidth(currNode, actor);	
					runningWidth = max(actor->width, min(currNode->width,edge.second ));
					
					if(runningWidth > actor->width){
						actor->width=runningWidth;
						actor->prevActor = currNode;
						actor->prevMovie = edge.first;
						searchQueue.push(actor);
					} 
					
				}
			}
				
		}
	}
	
	return rootNode;
/*

	priority_queue<ActorNode*, vector<ActorNode*>, compareNodes> searchQueue;
	int c;
	rootNode->dist=0;
	
	searchQueue.push(rootNode);
	ActorNode* currNode = rootNode;
	
	while(!searchQueue.empty())
	{
		
		currNode = searchQueue.top();
		searchQueue.pop();
		
		if(targetNode->actorName == currNode->actorName)
		{return currNode;}

		if(!currNode->searched){
		
		setAdjVect(currNode, targetNode->actorName, "w");	
		
			currNode->searched= true;
			
			for(auto actor : currNode->adjActors){
				
				if(!actor->searched)
				{
					std::pair<Movie*, int> edge = calcWeight(currNode, actor);
					c = min(currNode->width, edge.second);
					
					if(c < actor->dist){
						actor->prevActor = currNode;
						actor->prevMovie = edge.first;
						actor->dist = c;
						searchQueue.push(actor); 
					}
					
					
						
					
				}
			}
				
		}
	}
	return rootNode;
*/



}


ActorNode* ActorGraph::dijkSearch(ActorNode* rootNode, ActorNode* targetNode)
{
	priority_queue<ActorNode*, vector<ActorNode*>, compareNodes> searchQueue;
	int c;
	rootNode->dist=0;
	
	searchQueue.push(rootNode);
	ActorNode* currNode = rootNode;
	
	while(!searchQueue.empty())
	{
		
		currNode = searchQueue.top();
		searchQueue.pop();
		
		if(targetNode->actorName == currNode->actorName)
		{return currNode;}

		if(!currNode->searched){
		
		setAdjVect(currNode, targetNode->actorName, "w");	
		
			currNode->searched= true;
			
			for(auto actor : currNode->adjActors){
				
				if(!actor->searched)
				{
					std::pair<Movie*, int> edge = calcWeight(currNode, actor);
					c = currNode->dist + edge.second;
					
					if(c < actor->dist){
						actor->prevActor = currNode;
						actor->prevMovie = edge.first;
						actor->dist = c;
						searchQueue.push(actor); 
					}
					
					
						
					
				}
			}
				
		}
	}
	return rootNode;
}


vector<ActorNode*> ActorGraph::setAdjVect(ActorNode* workingActorNode, string actor2, string weightType){

	vector<ActorNode*> nodesToEnque;
	// Store the list of movies for the current actor
	vector<Movie*> movieList = workingActorNode->moviesIn;
	
	// Go through each movie
	for(unsigned int i = 0; i < movieList.size(); i++){
		
		// The current movie
		Movie* workingMovieNode = movieList.at(i);
		// Store the cast for that movie
		vector<ActorNode*> actorList = workingMovieNode->actorsInMovie;

		// Go through each actor in the cast
		for(unsigned int j = 0; j < actorList.size(); j++){
				
			// The current actor
			ActorNode* actor_i = actorList.at(j);
			
			// Check for shorter movie connection 	
			// create priority queue that sorts by year
			// get the shared movie with shortest year and contine

						// If this node hasn't been searched, continue
			if(actor_i->searched == false){
				
				if(weightType == "w"){
						
						workingActorNode->adjActors.push_back(actor_i);
						actor_i->checkingMovie = workingMovieNode;
						
						}

					}
				
				
				if(weightType == "u"){
					// Set the previous actor 
					if(actor_i->prevActor == nullptr){
						actor_i->prevActor = workingActorNode;
					}
					// Set the previous movie
					if(actor_i->prevMovie == nullptr){
						actor_i->prevMovie = workingMovieNode;				
					}

					// Add the actor to the working actor's adjacency list
					workingActorNode->adjActors.push_back(actor_i);
				
					// Found the actor	
					if(actor_i->actorName == actor2){
						workingActorNode->found = true;
						foundNode = actor_i;
						}
				}
			}
			
		}		
		
	
	return nodesToEnque;
}



// Update distance if less
bool ActorGraph::calcDist(int distToCurr, ActorNode* visitingNode, ActorNode* workingActorNode){
	std::pair<Movie*, int> recentMov_weight = calcWeight(workingActorNode, visitingNode);
	
	if(visitingNode->dist == INF){
		visitingNode->prevActor = workingActorNode;
		visitingNode->prevMovie = recentMov_weight.first;
		visitingNode->dist = distToCurr+recentMov_weight.second;
		return true;
		
	}
	if(visitingNode->dist < (distToCurr+recentMov_weight.second) ){
		return false;
	}
	visitingNode->prevActor = workingActorNode;
	visitingNode->prevMovie = recentMov_weight.first;
	visitingNode->dist = distToCurr+recentMov_weight.second;
	return true;
}



// Calculate edge weight
// - Finds most current movie that connects the two nodes
std::pair<Movie*, int> ActorGraph::calcWeight(ActorNode* refNode, ActorNode* visitingNode){
	
	vector<Movie*> refMovieList = refNode->moviesIn;
	vector<Movie*> visitingMovieList = visitingNode->moviesIn;

	priority_queue<Movie*, vector<Movie*>, movieCompare> sharedMovies;

	for(auto ref : refMovieList){
		for(auto vis : visitingMovieList){	
			if(vis == ref){
				sharedMovies.push(vis);
			}
		}
	}

	int weight = 1 + (CURR_YEAR - sharedMovies.top()->movieYear);
	
	std::pair<Movie*, int> recentMov_weight(sharedMovies.top(), weight);	
	
	return recentMov_weight;
}


std::pair<Movie*, int> ActorGraph::calcWeightWidth(ActorNode* refNode, ActorNode* visitingNode){
	
	vector<Movie*> refMovieList = refNode->moviesIn;
	vector<Movie*> visitingMovieList = visitingNode->moviesIn;

	priority_queue<Movie*, vector<Movie*>, movieCompareWidth> sharedMovies;

	for(auto ref : refMovieList){
		for(auto vis : visitingMovieList){	
			if(vis == ref){
				sharedMovies.push(vis);
			}
		}
	}

	int weight = 1 + (CURR_YEAR - sharedMovies.top()->movieYear);
	
	std::pair<Movie*, int> recentMov_weight(sharedMovies.top(), weight);	
	
	return recentMov_weight;
}



/*
vector<ActorNode*> ActorGraph::setAdjVect(ActorNode* workingActorNode, string actor2, string weightType){

	vector<ActorNode*> nodesToEnque;
	// Store the list of movies for the current actor
	vector<Movie*> movieList = workingActorNode->moviesIn;
	
	// Go through each movie
	for(unsigned int i = 0; i < movieList.size(); i++){
		
		// The current movie
		Movie* workingMovieNode = movieList.at(i);
		
		// Store the cast for that movie
		vector<ActorNode*> actorList = workingMovieNode->actorsInMovie;

		// Go through each actor in the cast
		for(unsigned int j = 0; j < actorList.size(); j++){
				
			// The current actor
			ActorNode* actor_i = actorList.at(j);
			
			// Check for shorter movie connection 	
			// create priority queue that sorts by year
			// get the shared movie with shortest year and contine

						// If this node hasn't been searched, continue
			if(actor_i->searched == false){
				
				if(weightType == "w"){
					bool updateDist = calcDist(workingActorNode->dist, actor_i, workingActorNode);
					if(updateDist){
						nodesToEnque.push_back(actor_i);
						// Found the actor	
						if(actor_i->actorName == actor2){
							workingActorNode->found = true;
							foundNode = actor_i;
							return nodesToEnque;
						}

					}
				}
				
				if(weightType == "u"){
					// Set the previous actor 
					if(actor_i->prevActor == nullptr){
						actor_i->prevActor = workingActorNode;
					}
					// Set the previous movie
					if(actor_i->prevMovie == nullptr){
						actor_i->prevMovie = workingMovieNode;				
					}

					// Add the actor to the working actor's adjacency list
					workingActorNode->adjActors.push_back(actor_i);
				
					// Found the actor	
					if(actor_i->actorName == actor2){
						workingActorNode->found = true;
						foundNode = actor_i;
					}
				}
			}
			
				
		}
	}
	return nodesToEnque;
}*/



//void ActorGraph::setWeightAdjPQ



bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;	

		runcount++;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
        	string next;
 

        	// get the next string before hitting a tab character and put it in 'next'
         	if (!getline( ss, next, '\t' )) break;

          	record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

				string titleAndYear = movie_title + "#@" + std::to_string(movie_year);
        
				// neither actor nor movie exist in the maps
				if(( actorMap.find(actor_name) == actorMap.end() ) && ( movieMap.find(titleAndYear) == movieMap.end() )){

					//create new movie node
					Movie* currMovie = new Movie(titleAndYear);
					if(runcount == 1){
						numMovies++;
						numActors++;
					}
					//create new actor node
					ActorNode* currActorNode = new ActorNode(actor_name);						
					
					//set the member variables for both nodes
					currMovie->movieYear = movie_year;
					currMovie->actorsInMovie.push_back(currActorNode);
					currActorNode->moviesIn.push_back(currMovie);

					//add both the movie and actor to their respective hashmaps
					actorMap.insert({actor_name, currActorNode});
					movieMap.insert({titleAndYear, currMovie});

				// movie exists, actor doesn't
				} else if(( actorMap.find(actor_name) == actorMap.end() ) && ( movieMap.find(titleAndYear) != movieMap.end() )){
					//create new actor node
					ActorNode* currActorNode = new ActorNode(actor_name);	
					if(runcount == 1){
						numActors++;
					}
					//locate the movie in the hashmap					
					unordered_map<string, Movie*>::const_iterator got = movieMap.find(titleAndYear);
	
					//add the movie node to the actor node's movie vector
					currActorNode->moviesIn.push_back(got->second);

					//add the actor node to the movie node's cast vector
					got->second->actorsInMovie.push_back(currActorNode);

					actorMap.insert({actor_name, currActorNode});


				// actor exists, movie doesn't
				} else if(( actorMap.find(actor_name) != actorMap.end() ) && ( movieMap.find(titleAndYear) == movieMap.end() )){
					
					// create new movie node
					Movie* currMovie = new Movie(titleAndYear);
					if(runcount == 1){
						numMovies++;
					}	
					// set the member variables for the new movie node
					currMovie->movieYear = movie_year;
					
					// locate the actor in the hashmap
					unordered_map<string, ActorNode*>::const_iterator got = actorMap.find(actor_name);
					
					// add the new movie node to the actor node's movie vector
					got->second->moviesIn.push_back(currMovie);
					
					// add the actor node to the movie node's cast vector
					currMovie->actorsInMovie.push_back(got->second);		
					
					movieMap.insert({titleAndYear, currMovie});			

				// both exist
				} else {
					
					// locate both nodes in their respective hashmaps
					unordered_map<string, ActorNode*>::const_iterator currActorNode = actorMap.find(actor_name);
					unordered_map<string, Movie*>::const_iterator currMovieNode = movieMap.find(titleAndYear);

					// add the actor to the movie and the movie to the actor
					currActorNode->second->moviesIn.push_back(currMovieNode->second);
					currMovieNode->second->actorsInMovie.push_back(currActorNode->second);

				}
    }

    if (!infile.eof()) {
    	cerr << "Failed to read " << in_filename << "!\n";
      return false;
		}
    infile.close();
    
		return true;
}
