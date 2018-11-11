#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QSqlTableModel>
#include <QString>
#include <QVariant>
#include <QPixmap>
#include <caroccupancy.h>
#include <asmOpenCV.h>
#include <QBuffer>

class DBManager
{
public:
    DBManager();
    QSqlTableModel* getDataModel();
    QPixmap getPicture (int id) ;
    void insertResult(CarOccupancy occupant);
private:
    QSqlDatabase db ;
    QString DRIVER = "QSQLITE";
};

#endif // DBMANAGER_H
