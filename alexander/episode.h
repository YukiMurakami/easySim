//
//  episode.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#ifndef __easySimu__history__
#define __easySimu__history__

#include <iostream>
#include <map>

#include "person.h"
#include "place.h"

using namespace std;

class Episode {
public:
    map<string,Person> _persons;
    map<string,Place> _places;
    int _time;
    Episode(int time,map<string,Person> persons,map<string,Place> places);
    Episode();
};

vector<Episode> findEpisodeFromTime(int beginTime,int endTime,vector<Episode> episodes);
vector<Episode> getOnlyPersonEpisode(string name,vector<Episode> episodes);
vector<Episode> getFusionEpisode(vector <vector<Episode> > episodess);
void EpisodesOutput(vector<Episode> &episodes,string filename,string personName,double score,int maxDepth);

bool isSameEpisodesAlexander(vector<Episode> &a,vector<Episode> &b);
#endif /* defined(__easySimu__history__) */
