//
//  InputTextValidation.hpp
//  FinalYearProject_Higher
//
//  Created by Constantin on 22/10/2018.
//  Copyright © 2018 Yijia.Zhao. All rights reserved.
//

#ifndef InputValidation_hpp
#define InputValidation_hpp

#include <iostream>
#include <sstream>
#include <regex>

using namespace std;

class InputValidation
{
public:
    InputValidation();
    bool isRepeatPoint(vector<tuple<char,int,char>> *vct, int *index);
    bool basic_Validation(string codes);
    bool second_Validation(string codes);
    void getCombCodes(vector<vector<tuple<char,int,bool>>> *combine_codes,string codes);
    string checkSpace(string input);
};

#endif /* InputValidation_hpp */
