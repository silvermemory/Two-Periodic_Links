//
//  main.cpp
//  FinalYearProject
//
//  Created by Yijia Zhao on 18/09/2018.
//  Copyright © 2018 Yijia Zhao. All rights reserved.
//

#include "InputValidation.hpp"
#include "FreeVectors.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

// vertex name property
typedef boost::property<boost::vertex_name_t, std::tuple<char,int,bool>> vertices;
// define edges between two vertices
typedef std::pair<std::tuple<char,int,bool>, std::tuple<char,int,bool>> Edge;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,vertices> Graph;
//typedef boost::property_map<Graph, std::tuple<char,int,bool>>::type Vertice_Index;

bool sortByIndex(std::tuple<char,int,bool> &a, std::tuple<char,int,bool> &b) {
    return (std::get<1>(a) < std::get<1>(b));
}

int main(void)
{
    std::vector<std::vector<std::tuple<char,int,bool>>> *combine_codes = new std::vector<std::vector<std::tuple<char,int,bool>>>();
    const std::tuple<char,int,bool> corner_vertice = std::make_tuple('c',0,true);
    std::vector<std::tuple<char,int,bool>> *hor_vertice = new std::vector<std::tuple<char,int,bool>>();
    std::vector<std::tuple<char,int,bool>> *ver_vertice = new std::vector<std::tuple<char,int,bool>>();
    std::vector<std::tuple<char,int,bool>> *cross_vertice = new std::vector<std::tuple<char,int,bool>>();
    InputValidation IV;
    FreeVectors FV;
    Graph g;

//    std::string codes = "";
//
//    bool all_satisfy = true;
//    bool b_valid = false;
//    std::cout << "Codes are consisting of types, indexs and signs of orientations of points." << std::endl;
//    std::cout << "Please use comma to seperate different circles of point." << std::endl;
//    std::cout << "Type: h(Horizontal), v(Vertical), o(Overcorssing), u(Undercrossing)" << std::endl;
//    std::cout << "e.g: h1+v1-o1-u2+,h2+u1-v2+o2+" << std::endl;
//    std::cout << "Please input your codes:" << std::endl;
//
//    while(!b_valid || !all_satisfy) {
//        if (all_satisfy == false)
//            std::cout << "Please check and input codes again" << std::endl;
//
//        std::getline(std::cin,codes);
//        boost::to_lower(codes);
//        if (codes.empty() == false)
//            codes = IV.checkSpace(codes);
//
//        if ((b_valid = IV.basic_Validation(codes))) {
//            if ((all_satisfy = IV.second_Validation(codes,hor_vertice,ver_vertice,cross_vertice))) {
//                std::cout << "Input codes passed all checks, your code is: ";
//                IV.getCombCodes(combine_codes, codes);
//            }
//        }
//    }
//
//    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin();it != combine_codes->end(); ++it) {
//        if (it->empty() == false) {
//            for (std::vector<std::tuple<char,int,bool>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
//                std::cout << std::get<0>(*it_tp) << std::get<1>(*it_tp) << std::get<2>(*it_tp);
//            }
//            std::cout << std::endl;
//        } else {
//            std::cout << "," << std::endl;
//        }
//    }
    
    std::string codes = "h1+v1-o2-u1+,H2+u2-V2+o1+";
    boost::to_lower(codes);
    IV.second_Validation(codes,hor_vertice,ver_vertice,cross_vertice);
    
    // sort vertices by their index in ascending order.
    sort(hor_vertice->begin(),hor_vertice->end(),sortByIndex);
    sort(ver_vertice->begin(),ver_vertice->end(),sortByIndex);
    std::sort(cross_vertice->begin(),cross_vertice->end(),sortByIndex);
    
    int num_vertices = (int)hor_vertice->size() + (int)ver_vertice->size() + (int)cross_vertice->size() + 1;
    std::cout << num_vertices << " vertices" << std::endl;
    Graph::vertex_descriptor vertice_collection[num_vertices];
    
    int position = 0;
    // add vertices into the graph
    vertice_collection[position] = boost::add_vertex(corner_vertice,g);
    position++;
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = hor_vertice->begin(); it != hor_vertice->end(); ++it) {
        vertice_collection[position] = boost::add_vertex(*it,g);
        position++;
    }
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = ver_vertice->begin(); it != ver_vertice->end(); ++it) {
        vertice_collection[position] = boost::add_vertex(*it,g);
        position++;
    }
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = cross_vertice->begin(); it != cross_vertice->end(); ++it) {
        vertice_collection[position] = boost::add_vertex(*it, g);
        position++;
    }
    
    int edge_number = 0;
    position = 0;
    
    
    
    FV.freeVector_Bool(hor_vertice);
    FV.freeVector_Bool(ver_vertice);
    FV.freeVector_Bool(cross_vertice);
    FV.freeTDVector(combine_codes);
    return 0;
}
