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
void doActionWithHeuristics(map<string,Person> &persons,map<string,Place> &places);
void doActionWithHeuristicsAndShortest(map<string,Person> &persons,map<string,Place> &places,map<string, map<string,int> > &shortestInfo,vector<Constraint> constraints,int nowTime);
void doActionUCB(map<string,Person> &persons,map<string,Place> &places,int nowTime,int endTime,vector<Constraint> &constraints,vector<Episode> &episodes);
//木を作成しながらシミュレーションを繰り返す
//総プレイアウト回数を返す
int doActionMCTS(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,int endTime,string outputFilename,double Cp);

//プレイアウト回数で打ち切るまでまわし、そのなかで報酬値が高かったものトップ１０のエピソードを出力する
int doActionMCTSWithRank(map<string,Person> &persons,map<string,Place> &places,vector<Constraint> &constraints,int endTime,string outputFilename, double Cp,map<string, map<string,int> > &shortestInfo);

//移動アクションの処理
void move(Person &person,map<string,Person> &persons,map<string,Place> &places,Place &nextplace);

//エージェントの初期化
void initAgent(map<string,Person> &persons,map<string,Place> &places,string filename);
void initPlace(map<string,Place> &places,string filename);
//エピソードの評価
bool isEpisodeSatisfyWithConstraint(vector<Episode> &episode,Constraint constraint);
double checkEpisode(vector<Episode> episodes,vector<Constraint> constraints);
double checkEpisodePerson(vector<Episode> episodes,vector<Constraint> constraints,string _personName ,bool isShowConstraints);

void checkEpisodePersonAndOutput(vector<Episode> episodes,vector<Constraint> constraints,string _personName,string filename,bool isShowConstraints);

//生成された制約条件とアノテーションデータとの比較
void checkConstraintGenerator(string testFilename,vector<Constraint> &constraints);
void showDifferentConstraints(vector<Constraint> &a,vector<Constraint> &c);
void showDifferentConstraintsWithoutId(vector<Constraint> a,vector<Constraint> c);
void showAndOutputUnknownPlaceFromAnnotation(string filename,vector<string> &persons,vector<string> &places,string outputFileName);
//制約条件の初期化
void initConstraints(vector<Constraint> &constraints,string filename);
void initConstraintsFromAnnotation(vector<Constraint> &constraints,string filename,vector<string> &persons,vector<string> &places,bool isSameMode);
//設定された制約条件からランダムにcount個選んだ物を制約条件として設定する
void initConstraintsWithCount(vector<Constraint> &constraints,string filename,int count);
//問題の初期化
void initQuestion(Question &question,string filename);
//問題の評価
bool checkQuestion(vector<Episode> episodes,Question question);
//エピソードの表示
void showEpisode(vector<Episode> episodes);
void showEpisodeWithPerson(vector<Episode> episodes);
double checkEpisodePersonWithActions(vector<string> actions,vector<Constraint> &constraints,string _personName,string firstPlace,bool isShowConstraints);
//エージェントの表示
void showPersons(map<string,Person> &persons);
//制約条件の地名分布を表示
void showConstraintsPlaceDistribution(vector<Constraint> &constraints);
//BC換算の時間表示
void showBC(int time);
//プレイアウト回数毎の平均報酬値をファイルに書き出す
void outputAverageReward(string filename,int playout,double reward);

//UDB1値の算出
double calcUcb1(double sumVal,int n,int N,double c);

//xorshift乱数生成
uint32_t xor128(void) ;
double getRandomZeroToOne();

//時代換算関数  {紀元前年、月} 例{332,6}
vector<int> getBCfromTime(int time);
int getTimeFromBC(int bc,int month);
int getMonthFromString(string mString);

//大文字小文字の区別なく文字列の比較
bool isEqualStringWithoutCapital(string a,string b);
//表記揺れを除去する（リストに該当する項目は、最も使われていると思われる表記に変換する）
string removeOrthographicalVariantString(string &a) ;
//表記揺れを考慮した文字列の比較
bool isEqualStringWithoutOrthographicalVariant(string &a,string &b);

//Alexandria of the S -> A S
//A in S -> A S
string processingOfAlexandria(string &a);

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
double checkEpisodePersonWithArrayWithTree(MCTREE *root,MCTREE *leaf, vector<Constraint> constraints,string _personName,Episode *episodesArray,bool isShowConstraints) ;
void deleteTree(MCTREE *root);

//おなじエージェントが複数存在したり、エージェントの位置情報とplaceのエージェント情報の一致を確認する
void assertEpisode(const vector<Episode> &episodes);

//テキスト操作
string extractStringFromTextfileWithLine(string filename,int line);
vector< vector<string> > extractAroundStringsFromTextfileWithString(string filename,string buf,int window);
void outputCoreferenceTestFileFromUnknownPlaces(string outputFileName,string unknownPlaceFilename);
void outputCoreferenceTestFileFromWikipedia(string outputFileName,vector<Constraint> &constraints,string wikipediaFilename);
void showErrorConstraintsFromEpisodeFiles(vector<string> episodeFileNames);

map<string, map<string,int> > getShortestLengthToNextConstraint(map<string,Place> &places) ;
void enqueue(vector< vector<string> > &queue,vector<string> &data);
vector<string> dequeue(vector< vector<string> > &queue);

//ファイル操作
void makeDirectory(string path,string folderName) ;
vector<string> getFilenamesByFoldername(string folderName);
vector<string> getFilenamesByFilelistname(string listName);

#endif /* defined(__easySimu__utility__) */
