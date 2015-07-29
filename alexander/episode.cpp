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

#include "utility.h"

Episode::Episode(int time,map<string,Person> persons, map<string,Place> places) {
    _persons = persons;
    _places = places;
    _time = time;
}

Episode::Episode() {}


vector<Episode> findEpisodeFromTime(int beginTime,int endTime,vector<Episode> episodes) {
    vector<Episode> episode;
    bool flag = false;
    int size = (int)episodes.size();
    for(int i=0;i<size;i++) {
        if(flag && episodes[i]._time > endTime) {
            break;
        }
        if(episodes[i]._time >= beginTime && episodes[i]._time <= endTime) {
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

void EpisodesOutput(vector<Episode> &episodes,string filename,string personName,double score,int maxDepth) {
  //  cout << "output episode : '" << filename << "'" << endl;
    ofstream ofs(filename.c_str(),ios::app );
    

    ofs << "newPlayout ----------------------------------------------" << endl;
    for (int i=0; i<(int)episodes.size(); i++) {
        int time = episodes[i]._time;
        Person person = episodes[i]._persons[personName];
        ofs << time << " " << personName << " " << person._nowPlace << endl;
    }
    ofs << "score:" << score << " maxDepth:" << maxDepth << endl;
    ofs << "playoutEnd ----------------------------------------------" << endl;
    ofs.close();
}

vector< pair< vector<Episode>,vector<Constraint> > > readEpisodesAndConstraintsFromEpisodeFile(string filename,string initPlaceFilename) {
    map<string,Place> places;
    
    ifstream ifs2(initPlaceFilename.c_str());
    string buf;
    if(!ifs2) {
        cout << "not found initialize file" << endl;
        exit(0);
    }
    vector<string> placeVector;
    while(getline(ifs2,buf)) {
        if(buf == "<area_start>") {
            while(getline(ifs2,buf)) {
                if(buf == "<area_end>") break;
                vector<string> out2 = SpritString(buf, ":");
                int placeIndex = atoi(out2[0].c_str());
                vector<string> out3 = SpritString(out2[1], ",");
                if(places.find(placeVector[placeIndex]) != places.end()) {
                    for(int i=0;i<(int)out3.size();i++) {
                        int nextIndex = atoi(out3[i].c_str());
                        if(places.find(placeVector[nextIndex]) != places.end()) {
                            places[placeVector[placeIndex]]._nextPlaces.push_back(placeVector[nextIndex]);
                        }
                    }
                }
            }
        }
        if(buf == "<area_end>") {
            continue;
        }
        
        vector<string> out = SpritString(buf, ":");
        if(out[0] == "place") {
            placeVector = SpritString(out[1],",");
            for(int i=0;i<(int)placeVector.size();i++) {
                Place place;
                place._name = placeVector[i];
                places.insert(make_pair(place._name, place));
            }
        }
    }
    ifs2.close();
    
    
    
    ifstream ifs(filename.c_str());
    if(!ifs) {
        cout << "error: not found file '" << filename << "' @readEpisodesAndConstraintsFromEpisodeFile" << endl;
        exit(0);
    }

    vector< pair< vector<Episode>,vector<Constraint> > > episodes;
    
    bool isSelectedConstraintsSection = false;
    
    while (getline(ifs,buf)) {
        vector<string> out = SpritString(buf, " ");
        if(out[0] == "newPlayout") {
            vector<Episode> episode;
            vector<Constraint> constraints;
            episodes.push_back(make_pair(episode, constraints));
            
        } else if(out[0] == "playoutEnd") {
            
        } else if(out[0] == "score") {
            
        } else if(out[0] == "selectedConstraintsStart") {
            isSelectedConstraintsSection = true;
        } else if(out[0] == "selectedConstraintsEnd") {
            isSelectedConstraintsSection = false;
        } else if(out.size() >= 3) {
            int time = atoi(out[0].c_str());
            string personName = out[1];
            string placeName = "";
            for(unsigned int i=0;i<out.size() - 2;i++) {
                placeName += out[i+2];
                if(i != out.size()-3) {
                    placeName += " ";
                }
            }
            
            Person person(personName,placeName,0);
            map<string,Person> persons;
            persons.insert(make_pair(personName, person));
            
            map<string,Place> copyPlaces(places);
            copyPlaces[placeName]._persons.push_back(person);
            Episode epi(time,persons,copyPlaces);
            
            episodes[episodes.size()-1].first.push_back(epi);
            
        } else if(isSelectedConstraintsSection) {
            vector<string> out2 = SpritString(buf, ":");
            if(out2[0] == "constraint") {
                int beginTime = atoi(out2[1].c_str());
                int endTime = atoi(out2[2].c_str());
                string personName = out2[3];
                string placeName = out2[4];
                CONSTRAINT con = getEnumFromString(out2[5]);
                int id = atoi(out2[6].c_str());
                
                Constraint constraint(beginTime,endTime,personName,placeName,con,id);
                episodes[episodes.size()-1].second.push_back(constraint);
            }
        }
    }
    
    return episodes;
}

bool isSameEpisodesAlexander(vector<Episode> &a,vector<Episode> &b) {
    if(a.size() != b.size()) return false;
    
    for(unsigned int i=0;i<a.size();i++) {
        if(a[i]._persons["Alexander"]._nowPlace != b[i]._persons["Alexander"]._nowPlace) {
            return false;
        }
    }
    return true;
}
