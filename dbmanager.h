#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
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
#include <qdateconvertor.h>
#include <QStringList>

class DBManager
{
public:
    struct DetectionImages{
        QPixmap Front_Image_Raw ;
        QPixmap Back_Image_Raw ;
        QPixmap Front_Image_Processed ;
        QPixmap Back_Image_Processed ;
    };
    DBManager(QString dbType);
    bool createTable() ;
    bool openDatabase(const QString Server="",
                      const QString User="",
                      const QString Password="",
                      const QString databaseName = "OccupantDetectorDB",
                      bool trustedConnection = true ) ;
    QSqlQueryModel* getDataModel(QString date);
    DetectionImages getPicture (int id, QString table_name) ;
    void insertResult(CarOccupancy occupant);
    QString currentDateTimeJalali() ;
    QStringList GetTableNames();

private:

    QDateConvertor mdate;
    QString SQLDriver ;
    QSqlDatabase db ;
    QString mdatabaseName;
    QString lastDate = "" ;
//    QString DRIVER = "QSQLITE";
};

#endif // DBMANAGER_H
