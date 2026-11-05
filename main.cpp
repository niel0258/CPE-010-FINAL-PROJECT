#include <iostream>
#include <cstdlib>
#include "graph.h"

//create map
void genMap(Graph& mapLayout){
    //entrance = 0, phar = 1, er = 2, reg = 3,cas = 4,cas = 5, wa = 6,lab = 7,cli = 8,xray = 9,icu=10,or = 11
    mapLayout.add_edge(0, 1,  25);
    mapLayout.add_edge(0, 2, 20);
    mapLayout.add_edge(0, 3, 15);
    mapLayout.add_edge(1, 4, 10);
    mapLayout.add_edge(2,9,30);
    mapLayout.add_edge(3, 6, 10);
    mapLayout.add_edge(6, 7,20);
    mapLayout.add_edge(8, 7, 18);
    mapLayout.add_edge(6, 8, 15);
    mapLayout.add_edge(9, 10, 15);
    mapLayout.add_edge(10, 11, 10);

}

int main(){
    Graph HospitalLayout;
    genMap(HospitalLayout);

    return 0;
}