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
typedef boost::property_map<Graph, boost::vertex_name_t>::type Vertice_Ref;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;

//void addEdges(std::tuple<char,int,bool> first, std::tuple<char,int,bool> second, Vertice_Ref format, Graph g) {
//    typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
//    std::pair<vertex_iter, vertex_iter> vp;
//    std::pair<vertex_iter, vertex_iter> vp2;
//    for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//        if (format[*vp.first] == first) {
//            for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
//                if (format[*vp2.first] == second) {
//                    boost::add_edge(*vp.first, *vp2.first,g);
//                }
//            }
//
//        }
//    }
//}

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
    IV.getCombCodes(combine_codes, codes);
    
    // sort vertices by their index in ascending order.
    sort(hor_vertice->begin(),hor_vertice->end(),sortByIndex);
    sort(ver_vertice->begin(),ver_vertice->end(),sortByIndex);
    std::sort(cross_vertice->begin(),cross_vertice->end(),sortByIndex);
    
    int num_vertices = (int)hor_vertice->size() + (int)ver_vertice->size() + (int)cross_vertice->size() + 1;
    std::cout << num_vertices << " vertices" << std::endl;
//    int num_hor_edges = (int)hor_vertice->size() + 1;
//    int num_ver_edges = (int)ver_vertice->size() + 1;
    
    Graph::vertex_descriptor vertice_collection[num_vertices];
    Vertice_Ref format = get(boost::vertex_name, g);
    
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
    
    // check whether vertices are added into the graph
    Graph::vertex_iterator v,vend;
    for (boost::tie(v, vend) = boost::vertices(g); v != vend; ++v) {
        std::cout << get<0>(format[*v]) << get<1>(format[*v]) << get<2>(format[*v]) << " ";
    }
    std::cout << "\n\n";
    
    std::pair<vertex_iter, vertex_iter> vp;
    std::pair<vertex_iter, vertex_iter> vp2;
    // add horizontal edges to the graph and build the horizontal edge set
    Edge horizontal_set[hor_vertice->size()+1];
    position = 0;
    int prefix_pos = 1;
    for (int i = 0; i < hor_vertice->size(); ++i) {
        if (i != hor_vertice->size()-1) {
            horizontal_set[i] = std::make_pair(format[vertice_collection[prefix_pos+i]], format[vertice_collection[prefix_pos+i+1]]);
            std::cout << "(" << get<0>(format[vertice_collection[prefix_pos+i]]) << get<1>(format[vertice_collection[prefix_pos+i]]) << get<2>(format[vertice_collection[prefix_pos+i]]) << "->";
            std::cout << get<0>(format[vertice_collection[prefix_pos+i+1]]) << get<1>(format[vertice_collection[prefix_pos+i+1]]) << get<2>(format[vertice_collection[prefix_pos+i+1]]) << ") ";
            for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                if (format[*vp.first] == format[vertice_collection[prefix_pos+i]]) {
                    for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                        if (format[*vp2.first] == format[vertice_collection[prefix_pos+i+1]]) {
                            boost::add_edge(*vp.first, *vp2.first,g);
                        }
                    }
                    
                }
            }
//            addEdges(format[vertice_collection[prefix_pos+i]], format[vertice_collection[prefix_pos+i+1]], format, g);
        }
        else {
            horizontal_set[i] = std::make_pair(format[vertice_collection[prefix_pos+i]], format[vertice_collection[0]]);
            std::cout << "(" << get<0>(format[vertice_collection[prefix_pos+i]]) << get<1>(format[vertice_collection[prefix_pos+i]]) << get<2>(format[vertice_collection[prefix_pos+i]]) << "->";
            std::cout << get<0>(format[vertice_collection[0]]) << get<1>(format[vertice_collection[0]]) << get<2>(format[vertice_collection[0]]) << ") ";
            for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                if (format[*vp.first] == format[vertice_collection[prefix_pos+i]]) {
                    for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                        if (format[*vp2.first] == format[vertice_collection[0]]) {
                            boost::add_edge(*vp.first, *vp2.first,g);
                        }
                    }
                    
                }
            }
//            addEdges(format[vertice_collection[prefix_pos+i]], format[vertice_collection[0]], format, g);
            horizontal_set[i+1] = std::make_pair(format[vertice_collection[0]], format[vertice_collection[prefix_pos]]);
            std::cout << "(" << get<0>(format[vertice_collection[0]]) << get<1>(format[vertice_collection[0]]) << get<2>(format[vertice_collection[0]]) << "->";
            std::cout << get<0>(format[vertice_collection[prefix_pos]]) << get<1>(format[vertice_collection[prefix_pos]]) << get<2>(format[vertice_collection[prefix_pos]]) << ") ";
            for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                if (format[*vp.first] == format[vertice_collection[0]]) {
                    for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                        if (format[*vp2.first] == format[vertice_collection[prefix_pos]]) {
                            boost::add_edge(*vp.first, *vp2.first,g);
                        }
                    }
                }
            }
//            addEdges(format[vertice_collection[0]], format[vertice_collection[prefix_pos]], format, g);
        }
    }
    std::cout << "Number of edges in the horizontal edge set:" << hor_vertice->size() + 1 << std::endl;
    
    // add vertical edges to the graph and build the vertical edge set
//    Edge vertical_set[ver_vertice->size()+1];
//    prefix_pos = (int)hor_vertice->size() + 1;
//    for (int i = 0; i < ver_vertice->size(); ++i) {
//        if (i != ver_vertice->size()-1) {
//            vertical_set[i] = std::make_pair(format[vertice_collection[prefix_pos+i]], format[vertice_collection[prefix_pos+i+1]]);
//            std::cout << "(" << get<0>(format[vertice_collection[prefix_pos+i]]) << get<1>(format[vertice_collection[prefix_pos+i]]) << get<2>(format[vertice_collection[prefix_pos+i]]) << "->";
//            std::cout << get<0>(format[vertice_collection[prefix_pos+i+1]]) << get<1>(format[vertice_collection[prefix_pos+i+1]]) << get<2>(format[vertice_collection[prefix_pos+i+1]]) << ") ";
//
//            for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//                if (format[*vp.first] == format[vertice_collection[prefix_pos+i]]) {
//                    for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
//                        if (format[*vp2.first] == format[vertice_collection[prefix_pos+i+1]]) {
//                            boost::add_edge(*vp.first, *vp2.first,g);
//                        }
//                    }
//
//                }
//            }
////            addEdges(format[vertice_collection[prefix_pos+i]], format[vertice_collection[prefix_pos+i+1]], format, g);
//        }
//        else {
//            vertical_set[i] = std::make_pair(format[vertice_collection[prefix_pos+i]], format[vertice_collection[0]]);
//            std::cout << "(" << get<0>(format[vertice_collection[prefix_pos+i]]) << get<1>(format[vertice_collection[prefix_pos+i]]) << get<2>(format[vertice_collection[prefix_pos+i]]) << "->";
//            std::cout << get<0>(format[vertice_collection[0]]) << get<1>(format[vertice_collection[0]]) << get<2>(format[vertice_collection[0]]) << ") ";
//            for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//                if (format[*vp.first] == format[vertice_collection[prefix_pos+i]]) {
//                    for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
//                        if (format[*vp2.first] == format[vertice_collection[0]]) {
//                            boost::add_edge(*vp.first, *vp2.first,g);
//                        }
//                    }
//
//                }
//            }
////            addEdges(format[vertice_collection[prefix_pos+i]], format[vertice_collection[0]], format, g);
//            vertical_set[i+1] = std::make_pair(format[vertice_collection[0]], format[vertice_collection[prefix_pos]]);
//            std::cout << get<0>(format[vertice_collection[0]]) << get<1>(format[vertice_collection[0]]) << get<2>(format[vertice_collection[0]]) << "->";
//            std::cout << "(" << get<0>(format[vertice_collection[prefix_pos]]) << get<1>(format[vertice_collection[prefix_pos]]) << get<2>(format[vertice_collection[prefix_pos]]) << ") ";
//            for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//                if (format[*vp.first] == format[vertice_collection[0]]) {
//                    for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
//                        if (format[*vp2.first] == format[vertice_collection[prefix_pos]]) {
//                            boost::add_edge(*vp.first, *vp2.first,g);
//                        }
//                    }
//
//                }
//            }
////            addEdges(format[vertice_collection[0]], format[vertice_collection[prefix_pos]], format, g);
//        }
//    }
//    std::cout << "Number of edges in the vertical edge set:" << ver_vertice->size() + 1 << std::endl;
//
//    // add component edges to the graph and build the vertical edge set
//    int num_comp_edges = 0;
//    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin(); it != combine_codes->end(); ++it) {
//        num_comp_edges += it->size();
//    }
//    std::cout << "Number of edges in the component edge set:" << num_comp_edges << std::endl;
//
//    Edge comp_set_origin[num_comp_edges], comp_set_against[num_comp_edges];
//    position = 0;
//    int prefix_pos_v = (int)hor_vertice->size();
//    int prefix_pos_c = (int)hor_vertice->size() + (int)ver_vertice->size();
//    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin(); it != combine_codes->end(); ++it) {
//        if (!it->empty()) {
//            for (std::vector<std::tuple<char,int,bool>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
//                int first_pos = 0, second_pos = 0;
//                if (it_tp != it->end() - 1) {
//                    std::cout << "(" << get<0>(*it_tp) << get<1>(*it_tp) << get<2>(*it_tp) << "->";
//                    std::cout << get<0>(*(it_tp+1)) << get<1>(*(it_tp+1)) << get<2>(*(it_tp+1)) << ") ";
//                    // get the vertice position in the vertice set
//                    if (get<0>(*it_tp) == 'h')
//                        first_pos = get<1>(*it_tp);
//                    else if (get<0>(*it_tp) == 'v')
//                        first_pos = prefix_pos_v + get<1>(*it_tp);
//                    else if (get<0>(*it_tp) == 'u' || get<0>(*it_tp) == 'o')
//                        first_pos = prefix_pos_c + get<1>(*it_tp);
//
//                    if (get<0>(*(it_tp+1)) == 'h')
//                        second_pos = get<1>(*(it_tp+1));
//                    else if (get<0>(*(it_tp+1)) == 'v')
//                        second_pos = prefix_pos_v + get<1>(*(it_tp+1));
//                    else if (get<0>(*(it_tp+1)) == 'u' || get<0>(*(it_tp+1)) == 'o')
//                        second_pos = prefix_pos_c + get<1>(*(it_tp+1));
//
//                    comp_set_origin[position] = std::make_pair(format[vertice_collection[first_pos]], format[vertice_collection[second_pos]]);
//                    comp_set_against[position] = std::make_pair(format[vertice_collection[second_pos]], format[vertice_collection[first_pos]]);
//                    position++;
//                    for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//                        if (format[*vp.first] == format[vertice_collection[first_pos]]) {
//                            for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
//                                if (format[*vp2.first] == format[vertice_collection[second_pos]]) {
//                                    boost::add_edge(*vp.first, *vp2.first,g);
//                                }
//                            }
//
//                        }
//                    }
////                    addEdges(format[vertice_collection[first_pos]], format[vertice_collection[second_pos]], format, g);
//                } else {
//                    std::cout << "(" << get<0>(*it_tp) << get<1>(*it_tp) << get<2>(*it_tp) << "->";
//                    std::cout << get<0>(*(it_tp-(it->size()-1))) << get<1>(*(it_tp-(it->size()-1))) << get<2>(*(it_tp-(it->size()-1))) << ") " << std::endl;
//
//
//                    if (get<0>(*it_tp) == 'h')
//                        first_pos = get<1>(*it_tp);
//                    else if (get<0>(*it_tp) == 'v')
//                        first_pos = prefix_pos_v + get<1>(*it_tp);
//                    else if (get<0>(*it_tp) == 'u' || get<0>(*it_tp) == 'o')
//                        first_pos = prefix_pos_c + get<1>(*it_tp);
//
//                    if (get<0>(*(it_tp-(it->size()-1))) == 'h')
//                        second_pos = get<1>(*(it_tp-(it->size()-1)));
//                    else if (get<0>(*(it_tp-(it->size()-1))) == 'v')
//                        second_pos = prefix_pos_v + get<1>(*(it_tp-(it->size()-1)));
//                    else if (get<0>(*(it_tp-(it->size()-1))) == 'u' || get<0>(*(it_tp-(it->size()-1))) == 'o')
//                        second_pos = prefix_pos_c + get<1>(*(it_tp-(it->size()-1)));
//                    comp_set_origin[position] = std::make_pair(format[vertice_collection[first_pos]], format[vertice_collection[second_pos]]);
//                    comp_set_against[position] = std::make_pair(format[vertice_collection[second_pos]], format[vertice_collection[first_pos]]);
//                    position++;
//                    for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
//                        if (format[*vp.first] == format[vertice_collection[first_pos]]) {
//                            for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
//                                if (format[*vp2.first] == format[vertice_collection[second_pos]]) {
//                                    boost::add_edge(*vp.first, *vp2.first,g);
//                                }
//                            }
//
//                        }
//                    }
////                    addEdges(format[vertice_collection[first_pos]], format[vertice_collection[second_pos]], format, g);
//                }
//            }
//        }
//    }
    
    boost::graph_traits<Graph>::edge_iterator  ei, ei_end ;
    std::cout << "Edge in g  = [ ";
    for(boost::tie(ei, ei_end)=boost::edges(g); ei != ei_end; ++ei) {
        std::cout<<"("<< get<0>(format[boost::source(*ei, g)]) << get<1>(format[boost::source(*ei,g)]) << get<2>(format[boost::source(*ei,g)]) << " , ";
        std::cout << get<0>(format[boost::target(*ei, g)]) << get<1>(format[boost::source(*ei,g)]) << get<2>(format[boost::source(*ei,g)]) <<")"<< std::endl;
    }
    std::cout<<"]"<<std::endl;
    
//    int total_edges = num_comp_edges + (int)(ver_vertice->size()+1) + (int)(hor_vertice->size()+1);
//    std::cout << "Total number of edges in the graph: " << total_edges << std::endl;
    
    FV.freeVector_Bool(hor_vertice);
    FV.freeVector_Bool(ver_vertice);
    FV.freeVector_Bool(cross_vertice);
    FV.freeTDVector(combine_codes);
    return 0;
}
