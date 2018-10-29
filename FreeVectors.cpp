//
//  FreeVectors.cpp
//  FinalYearProject_Higher
//
//  Created by Constantin on 22/10/2018.
//  Copyright © 2018 Yijia.Zhao. All rights reserved.
//

#include "FreeVectors.hpp"

FreeVectors::FreeVectors()
{
    
}

void FreeVectors::freeStrVector(vector<string> *str) {
    str->clear();
    str->resize(0);
    str->shrink_to_fit();
    // delete pointer
    delete str;
    str = NULL;
}

void FreeVectors::freeVector(vector<tuple<char,int,char>> *vectors) {
    vectors->clear();
    vectors->resize(0);
    vectors->shrink_to_fit();
    //vector<pair<char,int>>().swap(*vectors);
    delete vectors;
    vectors = NULL;
}

void FreeVectors::freeTDVector(vector<vector<tuple<char,int,char>>> *vectors) {
    vectors->clear();
    vectors->resize(0);
    vectors->shrink_to_fit();
    delete vectors;
    vectors = NULL;
}
