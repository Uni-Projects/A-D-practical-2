/*
 * Paolo Scattolin
 * Johan Urban
 */
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

struct Actor {
    string name;
    map<int, string> moviesMap;
};

struct Actress {
    string name;
    vector<string> starredmovies;
};

vector<Actress> actresses;
vector<Actor> actors;

//this function returns true if actress and actor have costarred in some movie, false otherwise
//every movie the actress is in is checked on every actor hash-map to see if they costarred
bool costarred (const Actress& actress , const Actor& actor){
    hash<string> hash;
    for (const string& m : actress.starredmovies){
        auto it = actor.moviesMap.find(hash(m));
        if ( it != actor.moviesMap.end())
            return true;
    }
    return false;
}

bool bpm(vector<vector<bool>>& bpGraph, int u, vector<bool>& seen, vector<int>& matchR){
    // Try every actor one by one
    for (int v = 0; v < bpGraph.size(); v++){
        // If actress u has worked with actor v and v is not been already seen
        if (bpGraph[u][v] && !seen[v]){
            // Mark v as seen
            seen[v] = true;
            // If actor 'v' is not already matched to an
            // actresses OR previously seen, actress for actor v (which is matchR[v])
            // has an alternate actor available. ???
            // Since v is marked as visited in
            // the above line, matchR[v] in the following
            // recursive call will not get actor 'v' again
            if (matchR[v] < 0 || bpm(bpGraph, matchR[v], seen, matchR)){
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}
// Returns maximum number of matching from actresses to actors:
int maxBPM(vector<vector<bool>>& bpGraph){

    //matchR[i] is the actress number assigned to actor i
    vector<int> matchR;
    matchR.reserve(bpGraph.size());
    //initialise the vector
    for (int i = 0 ; i < bpGraph.size(); i++)
        matchR.push_back(-1);  // the value -1 indicates nobody is assigned.

    // Count of actors assigned to actresses
    int result = 0;
    for (int u = 0; u < bpGraph.size(); u++){
        // Mark all actors as unseen for the next actress.
        vector<bool> seen;
        seen.reserve(bpGraph.size());
        for (int i = 0 ; i < bpGraph.size(); i++)
            seen.push_back(false);

        // Find if the actresses 'u' can get a actor
        if (bpm(bpGraph, u, seen, matchR))
            result++;
    }
    return result;
}

int main()
{
    int total_actors, total_movies;
    string total;
    getline(cin,total);
    stringstream (total) >> total_actors >> total_movies;

    auto start = high_resolution_clock::now();
    string name;
    hash<string> hash; //this function hashes the string
    // get actress names
    for(int i = 0; i < total_actors; i++) {
        getline(cin,name);
        Actress actress = {name};
        actresses.push_back(actress);
    }

    // get actor names
    for (int i = 0; i < total_actors; i++) {
        getline(cin,name);
        Actor actor = {name};
        actors.push_back(actor);
    }

    // set up movies/actors relationship
    for(int i = 0; i < total_movies; i++) {
        // get movie name
        string title;
        getline(cin,title);
        // get cast size
        int cast_size;
        string cast_size_string;
        getline(cin,cast_size_string);
        cast_size = stoi(cast_size_string);
        // populate actors hash-map and actresses movies
        for (int j = 0; j < cast_size ; j++) {
            getline(cin,name);
            for (int k = 0; k < total_actors; k++) {
                if(actresses[k].name == name)
                    actresses[k].starredmovies.push_back(title);
                if(actors[k].name == name) {
                    actors[k].moviesMap.insert(pair<int,string>(hash(title),title));
                }
            }
        }
    }
    /*
    for(const auto& actress : actresses) {
        cout << actress.name << endl;
    }
    for (const auto& actor : actors){
        cout << actor.name << endl;
    }
    */

    //timestamp
    auto stop1 = high_resolution_clock::now();
    //cout << "Time taken to get the input: " <<  duration_cast<milliseconds>(stop1 - start).count() << endl;

    // creation of the bipartite graph as a adjacency matrix made with vectors (crazy)
    vector<vector<bool>> bpGraph;
    for(const auto& actress : actresses) {
        vector<bool> temp;
        temp.reserve(actors.size());
        for (const auto& actor : actors){
            temp.push_back(costarred(actress, actor));
        }
        bpGraph.push_back(temp);
    }

    //timestamp before algorithm
    auto stop2 = high_resolution_clock::now();
    //cout << "Time taken to fill the graph: " <<  duration_cast<milliseconds>(stop2 - stop1).count() << endl;
    int res = maxBPM(bpGraph);

    //timestamp after algorithm
    auto stop3 = high_resolution_clock::now();
    //cout << "Time taken to run the algorithm: " <<  duration_cast<milliseconds>(stop3 - stop2).count() << endl;

    //output
    if(res == total_actors)
        cout <<"Mark";
    else
        cout <<"Veronique";
    return 0;
}
//useful links:
//https://cs.stackexchange.com/questions/118508/winning-strategy-for-a-given-game-on-graphs
//http://discrete.openmathbooks.org/dmoi3/sec_matchings.html
//https://www.geeksforgeeks.org/maximum-bipartite-matching/ this is the current implementation!!
//https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-1-introduction/ this is better but harder i think
//https://www.geeksforgeeks.org/hopcroft-karp-algorithm-for-maximum-matching-set-2-implementation/