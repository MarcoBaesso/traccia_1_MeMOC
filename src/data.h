#ifndef DATA_H
#define DATA_H

#include <QString>
#include <coordinata.h>
#include <QList>
#include <QVector>
#include <stdio.h>

using namespace std;

class data
{
public:
    data(QString filename);
    vector<vector<double > > get_tempi() const;
    vector<coordinata> get_nodi() const;

    QString get_file_name() const;
    int get_num_nodi() const;

    static double calcola_distanza(coordinata pt1,coordinata pt2);

    static const QString path_file;
private:
    QString filename;
    vector<coordinata> nodi;
    vector<vector<double > > tempi;
};



#endif // DATA_H
