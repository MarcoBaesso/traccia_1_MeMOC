#include "coordinata.h"

coordinata::coordinata(){
    x=0;
    y=0;
}

coordinata::coordinata(double x,double y){
    this->y=y;
    this->x=x;
}
double coordinata::get_x(){
    return x;
}
double coordinata::get_y(){
    return y;
}
