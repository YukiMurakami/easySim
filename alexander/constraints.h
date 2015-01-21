//
//  constraints.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/25.
//
//

#ifndef __easySimu__constraints__
#define __easySimu__constraints__

#include <iostream>
#include <string>
#include <vector>


using namespace std;

enum CONSTRAINT {
    none,
    there_is,
    there_is_no,
};

class Constraint {
public:
    int _beginTime;
    int _endTime;
    string _personName;
    string _placeName;
    CONSTRAINT _constraint;
    
    int _id;
    
    Constraint(int beginTime,int endTime,string person,string place,CONSTRAINT constraint);
    Constraint(int beginTime,int endTime,string person,string place,CONSTRAINT constraint,int id);
    Constraint();
    void show();
    string getString();
};

CONSTRAINT getEnumFromString(string constraint);
string getStringFromEnum(CONSTRAINT constraint);

bool isSameConstraint(Constraint a,Constraint b);

//isSameModeはtrue　で　全く同じ制約条件を一つと見なすモードになる　falseで通常モード
vector<Constraint> makeConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places,bool isSameMode);
vector<Constraint> makeConstraintsFromTestfileWithError(string filename,vector<string> &persons,vector<string> &places,double rate,bool isSameMode);

//条件にあわないもの、情報がそろってないものも制約条件として生成する
vector<Constraint> makeAllConstraintsFromTestfile(string filename,vector<string> &persons,vector<string> &places);
void showConstraints(vector<Constraint> &constraints);

vector<Constraint> selectRandomConstraintsWithRate(vector<Constraint> constraints,double rate);
vector<Constraint> removeConstraintsWithPlaceName(vector<Constraint> constraints,vector<string> removePlaceName);

double getCorrectRateWithConstraintAndEpisodeFile(Constraint con,string episodeFileName);

class Coreference {
public:
    string _title;
    Constraint _constraint;
    vector<string> _candidates;
    string _answer;
    
    Coreference(string title,Constraint constraint,vector<string> candidates);
    void show();
};

vector<Coreference> makeCoreferencesFromTextfile(string filename);

void solveCoreference(Coreference coreference,string episodeFileName);

vector<Constraint> makeConstraintsFromConstraintFile(string filename,int beginLine,int endLine);


class FourChoiceQuestion {
public:
    vector<Constraint> _constraints;
    int _answerIndex;//0 - 3
    
    void show();
};

FourChoiceQuestion makeFourChoiceQuestionByCorrectConstraintAndAnnotationConstraints(Constraint correctConstriant,vector<Constraint> annotationConstraints,vector<string> placeVector);

vector<FourChoiceQuestion> makeFourChoiceQuestionsWithCount(int count,vector<Constraint> annotationConstraints,vector<string> placeVector);

void outputFourChoiceQuestions(vector<FourChoiceQuestion> questions,string filename);
vector<FourChoiceQuestion> readFourChoiceQuestions(string filename);
void solveFourChoiceQuestions(string prefix,vector<FourChoiceQuestion> questions,double rate,vector<Constraint> annotationConstraints) ;
#endif /* defined(__easySimu__constraints__) */
