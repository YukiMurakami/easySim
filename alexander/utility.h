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

#include <time.h>

#include "person.h"
#include "place.h"
#include "episode.h"
#include "constraints.h"
#include "question.h"


using namespace std;

//文字列の区切り
vector<string> SpritString(const string &src,const string &delim);
//状態の表示
void show(int time,map<string,Place> places);//全てを表示
void showOnlyPerson(int time,map<string,Place> places);//人がいるところのみ表示する

//アクションの処理(全てのエージェント分）
//ランダムな探索
int randomAction(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints) ;
//全探索
void allSearchAction(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints);
void subAllSearch(map<string,Person> persons,map<string,Place> places,vector<Constraint> &constraints,vector<Episode>episodes,bool &isFinish,string action,Person person,int step,vector<Episode> &answerEpisodes);
void doAction(map<string,Person> &persons,map<string,Place> &places);
void doActionUCB(map<string,Person> &persons,map<string,Place> &places,int nowTime,int endTime,vector<Constraint> &constraints,vector<Episode> &episodes);
//木を作成しながらシミュレーションを繰り返す
//総プレイアウト回数を返す
int doActionMCTS(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,int endTime);

//移動アクションの処理
void move(Person &person,map<string,Person> &persons,map<string,Place> &places,Place &nextplace);

//エージェントの初期化
void initAgent(map<string,Person> &persons,map<string,Place> &places,string filename);
//エピソードの評価
double checkEpisode(vector<Episode> episodes,vector<Constraint> constraints);
double checkEpisodePerson(vector<Episode> episodes,vector<Constraint> constraints,string _personName);
double checkEpisodePersonWithArray(vector<Episode> *episodes,vector<Constraint> constraints,string _personName,Episode *episodesArray);
//制約条件の初期化
void initConstraints(vector<Constraint> &constraints,string filename);
//設定された制約条件からランダムにcount個選んだ物を制約条件として設定する
void initConstraintsWithCount(vector<Constraint> &constraints,string filename,int count);
//問題の初期化
void initQuestion(Question &question,string filename);
//問題の評価
bool checkQuestion(vector<Episode> episodes,Question question);
//エピソードの表示
void showEpisode(vector<Episode> episodes);
void showEpisodeWithPerson(vector<Episode> episodes);
//エージェントの表示
void showPersons(map<string,Person> &persons);
//BC換算の時間表示
void showBC(int time);

//UDB1値の算出
double calcUcb1(double sumVal,int n,int N,double c);

//時代換算関数  {紀元前年、月} 例{332,6}
vector<int> getBCfromTime(int time);


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

    vector<struct mctree*> childs;
    struct mctree *parent;
    Episode episode;
    int *sumCount;
} MCTREE;

MCTNode makeNode(string _action,int _sum,int _count,double _ucb);
MCTREE* makeTree(Episode _episode,MCTREE* _parent,int nowTime,int *sumCount,string action);
void showTree(MCTREE *root);
void showTreeSub(MCTREE *current,int depth) ;
void makeEpisodesFromTree(MCTREE *rootNode , MCTREE *leafNode,vector<Episode> &episodes);


//おなじエージェントが複数存在したり、エージェントの位置情報とplaceのエージェント情報の一致を確認する
void assertEpisode(const vector<Episode> &episodes);

#endif /* defined(__easySimu__utility__) */
