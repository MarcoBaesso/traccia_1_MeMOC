#ifndef MODELLO_TSP_H
#define MODELLO_TSP_H

#include "cpxmacro.h"
#include "data.h"
#include <stdio.h>
#include <message_error.h>

using namespace std;

void crea_variabili(CEnv env, Prob lp, data const* dati, int & num_vars, vector<vector<int> > & map_x, vector<vector<int> > & map_y){

    const int NAME_SIZE = 512;
    char name[NAME_SIZE];

    int num_nodi=dati->get_num_nodi();
    // creazione delle x
    map_x.resize(num_nodi);
    int contatore= 0;
    for(int i=0;i<num_nodi;i++){
        map_x[i].resize(num_nodi-1);
        for(int j=0;j<num_nodi;j++){
            if (i!=j){
                char xtype= 'I';
                double obj=0.0;
                double lb=0.0;
                double ub=CPX_INFBOUND;
                snprintf(name, NAME_SIZE, "x%d%d", i, j);
                char* xname = (char*)(&name[0]);

                CHECKED_CPX_CALL(CPXnewcols,env,lp,1,&obj,&lb,&ub,&xtype,&xname);
                if (j>i){
                    map_x[i][j-1]=contatore;
                    //std::cout<<map_x[i][j-1]<<",";
                }
                else{
                    map_x[i][j]=contatore;
                    //std::cout<<map_x[i][j]<<",";
                }
                contatore++;
            }
        }
    }

    contatore= CPXgetnumcols(env, lp);
    if (contatore!=(num_nodi*(num_nodi-1)))
        throw message_error("Errore nella creazione delle colonne delle variabili x");

    vector<vector<double > > tempi=dati->get_tempi();
    // creazione delle y
    map_y.resize(num_nodi);
    for(int i=0;i<num_nodi;i++){
        map_y[i].resize(num_nodi-1);
        for(int j=0;j<num_nodi;j++){
            if (i!=j){
                char xtype= 'B';
                double obj;
                if (j>i)
                    obj=tempi[i][j-1];
                else
                    obj=tempi[i][j];
                double lb=0.0;
                double ub=1.0;
                snprintf(name, NAME_SIZE, "y%d%d", i, j);
                char* yname = (char*)(&name[0]);

                CHECKED_CPX_CALL(CPXnewcols,env,lp,1,&obj,&lb,&ub,&xtype,&yname);
                if (j>i){
                    map_y[i][j-1]=contatore;
                    //std::cout<<map_y[i][j-1]<<",";
                }
                else{
                    map_y[i][j]=contatore;
                    //std::cout<<map_y[i][j]<<",";
                }
                contatore++;
            }
        }
    }

    contatore= CPXgetnumcols(env, lp);
    if (contatore!=(num_nodi*(num_nodi-1))*2)
        throw message_error("Errore nella creazione delle colonne delle variabili y");

    num_vars=contatore;

}

void crea_vincoli(CEnv env, Prob lp, data const *dati, vector<vector<int> > const & map_x, vector<vector<int> > const & map_y){

    //inserimento vincolo quantità flusso in uscita dal nodo iniziale
    {
        vector<int> idx(dati->get_num_nodi()-1);
        for(int j=0;j<dati->get_num_nodi()-1;j++){
            idx[j]=map_x[0][j];
        }
        vector<double> coef(dati->get_num_nodi()-1,1.0);
        char sense = 'E';
        int matbeg = 0;
        double rhs=dati->get_num_nodi();
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    //inserimento vincolo: differenza tra quantità flusso in entrata e in uscita pari a 1
    for(int k=1;k<dati->get_num_nodi();k++){
        vector<int> idx((dati->get_num_nodi()-1)*2);
        vector<double> coef((dati->get_num_nodi()-1)*2);
        // prima sommatoria
        for(int i=0;i<dati->get_num_nodi();i++){
            if (i!=k){
                if (k>i){
                    idx[i]=map_x[i][k-1];
                    coef[i]=1.0;
                }
                else{
                    idx[i-1]=map_x[i][k];
                    coef[i-1]=1.0;
                }
            }
        }
        int from=dati->get_num_nodi()-1;
        // seconda sommatoria
        for(int j=0;j<dati->get_num_nodi();j++){
            if (j!=k){
                if (j>k){
                    idx[from+j-1]=map_x[k][j-1];
                    coef[from+j-1]=-1.0;
                }
                else{
                    idx[from+j]=map_x[k][j];
                    coef[from+j]=-1.0;
                }
            }
        }
        char sense = 'E';
        int matbeg = 0;
        double rhs= 1.0;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    // inserimento vincolo: sommatoria degli archi attivi uscenti da un nodo deve essere pari a 1
    for(int i=0;i<dati->get_num_nodi();i++){
        vector<int> idx(dati->get_num_nodi()-1);
        vector<double> coef(dati->get_num_nodi()-1,1.0);
        for(int j=0;j<dati->get_num_nodi();j++){
            if (i!=j){
                if (j>i){
                    idx[j-1]=map_y[i][j-1];
                }
                else{
                    idx[j]=map_y[i][j];
                }
            }
        }
        char sense = 'E';
        int matbeg = 0;
        double rhs= 1.0;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    // inserimento vincolo: sommatoria degli archi attivi entranti ad un nodo deve essere pari a 1
    for(int j=0;j<dati->get_num_nodi();j++){
        vector<int> idx(dati->get_num_nodi()-1);
        vector<double> coef(dati->get_num_nodi()-1,1.0);
        for(int i=0;i<dati->get_num_nodi();i++){
            if (i!=j){
                if (j>i){
                    idx[i]=map_y[i][j-1];
                }
                else{
                    idx[i-1]=map_y[i][j];
                }
            }
        }
        char sense = 'E';
        int matbeg = 0;
        double rhs= 1.0;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
    }

    //inserimento vincolo del bigM
    for(int i=0;i<dati->get_num_nodi();i++){
        for(int j=0;j<dati->get_num_nodi()-1;j++){
            vector<int> idx(2);
            vector<double> coef(2);
            idx[0]=map_x[i][j];
            idx[1]=map_y[i][j];
            coef[0]=1.0;
            coef[1]=(dati->get_num_nodi())*(-1.0);
            char sense = 'L';
            int matbeg = 0;
            double rhs= 0.0;
            CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], NULL, NULL);
        }
    }

}

#endif // MODELLO_TSP_H
