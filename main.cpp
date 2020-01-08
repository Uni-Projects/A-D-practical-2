/*
 * Paolo Scattolin
 * Johan Urban
 */
#include <vector>
#include <queue>
#include <bits/stdc++.h>
#include <string>
#include <map>

using namespace std;

#define MAX 1000000
#define NIL 0
#define INF 999999

vector<int> graph[MAX];
int n, match[MAX], dist[MAX];

map<int, int> actorsMap;
map<int, int> actressesMap;

bool bfs() {
    int i, u, v, len;
    queue<int> Q;
    for (i = 1; i <= n; i++) {
        if (match[i] == NIL) {
            dist[i] = 0;
            Q.push(i);
        } else dist[i] = INF;
    }
    dist[NIL] = INF;
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();
        if (u != NIL) {
            len = graph[u].size();
            for (i = 0; i < len; i++) {
                v = graph[u][i];
                if (dist[match[v]] == INF) {
                    dist[match[v]] = dist[u] + 1;
                    Q.push(match[v]);
                }
            }
        }
    }
    return (dist[NIL] != INF);
}

bool dfs(int u) {
    int i, v, len;
    if (u != NIL) {
        len = graph[u].size();
        for (i = 0; i < len; i++) {
            v = graph[u][i];
            if (dist[match[v]] == dist[u] + 1) {
                if (dfs(match[v])) {
                    match[v] = u;
                    match[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INF;
        return false;
    }
    return true;
}

int hopcroft_karp() {
    int matching = 0, i;
    // match[] is assumed NIL for all vertex in graph
    while (bfs())
        for (i = 1; i <= n; i++)
            if (match[i] == NIL && dfs(i))
                matching++;
    return matching;
}

int main() {
    int total_actors, total_movies;
    hash<string> hash; //this function hashes the string
    string total;
    getline(cin, total);
    stringstream(total) >> total_actors >> total_movies;
    n = total_actors;

    // get actress names
    string name;
    for (int i = 0; i < total_actors; i++) {
        getline(cin, name);
        actressesMap.insert(pair<int, int>(hash(name), i));
    }

    // get actor names
    for (int i = 0; i < total_actors; i++) {
        getline(cin, name);
        actorsMap.insert(pair<int, int>(hash(name), i));
    }

    vector<int> actorTemp;
    vector<int> actressTemp;

    // set up movies/actors relationship
    string title;
    int cast_size;
    string cast_size_string;
    for (int i = 0; i < total_movies; i++) {
        // get movie name
        getline(cin, title);
        // get cast size
        getline(cin, cast_size_string);
        cast_size = stoi(cast_size_string);
        // populate actors hash-map and actresses movies
        for (int j = 0; j < cast_size; j++) {
            getline(cin, name);
            auto actressIt = actressesMap.find(hash(name));
            if (actressIt != actressesMap.end()) {
                actressTemp.push_back(actressesMap[hash(name)]);
            } else {
                actorTemp.push_back(actorsMap[hash(name)]);
            }
        }
        //creation of the graph
        for (const auto &actress : actressTemp) {
            for (const auto &actor : actorTemp) {
                graph[actress + 1].push_back(actor + total_actors + 1);
            }
        }
        actorTemp.clear();
        actressTemp.clear();
    }

    if (hopcroft_karp() == total_actors)
        cout << "Mark";
    else
        cout << "Veronique";

    return 0;
}