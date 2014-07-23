//
//  utility.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#ifndef __easySimu__utility__
#define __easySimu__utility__

#include <iostream>
#include <map>

#include "person.h"
#include "place.h"
#include "episode.h"
#include "constraints.h"
#include "question.h"

using namespace std;

//文字列の区切り
vector<string> SpritString(const string &src,const string &delim);
//状態の表示
void show(int time,map<string,Place> places);

//アクションの処理(全てのエージェント分）
void doAction(map<string,Person> &persons,map<string,Place> &places);
void doActionUCB(map<string,Person> &persons,map<string,Place> &places,int nowTime,int endTime,vector<Constraint> &constraints,vector<Episode> &episodes);
//木を作成しながらシミュレーションを繰り返す
void doActionMCTS(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,vector< vector<string> > &actionList);

//移動アクションの処理
void move(Person &person,map<string,Person> &persons,map<string,Place> &places,Place &nextplace);

//エージェントの初期化
void initAgent(map<string,Person> &persons,map<string,Place> &places,string filename);
//エピソードの評価
double checkEpisode(vector<Episode> episodes,vector<Constraint> constraints);
double checkEpisodePerson(vector<Episode> episodes,vector<Constraint> constraints,string _personName);
//制約条件の初期化
void initConstraints(vector<Constraint> &constraints,string filename);
//問題の初期化
void initQuestion(Question &question,string filename);
//問題の評価
bool checkQuestion(vector<Episode> episodes,Question question);
//エピソードの表示
void showEpisode(vector<Episode> episodes);
//エージェントの表示
void showPersons(map<string,Person> &persons);

//UDB1値の算出
double calcUcb1(double sumVal,int n,int N,double c);


//モンテカルロ木ノード
typedef struct mctnode {
    string action;
    double sumValue;
    int count;
    double ucb;
} MCTNode;

typedef struct mctree {
    string action;
    int count;
    double sumVal;
    double ucb;
    map<string,Place> places;
    map<string,Person> persons;
    vector<struct mctree*> childs;
    struct mctree *parent;
    vector<Episode> episodes;
    int time;
    int *sumCount;
} MCTREE;

MCTNode makeNode(string _action,int _sum,int _count,double _ucb);
MCTREE* makeTree(map<string,Person> &_persons,map<string,Place> &_places,vector<Episode> _nowEpisodes,MCTREE* _parent,int nowTime,int *sumCount,string action);
void showTree(MCTREE *root);
void showTreeSub(MCTREE *current,int depth) ;


//おなじエージェントが複数存在したり、エージェントの位置情報とplaceのエージェント情報の一致を確認する
void assertEpisode(vector<Episode> episodes);

#endif /* defined(__easySimu__utility__) */
