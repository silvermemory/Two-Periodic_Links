//
//  FreeVectors.hpp
//  FinalYearProject_Higher
//
//  Created by Constantin on 22/10/2018.
//  Copyright © 2018 Yijia.Zhao. All rights reserved.
//

#ifndef FreeVectors_hpp
#define FreeVectors_hpp

#include <vector>
#include <string>
#include <tuple>

using namespace std;

class FreeVectors
{
public:
    FreeVectors();
    void freeVector(vector<tuple<char,int,char>> *vectors);
    void freeStrVector(vector<string> *str);
    void freeTDVector(vector<vector<tuple<char,int,bool>>> *vectors);
    void freeVector_Bool(vector<tuple<char,int,bool>> *vectors);
};

#endif /* FreeVectors_hpp */
