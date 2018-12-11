#include "dbmanager.h"

DBManager::DBManager()
{
    db = QSqlDatabase::addDatabase(DRIVER) ;
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
//        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName("results.db");
        if(!db.open())
          qWarning() << "ERROR: " << db.lastError();
        QString todayDate= this->currentDateTimeJalali().split(" ")[0].replace('/','_');
        QString queryText = "CREATE TABLE IF NOT EXISTS t" + todayDate+
                " (id INTEGER PRIMARY KEY AUTOINCREMENT,occupant_total INTEGER,occupant_front INTEGER,occupant_back INTEGER ,date varchar(50),"
                  "imagedata_raw_front BLOB, imagedata_raw_back BLOB, imagedata_processed_front BLOB, imagedata_processed_back BLOB)" ;
//        qDebug() << queryText ;
        QSqlQuery query(queryText);
        if(!query.isActive())
            qWarning() << "ERROR: " << query.lastError().text();

    }

}
QStringList DBManager::GetTableNames () {
    QString querytxt ;
    querytxt = "SELECT name FROM sqlite_master WHERE type = 'table' and name!='sqlite_sequence' ORDER BY name DESC";
    QSqlQuery query;
    query.exec(querytxt) ;
    QStringList tableNames ;
    while (query.next())
        tableNames.append(query.value(0).toString().replace('t',""));
    return tableNames ;
}
DBManager::DetectionImages DBManager::getPicture(int id, QString table_name) {
        QString querytxt ;
        querytxt = "SELECT imagedata_raw_front, imagedata_raw_back BLOB, imagedata_processed_front BLOB,"
                   "imagedata_processed_back BLOB FROM t"+table_name+" WHERE id = ?";
        QSqlQuery query;
        query.prepare(querytxt);
        query.bindValue(0, id);
        query.exec() ;
        query.first() ;

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


        DBManager::DetectionImages images = {FrontRawPixmap,BackRawPixmap,FrontProcessedPixmap,BackProcessedPixmap} ;
        return images;


}
QSqlTableModel* DBManager::getDataModel(QString date) {
    //    editSelectedRowId = -2 ;
        QSqlTableModel* dataModel = new QSqlTableModel();
    //        connect(dataModel, SIGNAL( dataChanged(QModelIndex,QModelIndex,QVector<int>)), SLOT(handleAfterEdit(QModelIndex,QModelIndex,QVector<int>)));

        dataModel->setTable("t"+date);
        dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //    if (ui->lineEditSearch->text()!="") {
    //        QString filter_txt = "code like '" + ui->lineEditSearch->text() + "%'";
    //        dataModel->setFilter(filter_txt);
    //    }

        dataModel->select();
        dataModel->removeColumn(1);
        dataModel->removeColumn(2);
        dataModel->removeColumn(3);
        dataModel->removeColumn(3);
        dataModel->removeColumn(3);
        dataModel->removeColumn(3);

        dataModel->setHeaderData(0, Qt::Orientation::Horizontal, ("رکورد"));
        dataModel->setHeaderData(1,  Qt::Orientation::Horizontal, ("تعداد سرنشین"));
        dataModel->setHeaderData(2,  Qt::Orientation::Horizontal, ("تاریخ"));
        return  dataModel ;

}

QString DBManager::currentDateTimeJalali() {

    QDateTime a = QDateTime::currentDateTime() ;
    QString date_string = a.toString("yyyy-MM-dd hh,mm,ss,zzz") ;
    QStringList parts = date_string.split(' ');
    QStringList parts_date = parts[0].split('-') ;
//    return a.toString("yyyy-MM-dd hh,mm,ss,zzz");

    QStringList shamsi=  mdate.ToJalali( parts_date[0],parts_date[1],parts_date[2]);
    QString JalailDate =shamsi.at(0)+"/"+shamsi.at(1)+"/"+shamsi.at(2)+ " " +parts[1];
    return JalailDate ;
//    qDebug()<<JalailDate;
//    // jalali to gregorian
//    QStringList m= mdate.ToMiladi("1372","3","6");
//    QString miladiDate= m.at(0)+"/"+ m.at(1)+"/"+m.at(2);
//    qDebug()<<miladiDate;


}
QString DBManager::currentDateTimeMiladi() {
    QDateTime a = QDateTime::currentDateTime() ;
    return a.toString("yyyy-MM-dd hh,mm,ss,zzz");
}

void DBManager::insertResult(CarOccupancy occupant) {
    qDebug () << occupant.FrontOccupantNumber ;

    if (occupant.FrontOccupantNumber+occupant.BacktOccupantNumber>0 ) {
        QSqlQuery query = QSqlQuery(db);

        QString todayDate= this->currentDateTimeJalali().split(" ")[0].replace('/','_');
        QString queryText = "INSERT INTO t" + todayDate+ +"  (occupant_total, occupant_front, date , imagedata_raw_front, imagedata_processed_front)"
                                                          " VALUES (:occupant_total, :occupant_front, :date , :imagedata_raw_front , :imagedata_processed_front)" ;


        query.prepare( queryText );

        QPixmap inPixmap = ASM::cvMatToQPixmap(occupant.FrontCarImage) ;
        QByteArray inByteArray;
        QBuffer inBuffer( &inByteArray );
        inBuffer.open( QIODevice::WriteOnly );
        inPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format

        QPixmap processedPixmap = ASM::cvMatToQPixmap(occupant.FrontCarImageProcessed) ;
        QByteArray inByteArrayProcessed;
        QBuffer inBufferProcessed( &inByteArrayProcessed );
        inBufferProcessed.open( QIODevice::WriteOnly );
        processedPixmap.save( &inBufferProcessed, "PNG" ); // write inPixmap into inByteArray in PNG format

        query.bindValue( ":occupant_front", occupant.FrontOccupantNumber );
        query.bindValue(":date", QString::fromStdString(currentDateTime()));
        query.bindValue( ":imagedata_raw_front", inByteArray );
        query.bindValue( ":imagedata_processed_front", inByteArrayProcessed );

        if( !query.exec() )
            qDebug() << "Error inserting image into table:\n" << query.lastError();
    }

}

