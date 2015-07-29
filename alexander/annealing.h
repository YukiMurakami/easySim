//
//  annealing.h
//  alexander
//
//  Created by Yuki Murakami on 2015/03/03.
//
//

#pragma once

#include <stdio.h>
#include <vector>
#include <string>
#include "episode.h"

using namespace std;

//より汎用的なメソッド
void makeAnnealingEpisodesWithFolder(string initializeFilename,int getTopCount,string inputFilePath,string outputFilePath,int *endIters,int nEndIters,int nRandomEpisode,int begin,int end);
void makeAnnealingEpisodeFileFromNormalEpisodeFileWithFolder(string episodeFilename, string initializeFilename,int getCount,string inputPath,string outputPath,int endIter,int nRandomEpisode);



void makeAnnealingEpisodes(string folderPath, string initializeFilename,int getCount);

void makeAnnealingEpisodeFileFromNormalEpisodeFile(string episodeFilename, string initializeFilename,int getCount,string folderPath,int endIter);

vector< vector<Episode> > getAnnealingEpisodes( vector<Episode> &episode ,vector<Constraint> &constraints,string initializeFilename,int getCount,double &bestScore,int endIter);

vector<string> getNeighbor(vector<string> actions,bool isDebug,string firstPlace,map<string,Place> &places,vector< vector<string> > &loops);
double getAnnealingProbability(double nowVal,double nextVal,int iter,int maxIter);

vector<string> makeRandomActions(string firstPlace,map<string,Place> &places);

vector<string> getActionsFromEpisodes(vector<Episode> &episodes);
vector<Episode> getEpisodesFromActionsWithAlexander(vector<string> &actions,string initializeFilename);


vector<string> addNothingActionByTime(vector<string> actions,int time,bool isDebug);
vector<string> removeNothingActionWithRandom(vector<string> actions,bool isDebug);
vector<string> removeNothingActionWithTimeWidth(vector<string> actions,bool isDebug,int start,int end);
vector<string> addDetourActionByTime(vector<string> actions,int time,bool isDebug,string firstPlace,map<string,Place> &places);
vector<string> removeDetourActionWithRandom(vector<string> actions,bool isDebug,string firstPlace,map<string,Place> &places);
vector<string> changeLoopActionWithRandom(vector<string> actions,bool isDebug,vector< vector<string> > &loops,string firstPlace) ;

void eraseActionByTime(vector<string> &actions,int time);
void insertActionByTime(vector<string> &actions,int time,string action);
bool checkActions(vector<string> &actions) ;

vector< vector<string> > getLoopFromPlaces(map<string,Place> &places);
void subGetLoopFromPlaces(vector< vector<string> > &loops, map<string,Place> &places, map<string,bool> placeVisited,string parentPlace,string currentPlace,vector<string> episode);