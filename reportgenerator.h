#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H
#include <QVector>

class ReportGenerator
{
public:
    ReportGenerator();
    int bookSatistics();
    int memberStatistics();
    QVector<QVector<QString>> loadTopEmpruntedBook();

};

#endif // REPORTGENERATOR_H



