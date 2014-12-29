
#include <iostream>
#include <ilcplex/cplex.h>
#include <data.h>
#include <message_error.h>
#include "cpxmacro.h"
#include "modello_tsp.h"
#include <time.h>
#include <generatore.h>
#include <type_generatore_casuale.h>
#include <QFile>

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

int main (int argc, char const *argv[])
{
    try{
        //argv[1] è la distribuzione dei nodi nel test
        //argv[2] è il numero di nodi da generare

        QString input_file;

        if (QString(argv[1])=="default"){
            input_file= "default";
        }
        else{
            generatore_casuale type_tsp=convert_string_to_generatore(argv[1]);
            bool ok_conversion=true;
            int num_nodi= QString(argv[2]).toInt(&ok_conversion);
            if (ok_conversion==false)
                throw message_error("come secondo parametro inserire un intero");

            int dispersione= QString(argv[3]).toInt(&ok_conversion);
            if (ok_conversion==false)
                throw message_error("come terzo parametro inserire la dispersione da cui prelevare le x");

            if (dispersione<num_nodi)
                throw message_error("la dispersione deve essere almeno pari al numero di nodi");

            input_file=genera_nodi_problema(type_tsp,num_nodi,dispersione);
        }


        data dati(input_file);
        int num_vars;
        vector<vector<int> > map_x;
        vector<vector<int> > map_y;
        double objval;
        vector<double> solution;
        const int NAME_SIZE = 512;
        char name[NAME_SIZE];

        DECL_ENV( env );
        DECL_PROB( env, lp );

        crea_variabili(env,lp,&dati,num_vars,map_x,map_y);

        crea_vincoli(env,lp,&dati,map_x,map_y);

        CHECKED_CPX_CALL(CPXwriteprob, env, lp, "tsp.lp", 0);

        clock_t initial_timer=clock();
        clock_t final_timer;

        // optimize
        CHECKED_CPX_CALL(CPXmipopt, env, lp);

        final_timer=clock()-initial_timer;

        // print objval
        CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
        std::cout<<"Objval: "<<objval<< std::endl;

        solution.resize(num_vars);
        CHECKED_CPX_CALL(CPXgetx, env, lp, &solution[0], 0, num_vars-1);
        for(int i=0;i<num_vars;i++){
            if (i<(dati.get_num_nodi()*(dati.get_num_nodi()-1))){
                int riga=i/(dati.get_num_nodi()-1);
                int colonna=i%(dati.get_num_nodi()-1);
                if (colonna>=riga)
                    colonna=colonna+1;
                snprintf(name, NAME_SIZE, "x_%d_%d", riga, colonna);
            }
            else{
                int riga=(i/(dati.get_num_nodi()-1));
                int colonna=i%(dati.get_num_nodi()-1);
                if (colonna>=riga)
                    colonna=colonna+1;
                snprintf(name, NAME_SIZE, "y_%d_%d", riga-dati.get_num_nodi(), colonna);
            }
            char* var_name = (char*)(&name[0]);
            QString variabile=QString(var_name);
            double val_solution=solution[i];
            if (val_solution==-0 || val_solution==-0.0)
                val_solution=0;
            if (val_solution!=0)
                std::cout<<variabile.toStdString()<<"="<<val_solution<<std::endl;

        }

        CHECKED_CPX_CALL(CPXsolwrite, env, lp, "tsp.sol");

        // free
        CPXfreeprob(env, &lp);
        CPXcloseCPLEX(&env);

        std::cout<<"Time: "<<(float(final_timer))/CLOCKS_PER_SEC<<" secondi"<<std::endl;
    }
    catch (message_error error){
        std::cout<<error.what()<<std::endl;
    }
    catch(std::exception& e)
    {
        std::cout<<"EXCEPTION: "<<e.what()<<std::endl;
    }
}
