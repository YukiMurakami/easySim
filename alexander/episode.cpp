//
//  episode.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#include "episode.h"

#include <stdlib.h>

#include <fstream>

Episode::Episode(int time,map<string,Person> persons, map<string,Place> places) {
    _persons = persons;
    _places = places;
    _time = time;
}

Episode::Episode() {}


vector<Episode> findEpisodeFromTime(int time,vector<Episode> episodes) {
    vector<Episode> episode;
    bool flag = false;
    int size = (int)episodes.size();
    for(int i=0;i<size;i++) {
        if(flag && episodes[i]._time > time) {
            break;
        }
        if(episodes[i]._time == time) {
            episode.push_back( episodes[i] );
            flag = true;
        }
    }
    if(!flag) {
        cout << "error : not found such time episode @findEpisodeFromTime" << endl;
        exit(0);
    }
    return episode;
}

vector<Episode> getOnlyPersonEpisode(string name,vector<Episode> episodes) {
    vector<Episode> onlyEpisodes;
    
    for(int i=0;i<(int)episodes.size();i++) {
        Episode episode = episodes[i];
        map<string,Person> persons = episode._persons;
        map<string,Place> places = episode._places;
        for(map<string,Place>::iterator it = places.begin();it != places.end(); it++) {
            for(vector<Person>::iterator it2 = (*it).second._persons.begin();it2 != (*it).second._persons.end();) {
                if((*it2)._name != name) {
                    it2 = (*it).second._persons.erase(it2);
                } else {
                    ++it2;
                }
            }
        }
        for(map<string,Person>::iterator it = persons.begin();it != persons.end();) {
            Person person = (*it).second;
            if(person._name != name) {
                persons.erase(it++);
            } else {
                ++it;
            }
        }
        
        Episode newEpisode(episode._time,persons,places);
        onlyEpisodes.push_back(newEpisode);
    }
    
    
    return onlyEpisodes;
}

vector<Episode> getFusionEpisode(vector <vector<Episode> > episodess) {
    vector<Episode> fusionEpisodes;
    
    int nstep = (int)episodess[0].size();
    for(int i=0;i<nstep;i++) {
        Episode fusionEpisode;
        
        for(int ei=0;ei<(int)episodess.size();ei++) {
            vector<Episode> episodes = episodess[ei];
            Episode episode = episodes[i];
            for(map<string,Person>::iterator pit = episode._persons.begin();pit != episode._persons.end();pit++) {
                string name = (*pit).second._name;
                if(fusionEpisode._persons.find(name) == fusionEpisode._persons.end()) {
                    fusionEpisode._persons.insert(make_pair(name, (*pit).second));
                }
            }
            for(map<string,Place>::iterator pit = episode._places.begin();pit != episode._places.end();pit++) {
                string name = (*pit).second._name;
                if(fusionEpisode._places.find(name) == fusionEpisode._places.end()) {
                    fusionEpisode._places.insert(make_pair(name, (*pit).second));
                } else {
                    Place place = (*pit).second;
                   // cout << "i:" << i << " ei:" << ei << " place:" << place._name << " " << place._persons.size() << endl;
                    for(int pi=0;pi<(int)place._persons.size();pi++) {
                        int flag = -1;
                        string personName = place._persons[pi]._name;
                        for(int ppi=0;ppi<(int)fusionEpisode._places[name]._persons.size();ppi++) {
                            if(fusionEpisode._places[name]._persons[ppi]._name == personName) {
                                flag = ppi;
                                break;
                            }
                        }
                        if(flag == -1) {
                            fusionEpisode._places[name]._persons.push_back(place._persons[pi]);
                        }
                    }
                }
            }
        }
        fusionEpisode._time = episodess[0][i]._time;
        fusionEpisodes.push_back(fusionEpisode);
    }
    
    return fusionEpisodes;
}

void EpisodesOutput(vector<Episode> &episodes,string filename,string personName) {
 
    ofstream ofs(filename.c_str(),ios::app );


    ofs << "newPlayout ----------------------------------------------" << endl;
    for (int i=0; i<(int)episodes.size(); i++) {
        int time = episodes[i]._time;
        Person person = episodes[i]._persons[personName];
        ofs << time << " " << personName << " " << person._nowPlace << endl;
    }
    ofs.close();
}
