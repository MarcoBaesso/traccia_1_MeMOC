#ifndef MESSAGE_ERROR_H
#define MESSAGE_ERROR_H

#include <QString>
#include <iostream>

using namespace std;

class message_error : public std::exception
{
public:
    message_error(char* errore);
    const char* what() const throw();

private:
    char* errore;
};

#endif // MESSAGE_ERROR_H
