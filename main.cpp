//
//  main.cpp
//  FinalYearProject
//
//  Created by Yijia Zhao on 18/09/2018.
//  Copyright © 2018 Yijia Zhao. All rights reserved.
//

#include "InputValidation.hpp"
#include "FreeVectors.hpp"

using namespace std;

int main(void)
{
    vector<vector<tuple<char,int,char>>> *combine_codes = new vector<vector<tuple<char,int,char>>>();
    InputValidation IV;
    FreeVectors FV;
    
    //string codes = "h1+v1-o1-u2+,h2+u1+v2+o2+";
    //getCombCodes(combine_codes, codes);
    //second_Validation(codes);
    
    string codes = "";
    
    bool all_satisfy = true;
    bool b_valid = false;
    cout << "Codes are consisting of types, indexs and signs of orientations of points." << endl;
    cout << "Please use comma to seperate different circles of point." << endl;
    cout << "Type: h(Horizontal), v(Vertical), o(Overcorssing), u(Undercrossing)" << endl; // d(Disjoint)
    //cout << "Signs of orientation for disjoint links + and - means clockwise and anticlockwise seperately" << endl;
    cout << "The code cannot end with comma which hint that input process might not be completed." << endl;
    cout << "e.g: h1+v1-o1-u2+,h2+u1-v2+o2+" << endl;
    cout << "Please input your codes:" << endl;
    
    while(!b_valid || !all_satisfy) {
        if (all_satisfy == false)
            cout << "Please check and input codes again" << endl;
        
        cin >> codes;
        
        if ((b_valid = IV.basic_Validation(codes))) {
            if ((all_satisfy = IV.third_Validation(codes))) {
                cout << "Input codes passed all checks, your code is: ";
                IV.getCombCodes(combine_codes, codes);
            }
        }
    }
    
    for (vector<vector<tuple<char,int,char>>>::iterator it = combine_codes->begin();it != combine_codes->end(); ++it) {
        for (vector<tuple<char,int,char>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
            cout << get<0>(*it_tp) << get<1>(*it_tp) << get<2>(*it_tp);
        }
        cout << "   ";
    }
    
    FV.freeTDVector(combine_codes);
    
    return 0;
}
