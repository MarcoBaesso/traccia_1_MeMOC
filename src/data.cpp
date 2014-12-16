#include "data.h"
#include <stdio.h>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <message_error.h>
#include <math.h>
#include <Qt>

using namespace std;

const QString data::path_file="data/";

double data::calcola_distanza(coordinata pt1,coordinata pt2){
    double l1,l2;
    l1=pt2.get_x()-pt1.get_x();
    l2=pt2.get_y()-pt1.get_y();
    // arrotondamento per eccesso
    //return ceil(sqrt(pow(l1,2)+pow(l2,2)));
    return sqrt(pow(l1,2)+pow(l2,2));
}

data::data(QString filename)
{
    QString relative_path;

    this->filename=filename;
    relative_path.append(path_file);
    relative_path.append(filename);
    //relative_path.append(extension);


    QFile file;
    file.setFileName(relative_path);
    bool opened_file=file.open(QIODevice::ReadOnly);

    if (opened_file==false)
        throw message_error("Il file di input non esiste");

    QByteArray dati=file.readAll();
    file.close();
    

    //std::cout<<dati.size()<<std::endl;

    int last_pos=0,last_virgola;
    QByteArray number;
    double x;
    double y;
    bool is_number;
    while (true){
        last_virgola=dati.indexOf(",",last_pos);
        if (last_virgola==-1)
            break;
        number=dati.mid(last_pos,last_virgola-last_pos);
        last_pos=last_virgola+1;
        x=number.toDouble(&is_number);

        //std::cout<<x<<std::endl;
        if (number.size()==0 || is_number==false)
            throw message_error("Inserire solo cifre numeriche");
        else{
            last_virgola=dati.indexOf("\n",last_pos);
            number=dati.mid(last_pos,last_virgola-last_pos);
            //char *input_data = number.data();
            //while (*input_data) {
            //    cout << *input_data;
            //    ++input_data;
            //}
            last_pos=last_virgola+1;
            y=number.toDouble(&is_number);
            //std::cout<<y<<std::endl;
            if (number.size()==0 || is_number==false)
                throw message_error("Inserire solo cifre numeriche");

            //std::cout<<x<<" "<<y<<std::endl;

            nodi.push_back(coordinata(x,y));
            

        }
    }

    if (nodi.size()==0)
        throw message_error("Dati inseriti vuoti o non nel formato coordinata _,_\n_,_....._,_ solo cifre numeriche");

    tempi.resize(nodi.size());
    for(int z=0;z<nodi.size();z++){
        tempi[z].resize(nodi.size()-1);
        //std::cout<<"calcolo distanza tra";
        for(int h=0;h<nodi.size()-1;h++){
            if (h<z){
                //std::cout<<nodi[h].get_x()<<nodi[h].get_y()<<std::endl;
                tempi[z][h]=calcola_distanza(nodi[z],nodi[h]);
            }
            else{
                //std::cout<<nodi[h+1].get_x()<<nodi[h+1].get_y()<<std::endl;
                tempi[z][h]=calcola_distanza(nodi[z],nodi[h+1]);
            }
            //std::cout<<tempi[z][h]<<std::endl;
        }
    }

}

vector<vector<double > > data::get_tempi() const{
    return this->tempi;
}

vector<coordinata> data::get_nodi() const{
    return this->nodi;
}

QString data::get_file_name() const{
    return this->filename;
}

int data::get_num_nodi() const{
    return this->nodi.size();
}


