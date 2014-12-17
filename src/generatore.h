#ifndef GENERATORE_H
#define GENERATORE_H

#include <type_generatore_casuale.h>
#include <Qt>
#include <coordinata.h>
#include <iostream>
#include <math.h>
#include <data.h>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

void generate_x(int from, int to, double rimasti, vector<coordinata>& x){
    int middle;
    middle= ((to-from)/2)+from;

    if (rimasti==0) return;
    if (rimasti==1){
        std::cout<<from<<"      "<<to<<"         "<<middle<<"      "<<rimasti<<std::endl;
        x.push_back(coordinata(middle,0));
        return;
    }
    if (from==to){
        return;
    }

    generate_x(from,middle,ceil((rimasti)/2),x);
    generate_x(middle+1,to,floor((rimasti)/2),x);

}

// ritorna il file dove leggere l'input
QString genera_nodi_problema(generatore_casuale generatore, int num_nodi, int dispersione){
    //generate_x(0,num_nodi,num_nodi,input_data);
    vector<coordinata> input_data;

    generate_x(1,dispersione,num_nodi,input_data);
    //std::cout<<input_data.size();
    if (input_data.size()!=num_nodi)
        throw message_error("numero di x generate sbagliato");

    QString path= data::path_file;
    path.append(convert_generatore_to_string(generatore));

    ofstream file;
    file.open(path.toLatin1().data());

    if (file.is_open()==false)
        throw message_error("problemi nella scrittura del file di input");

    switch (generatore) {
    case cerchio:{
        // cerchio di centro (dispersione/2,0) e raggio dispersione/2
        // y^2=ax+c  con a = -dispersione e c= 0
        // y= +- sqrt(dispersione*x)
        coordinata pt;
        double x,y;
        int segno=-1;
        for (std::vector<coordinata>::iterator it = input_data.begin() ; it != input_data.end(); ++it){
           pt=*it;
           x=coordinata(pt).get_x();
           y=segno*sqrt(dispersione*x);
           segno=segno*(-1);
           file << x <<","<<y<<"\n";
           //pt=coordinata(coordinata(*it).get_x(),)
        }
        break;
    }
    case randoms:{
        srand(time(NULL));

        coordinata pt;
        double x,y;
        for (std::vector<coordinata>::iterator it = input_data.begin() ; it != input_data.end(); ++it){
           pt=*it;
           x=coordinata(pt).get_x();
           y=rand() % dispersione + 1;;
           file << x <<","<<y<<"\n";
           //pt=coordinata(coordinata(*it).get_x(),)
        }
        break;
    }

    }

    file.close();

    return convert_generatore_to_string(generatore);
}



#endif // GENERATORE_H
