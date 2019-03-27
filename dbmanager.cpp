#include "dbmanager.h"
#include "QSqlQueryModel"

DBManager::DBManager(QString dbType)
{
    QString driver ;
    if (dbType=="QSQLITE")
        driver = "QSQLITE" ;
    else if (dbType=="MSSQL")
        driver = "QODBC" ;
    db = QSqlDatabase::addDatabase(driver) ;
    this->SQLDriver = driver ;

}

bool DBManager::openDatabase(const QString Server,
                             const QString User,
                             const QString Password,
                             const QString databaseName,
                             bool trustedConnection ) {
    if(QSqlDatabase::isDriverAvailable(SQLDriver)) {
        if(SQLDriver=="QSQLITE"){
            db.setDatabaseName("results.db");
            if(!db.open()) {
                qWarning() << "ERROR: " << db.lastError();
                return false ;
            }
            QString todayDate= this->currentDateTimeJalali().split(" ")[0].replace('/','_');
            QString queryText = QString("CREATE TABLE IF NOT EXISTS t%1"
                    " (id INTEGER PRIMARY KEY AUTOINCREMENT,occupant_total INTEGER,occupant_front INTEGER,occupant_back INTEGER ,date varchar(50),"
                      "imagedata_raw_front BLOB, imagedata_raw_back BLOB, imagedata_processed_front BLOB, imagedata_processed_back BLOB)").arg(todayDate) ;

            QSqlQuery query(queryText);
            if(!query.isActive()) {
                qWarning() << "ERROR: " << query.lastError().text();
                return false ;
            }

        }

        else if (SQLDriver=="QODBC") {
            this->mdatabaseName = databaseName;
            db.setDatabaseName(QString("DRIVER={%1};SERVER=%2;"
                                       "UID=%3;PWD=%4;Trusted_Connection=%5;")
                               .arg("SQL SERVER")
                               .arg(Server)
                               .arg(User)
                               .arg(Password)
                               .arg(trustedConnection ? "Yes" : "No"));
            if(!db.open()) {

                qWarning() << "ERROR: " << db.lastError();
                return false ;
            }

            QString queryText = QString("if not exists(select * from sys.databases where name = '%1') "
                                "create database %1").arg(databaseName) ;

            QSqlQuery query(queryText);
            if(!query.isActive()) {
                qWarning() << "ERROR: " << query.lastError().text();
                return false ;
            }

            QString todayDate= this->currentDateTimeJalali().split(" ")[0].replace('/','_');
            queryText = QString("IF NOT EXISTS (SELECT * FROM %1.sys.sysobjects WHERE name='t%2' and xtype='U') CREATE TABLE [%1].[dbo].t%2 ([id] [int] IDENTITY(1,1) NOT NULL,[occupant_total] [int] NULL,[occupant_front] [int] NULL,[occupant_back] [int] NULL,[date] [varchar](50) NULL,[imagedata_raw_front] [varbinary](max) NULL,[imagedata_raw_back] [varbinary](max) NULL,[imagedata_processed_front] [varbinary](max) NULL,[imagedata_processed_back] [varbinary](max) NULL)").arg(databaseName).arg(todayDate) ;

            QSqlQuery queryTable(queryText);
            if(!queryTable.isActive()) {
                qWarning() << "ERROR: " << queryTable.lastError().text();
                return false ;
            }
        }


    }
    else
        return false ;


}
// Return All table names (dates)
QStringList DBManager::GetTableNames () {
    QString querytxt ;
    if(SQLDriver=="QSQLITE")
        querytxt = "SELECT name FROM sqlite_master WHERE type = 'table' and name!='sqlite_sequence' ORDER BY name DESC";
    else if (SQLDriver=="QODBC")
        querytxt = QString("SELECT name FROM %1.sys.sysobjects WHERE xtype='U' ORDER BY name DESC").arg(mdatabaseName);

    QSqlQuery query;
    query.exec(querytxt) ;
    // create an empty QStringList
    QStringList tableNames ;
    // Replace 't' by '' in table names
    while (query.next())
        tableNames.append(query.value(0).toString().replace('t',""));
    return tableNames ;
}
// return sotred picture for a specific table row
DBManager::DetectionImages DBManager::getPicture(int id, QString table_name) {
        // prepate query
        QString querytxt ;

        if(SQLDriver=="QSQLITE")
            querytxt = QString("SELECT imagedata_raw_front, imagedata_raw_back , imagedata_processed_front ,"
                       "imagedata_processed_back FROM t%1 WHERE id = ?").arg(table_name);

        else if (SQLDriver=="QODBC")
            querytxt = QString("SELECT imagedata_raw_front, imagedata_raw_back , imagedata_processed_front ,"
                       "imagedata_processed_back FROM %1.dbo.t%2 WHERE id = ?").arg(mdatabaseName).arg(table_name);


        QSqlQuery query;
        query.prepare(querytxt);
        query.bindValue(0, id);
        query.exec() ;
        query.first() ;

        // covert QByteArrys to Pixmaps
        QByteArray outByteArray = query.value( 0 ).toByteArray();
        QPixmap FrontRawPixmap = QPixmap();
        FrontRawPixmap.loadFromData( outByteArray );

        outByteArray = query.value( 1 ).toByteArray();
        QPixmap BackRawPixmap = QPixmap();
        BackRawPixmap.loadFromData( outByteArray );

        outByteArray = query.value( 2 ).toByteArray();
        QPixmap FrontProcessedPixmap = QPixmap();
        FrontProcessedPixmap.loadFromData( outByteArray );


        outByteArray = query.value( 3 ).toByteArray();
        QPixmap BackProcessedPixmap = QPixmap();
        BackProcessedPixmap.loadFromData( outByteArray );

        // return stored pictures
        DBManager::DetectionImages images = {FrontRawPixmap,BackRawPixmap,FrontProcessedPixmap,BackProcessedPixmap} ;
        return images;


}
QSqlQueryModel* DBManager::getDataModel(QString date) {
    //    editSelectedRowId = -2 ;

        QSqlQueryModel *model = new QSqlQueryModel() ;

        if(SQLDriver=="QSQLITE")
             model->setQuery(QString("SELECT id,occupant_total,date FROM t%1 order by id DESC").arg(date)) ;
        else if (SQLDriver=="QODBC")
             model->setQuery(QString("SELECT id,occupant_total,date FROM %1.dbo.t%2 order by id DESC").arg(mdatabaseName).arg(date)) ;
        model->setHeaderData(0, Qt::Orientation::Horizontal, ("رکورد"));
        model->setHeaderData(1,  Qt::Orientation::Horizontal, ("تعداد سرنشین"));
        model->setHeaderData(2,  Qt::Orientation::Horizontal, ("ساعت"));
        return model;

}

QString DBManager::currentDateTimeJalali() {


    QDateTime a = QDateTime::currentDateTime() ;

    QStringList shamsi=  mdate.ToJalali( QString::number(a.date().currentDate().year()),
                                         QString::number(a.date().currentDate().month()),
                                         QString::number(a.date().currentDate().day()));

    QString JalailDate =shamsi.at(0)+
            "/"+QString("%1").arg(shamsi.at(1).toInt(),2,10,QLatin1Char('0'))+
            "/"+QString("%1").arg(shamsi.at(2).toInt(),2,10,QLatin1Char('0'))+ " " +a.time().currentTime().toString();;

    return JalailDate ;


}

void DBManager::insertResult(CarOccupancy occupant) {
//    qDebug () << occupant.FrontOccupantNumber ;

    if (occupant.FrontOccupantNumber+occupant.BackOccupantNumber>0 ) {
        QSqlQuery query = QSqlQuery(db);

        QString todayDate= this->currentDateTimeJalali().split(" ")[0].replace('/','_');
        QString queryText ;
        if (occupant.BackCarImageProcessed.empty()){
            if(SQLDriver=="QSQLITE")
                queryText = QString("INSERT INTO t%1 (occupant_total, occupant_front, date , imagedata_raw_front, imagedata_processed_front)"
                                                              " VALUES (:occupant_total, :occupant_front, :date , :imagedata_raw_front , :imagedata_processed_front)").arg(todayDate) ;

            else if (SQLDriver=="QODBC")
                queryText = QString("INSERT INTO %1.dbo.t%2 (occupant_total, occupant_front, date , imagedata_raw_front, imagedata_processed_front)"
                                                              " VALUES (:occupant_total, :occupant_front, :date , :imagedata_raw_front , :imagedata_processed_front)").arg(mdatabaseName).arg(todayDate) ;


        }

        else {
            if(SQLDriver=="QSQLITE")
                queryText = QString("INSERT INTO t%1 (occupant_total, occupant_front,occupant_back, date , imagedata_raw_front, imagedata_processed_front , imagedata_raw_back, imagedata_processed_back)"
                                                       " VALUES (:occupant_total, :occupant_front , :occupant_back , :date , :imagedata_raw_front , :imagedata_processed_front , :imagedata_raw_back , :imagedata_processed_back)").arg(todayDate) ;
            else if (SQLDriver=="QODBC")
                queryText = QString("INSERT INTO %1.dbo.t%2 (occupant_total, occupant_front,occupant_back, date , imagedata_raw_front, imagedata_processed_front , imagedata_raw_back, imagedata_processed_back)"
                                                       " VALUES (:occupant_total, :occupant_front , :occupant_back , :date , :imagedata_raw_front , :imagedata_processed_front , :imagedata_raw_back , :imagedata_processed_back)").arg(mdatabaseName).arg(todayDate) ;
        }



        query.prepare( queryText );

        QPixmap FrontPixmap = ASM::cvMatToQPixmap(occupant.FrontCarImage) ;
        QByteArray inByteArrayFront;
        QBuffer inBufferFront( &inByteArrayFront );
        inBufferFront.open( QIODevice::WriteOnly );
        FrontPixmap.save( &inBufferFront, "PNG" ); // write inPixmap into inByteArray in PNG format

        QPixmap FrontProcessedPixmap = ASM::cvMatToQPixmap(occupant.FrontCarImageProcessed) ;
        QByteArray inByteArrayFrontProcessed;
        QBuffer inBufferFrontProcessed( &inByteArrayFrontProcessed );
        inBufferFrontProcessed.open( QIODevice::WriteOnly );
        FrontProcessedPixmap.save( &inBufferFrontProcessed, "PNG" ); // write inPixmap into inByteArray in PNG format

        query.bindValue( ":occupant_front", occupant.FrontOccupantNumber );
        query.bindValue( ":occupant_total", occupant.FrontOccupantNumber + occupant.BackOccupantNumber );
        query.bindValue(":date", this->currentDateTimeJalali().split(" ")[1]);
        query.bindValue( ":imagedata_raw_front", inByteArrayFront );
        query.bindValue( ":imagedata_processed_front", inByteArrayFrontProcessed );

        if (!occupant.BackCarImageProcessed.empty()) {
            QPixmap BackPixmap = ASM::cvMatToQPixmap(occupant.BackCarImage) ;
            QByteArray inByteArrayBack;
            QBuffer inBufferBack( &inByteArrayBack );
            inBufferBack.open( QIODevice::WriteOnly );
            BackPixmap.save( &inBufferBack, "PNG" ); // write inPixmap into inByteArray in PNG format

            QPixmap BackProcessedPixmap = ASM::cvMatToQPixmap(occupant.BackCarImageProcessed) ;
            QByteArray inByteArrayBackProcessed;
            QBuffer inBufferBackProcessed( &inByteArrayBackProcessed );
            inBufferBackProcessed.open( QIODevice::WriteOnly );
            BackProcessedPixmap.save( &inBufferBackProcessed, "PNG" ); // write inPixmap into inByteArray in PNG format

            query.bindValue( ":occupant_front", occupant.BackOccupantNumber );
            query.bindValue( ":imagedata_raw_back", inByteArrayBack );
            query.bindValue( ":imagedata_processed_back", inByteArrayBackProcessed);



        }

        if( !query.exec() )
            qDebug() << "Error inserting image into table:\n" << query.lastError();
    }

}

