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
    vector<vector<tuple<char,int,bool>>> *combine_codes = new vector<vector<tuple<char,int,bool>>>();
    InputValidation IV;
    FreeVectors FV;
    
    string codes = "";
    
    bool all_satisfy = true;
    bool b_valid = false;
    cout << "Codes are consisting of types, indexs and signs of orientations of points." << endl;
    cout << "Please use comma to seperate different circles of point." << endl;
    cout << "Type: h(Horizontal), v(Vertical), o(Overcorssing), u(Undercrossing)" << endl;
    cout << "e.g: h1+v1-o1-u2+,h2+u1-v2+o2+" << endl;
    cout << "Please input your codes:" << endl;
    
    while(!b_valid || !all_satisfy) {
        if (all_satisfy == false)
            cout << "Please check and input codes again" << endl;
        
        getline(cin,codes);
        if (codes.empty() == false)
            codes = IV.checkSpace(codes);
        
        if ((b_valid = IV.basic_Validation(codes))) {
            if ((all_satisfy = IV.second_Validation(codes))) {
                cout << "Input codes passed all checks, your code is: ";
                IV.getCombCodes(combine_codes, codes);
            }
        }
    }
    
    for (vector<vector<tuple<char,int,bool>>>::iterator it = combine_codes->begin();it != combine_codes->end(); ++it) {
        if (it->empty() == false) {
            for (vector<tuple<char,int,bool>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
                cout << get<0>(*it_tp) << get<1>(*it_tp) << get<2>(*it_tp);
            }
            cout << endl;
        } else {
            cout << "," << endl;
        }
    }
    
    FV.freeTDVector(combine_codes);
	system("pause");
    return 0;
}
