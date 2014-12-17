#ifndef TYPE_GENERATORE_CASUALE_H
#define TYPE_GENERATORE_CASUALE_H

#include <Qt>
#include <message_error.h>

enum generatore_casuale {cerchio,randoms};

generatore_casuale convert_string_to_generatore(QString generatore){

    if (generatore=="cerchio")
        return cerchio;
    else{
        if (generatore=="randoms")
            return randoms;
    }

    throw message_error("tipo di generatore non conosciuto");
}

QString convert_generatore_to_string(generatore_casuale generatore){

    switch (generatore){
    case (cerchio):
        return "cerchio";
    case (randoms):
        return "randoms";


    }

}

#endif // TYPE_GENERATORE_CASUALE_H
