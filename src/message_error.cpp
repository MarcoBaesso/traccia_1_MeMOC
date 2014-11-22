#include "message_error.h"

message_error::message_error(char* errore)
{
    this->errore=errore;
}

const char* message_error::what() const throw(){
    return this->errore;
}
