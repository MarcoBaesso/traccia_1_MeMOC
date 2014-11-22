#ifndef COORDINATA_H
#define COORDINATA_H

class coordinata
{
public:
    coordinata();
    coordinata(double x,double y);
    double get_x();
    double get_y();

private:
    double x;
    double y;
};

#endif // COORDINATA_H
