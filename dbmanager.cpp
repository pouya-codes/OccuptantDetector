#include "dbmanager.h"

DBManager::DBManager()
{
    db = QSqlDatabase::addDatabase(DRIVER) ;
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
//        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName("results.db");
        if(!db.open())
          qWarning() << "ERROR: " << db.lastError();
        QSqlQuery query("CREATE TABLE IF NOT EXISTS result (id INTEGER PRIMARY KEY AUTOINCREMENT,occupant_total INTEGER,occupant_front INTEGER,occupant_back INTEGER ,date varchar(50),"
                        "imagedata_raw_front BLOB, imagedata_raw_back BLOB, imagedata_processed_front BLOB, imagedata_processed_back BLOB)");
        if(!query.isActive())
            qWarning() << "ERROR: " << query.lastError().text();

    }

}
DBManager::DetectionImages DBManager::getPicture(int id) {
        QString querytxt ;
        querytxt = "SELECT imagedata_raw_front, imagedata_raw_back BLOB, imagedata_processed_front BLOB,"
                   "imagedata_processed_back BLOB FROM result WHERE id = ?";
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
QSqlTableModel* DBManager::getDataModel() {
    //    editSelectedRowId = -2 ;
        QSqlTableModel* dataModel = new QSqlTableModel();
    //        connect(dataModel, SIGNAL( dataChanged(QModelIndex,QModelIndex,QVector<int>)), SLOT(handleAfterEdit(QModelIndex,QModelIndex,QVector<int>)));

        dataModel->setTable("result");
        dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //    if (ui->lineEditSearch->text()!="") {
    //        QString filter_txt = "code like '" + ui->lineEditSearch->text() + "%'";
    //        dataModel->setFilter(filter_txt);
    //    }

        dataModel->select();
        dataModel->removeColumn(2);
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

void DBManager::insertResult(CarOccupancy occupant) {
    if (occupant.FrontOccupantNumber+occupant.BacktOccupantNumber>0 ) {
        QSqlQuery query = QSqlQuery(db);
        query.prepare( "INSERT INTO result (occupant, date , imagedata) VALUES (:occupant, :date , :imageData)" );

        QPixmap inPixmap = ASM::cvMatToQPixmap(occupant.FrontCarImage) ;
        QByteArray inByteArray;
        QBuffer inBuffer( &inByteArray );
        inBuffer.open( QIODevice::WriteOnly );
        inPixmap.save( &inBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format

        query.bindValue( ":occupant", occupant.FrontOccupantNumber+occupant.BacktOccupantNumber );
        query.bindValue(":date", QString::fromStdString(currentDateTime()));
        query.bindValue( ":imageData", inByteArray );

        if( !query.exec() )
            qDebug() << "Error inserting image into table:\n" << query.lastError();
    }

}

