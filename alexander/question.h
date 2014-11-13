//
//  question.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/30.
//
//

#ifndef __easySimu__question__
#define __easySimu__question__

#include "constraints.h"
#include "episode.h"

#include <iostream>
#include <vector>

using namespace std;

class Question : public Constraint {
public:
    double getAnswerFromEpisodess(const vector< vector<Episode> > &episodess);
    double getAnswerFromEpisodesFile(string filename);
};

#endif /* defined(__easySimu__question__) */
