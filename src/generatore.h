#ifndef GENERATORE_H
#define GENERATORE_H

#include <type_generatore_casuale.h>
#include <Qt>
#include <coordinata.h>
#include <iostream>
#include <math.h>

using namespace std;

void generate_x(int from, int to, double rimasti, vector<coordinata>& x){
    int middle;

    if (rimasti==0) return;
    if (from==to) return;

    middle= ((to-from)/2)+from;
    //std::cout<<middle+1<<std::endl;
    x.push_back(coordinata(middle+1,0));

    generate_x(from,middle,floor(rimasti/2),x);
    generate_x(middle+1,to,floor(rimasti/2),x);

}

// ritorna il file dove leggere l'input
QString genera_nodi_problema(generatore_casuale generatore, int num_nodi){
    vector<coordinata> input_data;

    generate_x(0,num_nodi,num_nodi,input_data);





    return convert_generatore_to_string(generatore);
}




#endif // GENERATORE_H
