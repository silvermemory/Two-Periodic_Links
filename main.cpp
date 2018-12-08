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
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,vertices> Graph;
typedef boost::graph_traits<Graph>::vertex_iterator Vertice_Iter;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertice_Descrip;
typedef boost::property_map<Graph, boost::vertex_name_t>::type Vertice_Type;
typedef std::pair<Vertice_Iter, Vertice_Iter> Vertice_Iter_Pair;
typedef boost::graph_traits<Graph>::edge_iterator Edge_Iter;
typedef std::pair<std::tuple<char,int,bool>, std::tuple<char,int,bool>> Edge_Pair;

//void addEdges(Vertice_Descrip *first, Vertice_Descrip *second, Vertice_Type &format, Graph *g) {
//    typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
//    std::pair<vertex_iter, vertex_iter> vp;
//    std::pair<vertex_iter, vertex_iter> vp2;
//    for (vp = boost::vertices(*g); vp.first != vp.second; ++vp.first) {
//        if (format[*vp.first] == format[*first]) {
//            for (vp2 = boost::vertices(*g); vp2.first != vp2.second; ++vp2.first) {
//                if (format[*vp2.first] == format[*second]) {
//                    boost::add_edge(*vp.first, *vp2.first,*g);
//                    break;
//                }
//            }
//            break;
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
    std::sort(hor_vertice->begin(),hor_vertice->end(),sortByIndex);
    std::sort(ver_vertice->begin(),ver_vertice->end(),sortByIndex);
    std::sort(cross_vertice->begin(),cross_vertice->end(),sortByIndex);
    
    int num_vertices = (int)hor_vertice->size() + (int)ver_vertice->size() + (int)cross_vertice->size() + 1;
    std::cout << num_vertices << " vertices" << std::endl;
    
    Vertice_Descrip *vertice_collection = new Vertice_Descrip[num_vertices];
    Vertice_Type format = get(boost::vertex_name, g);
    
    int position = 0;
    // add vertices into the graph
    *vertice_collection = boost::add_vertex(corner_vertice,g);
    position++;
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = hor_vertice->begin(); it != hor_vertice->end(); ++it) {
        *(vertice_collection + position) = boost::add_vertex(*it,g);
        position++;
    }
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = ver_vertice->begin(); it != ver_vertice->end(); ++it) {
        *(vertice_collection + position) = boost::add_vertex(*it,g);
        position++;
    }
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = cross_vertice->begin(); it != cross_vertice->end(); ++it) {
        *(vertice_collection + position) = boost::add_vertex(*it, g);
        position++;
    }
    
    // check whether vertices are added into the graph
    Vertice_Iter v,vend;
    for (boost::tie(v, vend) = boost::vertices(g); v != vend; ++v) {
        std::cout << get<0>(format[*v]) << get<1>(format[*v]) << get<2>(format[*v]) << " ";
    }
    std::cout << "\n\n";
    
    Vertice_Iter_Pair vp;
    Vertice_Iter_Pair vp2;
    // add horizontal edges to the graph and build the horizontal edge set
    Edge_Pair *horizontal_set = new Edge_Pair[hor_vertice->size()+1];
    position = 0;
    int prefix_pos = 1;
    
    std::cout << "Number of edges in the horizontal edge set:" << hor_vertice->size() + 1 << std::endl;
    if (hor_vertice->size() > 0) {
        for (int i = 0; i < hor_vertice->size(); ++i) {
            if (i != hor_vertice->size()-1) {
                *(horizontal_set + i) = std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*(vertice_collection+prefix_pos + i + 1)]);
                for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                    if (format[*vp.first] == format[*(vertice_collection + prefix_pos + i)]) {
                        for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                            if (format[*vp2.first] == format[*(vertice_collection + prefix_pos + i + 1)]) {
                                boost::add_edge(*vp.first, *vp2.first,g);
                                break;
                            }
                        }
                        break;
                    }
                }
//                addEdges((vertice_collection + prefix_pos + i), (vertice_collection+prefix_pos + i + 1), format, &g);
            } else {
                *(horizontal_set + i) = std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*vertice_collection]);
                for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                    if (format[*vp.first] == format[*(vertice_collection + prefix_pos + i)]) {
                        for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                            if (format[*vp2.first] == format[*vertice_collection]) {
                                boost::add_edge(*vp.first, *vp2.first,g);
                                break;
                            }
                        }
                        break;
                    }
                }
    //            addEdges((vertice_collection + prefix_pos + i), (vertice_collection)], format, &g);
                *(horizontal_set + i + 1) = std::make_pair(format[*vertice_collection], format[*(vertice_collection + prefix_pos)]);
                for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                    if (format[*vp.first] == format[*vertice_collection]) {
                        for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                            if (format[*vp2.first] == format[*(vertice_collection + prefix_pos)]) {
                                boost::add_edge(*vp.first, *vp2.first,g);
                                break;
                            }
                        }
                        break;
                    }
                }
    //            addEdges((vertice_collection), (vertice_collection + prefix_pos), format, &g);
            }
        }
    } else if (hor_vertice->size() == 0) {
        *(horizontal_set + 0) = std::make_pair(format[*vertice_collection], format[*vertice_collection]);
        for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
            if (format[*vp.first] == format[*vertice_collection]) {
                boost::add_edge(*vp.first, *vp.first, g);
                break;
            }
        }
//            addEdges(vertice_collection, vertice_collection, format, &g);
    }
    
    for (int i = 0; i < hor_vertice->size() + 1; ++i) {
        std::cout << "(" << get<0>(std::get<0>(*(horizontal_set + i))) << get<1>(std::get<0>(*(horizontal_set + i))) << get<2>(std::get<0>(*(horizontal_set + i))) << "->";
        std::cout << get<0>(std::get<1>(*(horizontal_set + i))) << get<1>(std::get<1>(*(horizontal_set + i))) << get<2>(std::get<1>(*(horizontal_set + i))) << ") ";
    }
    std::cout << "\n";
    
    // add vertical edges to the graph and build the vertical edge set
    Edge_Pair *vertical_set = new Edge_Pair[ver_vertice->size()+1];
    prefix_pos = (int)hor_vertice->size() + 1;
    
    std::cout << "Number of edges in the vertical edge set:" << ver_vertice->size() + 1 << std::endl;
    if (ver_vertice->size() > 0) {
        for (int i = 0; i < ver_vertice->size(); ++i) {
            if (i != ver_vertice->size()-1) {
                *(vertical_set + i) = std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*(vertice_collection + prefix_pos+i + 1)]);
                for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                    if (format[*vp.first] == format[*(vertice_collection + prefix_pos + i)]) {
                        for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                            if (format[*vp2.first] == format[*(vertice_collection + prefix_pos+i + 1)]) {
                                boost::add_edge(*vp.first, *vp2.first,g);
                                break;
                            }
                        }
                        break;
                    }
                }
    //            addEdges((vertice_collection + prefix_pos + i), (vertice_collection + prefix_pos+i + 1), format, &g);
            } else {
                *(vertical_set + i) = std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*vertice_collection]);
                for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                    if (format[*vp.first] == format[*(vertice_collection + prefix_pos+i)]) {
                        for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                            if (format[*vp2.first] == format[*vertice_collection]) {
                                boost::add_edge(*vp.first, *vp2.first,g);
                                break;
                            }
                        }
                        break;
                    }
                }
    //            addEdges((vertice_collection + prefix_pos + i), (vertice_collection), format, *g);
                *(vertical_set + i + 1) = std::make_pair(format[*vertice_collection], format[*(vertice_collection + prefix_pos)]);
                for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                    if (format[*vp.first] == format[*vertice_collection]) {
                        for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                            if (format[*vp2.first] == format[*(vertice_collection + prefix_pos)]) {
                                boost::add_edge(*vp.first, *vp2.first,g);
                                break;
                            }
                        }
                        break;
                    }
                }
    //            addEdges(vertice_collection, (vertice_collection + prefix_pos), format, &g);
            }
        }
    } else if (ver_vertice->size() == 0) {
        *(vertical_set + 0) = std::make_pair(format[*vertice_collection], format[*vertice_collection]);
        for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
            if (format[*vp.first] == format[*vertice_collection]) {
                boost::add_edge(*vp.first, *vp.first, g);
                break;
            }
        }
//            addEdges(vertice_collection, vertice_collection, format, &g);
    }
    
    for (int i = 0; i < ver_vertice->size() + 1; ++i) {
        std::cout << "(" << get<0>(std::get<0>(*(vertical_set + i))) << get<1>(std::get<0>(*(vertical_set + i))) << get<2>(std::get<0>(*(vertical_set + i))) << "->";
        std::cout << get<0>(std::get<1>(*(vertical_set + i))) << get<1>(std::get<1>(*(vertical_set + i))) << get<2>(std::get<1>(*(vertical_set + i))) << ") ";
    }
    std::cout << "\n";
    
    // add component edges to the graph and build the vertical edge set
    int num_comp_edges = 0;
    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin(); it != combine_codes->end(); ++it) {
        num_comp_edges += it->size();
    }
    std::cout << "Number of edges in the component edge set:" << num_comp_edges << std::endl;
    
    Edge_Pair *comp_set_origin = new Edge_Pair[num_comp_edges];
    Edge_Pair *comp_set_against = new Edge_Pair[num_comp_edges];
    position = 0;
    int prefix_pos_v = (int)hor_vertice->size();
    int prefix_pos_c = (int)hor_vertice->size() + (int)ver_vertice->size();
    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin(); it != combine_codes->end(); ++it) {
        if (!it->empty()) {
            for (std::vector<std::tuple<char,int,bool>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
                int first_pos = 0, second_pos = 0;
                if (it_tp != it->end() - 1) {
                    // get the vertice position in the vertice set
                    if (get<0>(*it_tp) == 'h')
                        first_pos = get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'v')
                        first_pos = prefix_pos_v + get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'u' || get<0>(*it_tp) == 'o')
                        first_pos = prefix_pos_c + get<1>(*it_tp);

                    if (get<0>(*(it_tp + 1)) == 'h')
                        second_pos = get<1>(*(it_tp + 1));
                    else if (get<0>(*(it_tp + 1)) == 'v')
                        second_pos = prefix_pos_v + get<1>(*(it_tp + 1));
                    else if (get<0>(*(it_tp + 1)) == 'u' || get<0>(*(it_tp + 1)) == 'o')
                        second_pos = prefix_pos_c + get<1>(*(it_tp + 1));

                    *(comp_set_origin + position) = std::make_pair(format[*(vertice_collection + first_pos)], format[*(vertice_collection + second_pos)]);
                    *(comp_set_against + position) = std::make_pair(format[*(vertice_collection + second_pos)], format[*(vertice_collection + first_pos)]);
                    position++;
                    for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                        if (format[*vp.first] == format[*(vertice_collection + first_pos)]) {
                            for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                                if (format[*vp2.first] == format[*(vertice_collection + second_pos)]) {
                                    boost::add_edge(*vp.first, *vp2.first,g);
                                    break;
                                }
                            }
                            break;
                        }
                    }
//                    addEdges((vertice_collection + first_pos), (vertice_collection + second_pos), format, &g);
                } else {
                    if (get<0>(*it_tp) == 'h')
                        first_pos = get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'v')
                        first_pos = prefix_pos_v + get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'u' || get<0>(*it_tp) == 'o')
                        first_pos = prefix_pos_c + get<1>(*it_tp);

                    if (get<0>(*(it_tp - (it->size() - 1))) == 'h')
                        second_pos = get<1>(*(it_tp - (it->size() - 1)));
                    else if (get<0>(*(it_tp - (it->size() - 1))) == 'v')
                        second_pos = prefix_pos_v + get<1>(*(it_tp - (it->size() - 1)));
                    else if (get<0>(*(it_tp - (it->size() - 1))) == 'u' || get<0>(*(it_tp - (it->size() - 1))) == 'o')
                        second_pos = prefix_pos_c + get<1>(*(it_tp - (it->size() - 1)));
                    *(comp_set_origin + position) = std::make_pair(format[*(vertice_collection + first_pos)], format[*(vertice_collection + second_pos)]);
                    *(comp_set_against + position) = std::make_pair(format[*(vertice_collection + second_pos)], format[*(vertice_collection + first_pos)]);
                    position++;
                    for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
                        if (format[*vp.first] == format[*(vertice_collection + first_pos)]) {
                            for (vp2 = boost::vertices(g); vp2.first != vp2.second; ++vp2.first) {
                                if (format[*vp2.first] == format[*(vertice_collection + second_pos)]) {
                                    boost::add_edge(*vp.first, *vp2.first,g);
                                    break;
                                }
                            }
                            break;
                        }
                    }
//                    addEdges((vertice_collection + first_pos), (vertice_collection + second_pos), format, &g);
                }
            }
        }
    }
    for (int i = 0; i < num_comp_edges; ++i) {
        std::cout << "(" << get<0>(std::get<0>(*(comp_set_origin + i))) << get<1>(std::get<0>(*(comp_set_origin + i))) << get<2>(std::get<0>(*(comp_set_origin + i))) << "->";
        std::cout << get<0>(std::get<1>(*(comp_set_origin + i))) << get<1>(std::get<1>(*(comp_set_origin + i))) << get<2>(std::get<1>(*(comp_set_origin + i))) << ") ";
    }
    std::cout << "\n";
    
    Edge_Iter  ei, ei_end ;
    std::cout << "Edge in g  = [";
    for(boost::tie(ei, ei_end)=boost::edges(g); ei != ei_end; ++ei) {
        std::cout<<" ("<< get<0>(format[boost::source(*ei, g)]) << get<1>(format[boost::source(*ei,g)]) << get<2>(format[boost::source(*ei,g)]) << "->";
        std::cout << get<0>(format[boost::target(*ei, g)]) << get<1>(format[boost::target(*ei,g)]) << get<2>(format[boost::target(*ei,g)]) <<") ";
    }
    std::cout<<"]"<<std::endl;
    
    int total_edges = num_comp_edges + (int)(ver_vertice->size() + 1) + (int)(hor_vertice->size() + 1);
    std::cout << "Total number of edges in the graph: " << total_edges << std::endl;
    
    FV.freeVector_Bool(hor_vertice);
    FV.freeVector_Bool(ver_vertice);
    FV.freeVector_Bool(cross_vertice);
    FV.freeTDVector(combine_codes);
    
    // delete pointers
    delete[] vertice_collection;
    vertice_collection = NULL;
    delete[] horizontal_set;
    horizontal_set = NULL;
    delete[] vertical_set;
    vertical_set = NULL;
    delete[] comp_set_origin;
    comp_set_origin = NULL;
    delete[] comp_set_against;
    comp_set_against = NULL;
    
    return 0;
}
