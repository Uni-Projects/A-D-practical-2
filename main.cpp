#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum Status {USED, NOT_USED};

struct Actor {
    string name;
    Status availability;
};

struct Movie {
    string title;
    vector<Actor *> cast; // vector of pointers to type Actor in vectors actresses and actors
};

vector<Actor> actresses;
vector<Actor> actors;
vector<Movie> movies;

int main() {
    int total_actors, total_movies;
    cin >> total_actors >> total_movies;
    string name;
    // get actress names
    for(int i = 0; i < total_actors; i++) {
        cin >> name;
        Actor actress = {name, NOT_USED};
        actresses.push_back(actress);
    }
    // get actor names
    for (int i = 0; i < total_actors; i++) {
        cin >> name;
        Actor actor = {name, NOT_USED};
        actors.push_back(actor);
    }

    for(int i = 0; i < total_movies; i++) {
        // get movie name
        string title;
        cin >> title;

        // instance of struct movie + set title
        Movie movie;
        movie.title = title;
        // get cast size
        int cast_size;
        cin >> cast_size;
        // get cast + update cast vector in struct movie
        for (int j = 0; j < cast_size ; ++j) {
            cin >> name;
            for (int k = 0; k < actresses.size(); k++) {
                if(actresses[k].name == name)
                    movie.cast.push_back(&actresses[k]);
                else if(actors[k].name == name) {
                    movie.cast.push_back(&actors[k]);
                }
            }
        }
        // movies contains all the created instances of movie
        movies.push_back(movie);
    }

    // parse and modifications test
    for(int i = 0; i < movies.size(); i++) {
        cout << movies[i].title << ":" << endl;
        for (int j = 0; j < movies[i].cast.size(); ++j) {
            cout << (*movies[i].cast[j]).name << " " << (*movies[i].cast[j]).availability << endl;
        }
        cout << endl;
    }

    actresses[0].availability = USED;

    for(int i = 0; i < movies.size(); i++) {
        cout << movies[i].title << ":" << endl;
        for (int j = 0; j < movies[i].cast.size(); ++j) {
            cout << (*movies[i].cast[j]).name << " " << (*movies[i].cast[j]).availability << endl;
        }
        cout << endl;
    }

}