#include "dbmanager.h"

DBManager::DBManager()
{
    db = QSqlDatabase::addDatabase(DRIVER) ;
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
//        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName("results.db");
        if(!db.open())
          qWarning() << "ERROR: " << db.lastError();
        QSqlQuery query("CREATE TABLE IF NOT EXISTS result (id INTEGER PRIMARY KEY AUTOINCREMENT,occupant INTEGER,date varchar(50), imagedata BLOB)");
        if(!query.isActive())
            qWarning() << "ERROR: " << query.lastError().text();

    }

}
QPixmap DBManager::getPicture(int id) {
        QString querytxt ;
        querytxt = "SELECT imagedata FROM result WHERE id = ?";
        QSqlQuery query;
        query.prepare(querytxt);
        query.bindValue(0, id);
        query.exec() ;
        query.first() ;

        QByteArray outByteArray = query.value( 0 ).toByteArray();
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData( outByteArray );
        return outPixmap;


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
        dataModel->removeColumn(3);

        dataModel->setHeaderData(0, Qt::Orientation::Horizontal, ("ID"));
        dataModel->setHeaderData(1,  Qt::Orientation::Horizontal, ("Occupant"));
        dataModel->setHeaderData(2,  Qt::Orientation::Horizontal, ("Date"));
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

