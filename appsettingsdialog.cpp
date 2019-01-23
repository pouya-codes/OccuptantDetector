#include "appsettingsdialog.h"
#include "ui_appsettingsdialog.h"
#include "QFileDialog"
#include <iostream>
#include <QColorDialog>

AppSettingsDialog::AppSettingsDialog(AppSettings& settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsDialog)
{
    ui->setupUi(this);
    this->settings = &settings;

    connect(ui->label_color_car , SIGNAL(clicked()), this, SLOT(color_car_licked()));
    connect(ui->label_color_occupant, SIGNAL(clicked()), this, SLOT(color_occupant_label1clicked()));
    connect(ui->label_color_window, SIGNAL(clicked()), this, SLOT(color_window_label1clicked()));
    connect(ui->label_color_window_back, SIGNAL(clicked()), this, SLOT(color_window_back_label1clicked()));


    loadSettings() ;
}

AppSettingsDialog::~AppSettingsDialog()
{
    delete ui;
}
QString AppSettingsDialog::browseFile(QString selectFilter) {
    return QFileDialog::getOpenFileName(this,"Select a file",QDir::currentPath(),selectFilter);

}

QString AppSettingsDialog::browsePath(){
    return QFileDialog::getExistingDirectory(this,"Select a file", QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void AppSettingsDialog::setLableColor (ClickableLabel* lable, QColor color) {
    QPalette palette = lable->palette();
    palette.setColor(lable->backgroundRole(), color);
    lable->setPalette(palette);
    lable->setAutoFillBackground(true);
    lable->color = color ;


}
// load setting and set UI values
void AppSettingsDialog::loadSettings() {

    // set array for storing UI's line edits
    line_edits = {
        ui->le_source1, ui->le_source2 ,
        ui->le_spp_cfg , ui->le_spp_weight,
        ui->le_tiny_cfg, ui->le_tiny_weight,
        ui->le_windows_cfg, ui->le_windows_weight, ui->le_save_path
    } ;

    // set array for storing UI's line edits KEYS
    line_edits_keys = {
        settings->KEY_SOURCE_1, settings->KEY_SOURCE_2,
        settings->KEY_OCCUPANT_CFG, settings->KEY_OCCUPANT_WEIGHTS,
        settings->KEY_TINY_CFG, settings->KEY_TINY_WEIGHTS,
        settings->KEY_WINDOWS_CFG, settings->KEY_WINDOWS_WEIGHTS, settings->KEY_SAVE_VIDEOS_PATH
    } ;

    // set array for storing UI's spin boxs
    spin_boxs = {
        ui->sb_car_spp,ui->sb_car_tiny,
                 ui->sb_occupant,ui->sb_windows
    };

    // set array for storing UI's spin boxs KEYS
    spin_boxs_keys = {
        settings->KEY_CAR_THREADSHOLD,settings->KEY_TINY_CAR_THREADSHOLD,
        settings->KEY_OCCUPANT_THREADSHOLD,settings->KEY_WINDOW_THREADSHOLD
    };

    // set array for storing UI's colors
    color_lables = {
        ui->label_color_car,ui->label_color_occupant,
        ui->label_color_window,ui->label_color_window_back
    };

    // set array for storing UI's colors KEYS
    color_lable_keys = {
        settings->KEY_COLOR_CAR,settings->KEY_COLOR_OCCUPANT,
        settings->KEY_COLOR_WINDOW,settings->KEY_COLOR_WINDOW_BACK
    };

    // set array for storing UI's checkboxs
    checkboxs = {
        ui->cb_driver_detection,ui->cb_save_videos
    };

    // set array for storing UI's checkboxs KEYS
    checkbox_keys = {
        settings->KEY_DETECT_DRIVER,settings->KEY_SAVE_VIDEOS
    };



    // set line edit values
    for (unsigned idx = 0; idx < line_edits_keys.size(); ++idx)
        line_edits[idx]->setText(settings->getSetting(line_edits_keys[idx]).toString()) ;

    // set spin boxs values
    for (unsigned idx = 0; idx < spin_boxs_keys.size(); ++idx)
        spin_boxs[idx]->setValue(settings->getSetting(spin_boxs_keys[idx]).toDouble()) ;

    // set colors values
    for (unsigned idx = 0; idx < color_lable_keys.size(); ++idx)
        setLableColor(color_lables[idx],settings->getSetting(color_lable_keys[idx]).value<QColor>()) ;

    // set line edit values
    for (unsigned idx = 0; idx < checkbox_keys.size(); ++idx)
        checkboxs[idx]->setChecked(settings->getSetting(checkbox_keys[idx]).value<bool>()) ;

    // set checkboxs values
    if(!ui->cb_save_videos->isChecked()){
        ui->btn_save_path->setEnabled(false) ;
        ui->le_save_path->setEnabled(false) ;
    }

}

// save setting
void AppSettingsDialog::saveSettings() {
    for (unsigned idx = 0; idx < line_edits_keys.size(); ++idx)
        settings->setSetting(line_edits_keys[idx],line_edits[idx]->text());

    for (unsigned idx = 0; idx < spin_boxs_keys.size(); ++idx)
        settings->setSetting(spin_boxs_keys[idx],spin_boxs[idx]->value());

    for (unsigned idx = 0; idx < color_lable_keys.size(); ++idx)
        settings->setSetting(color_lable_keys[idx],color_lables[idx]->color);

    for (unsigned idx = 0; idx < checkbox_keys.size(); ++idx)
        settings->setSetting(checkbox_keys[idx],checkboxs[idx]->isChecked()) ;

}


void AppSettingsDialog::on_btn_spp_weight_clicked()
{
    QString path = browseFile("Weights Files (*.weights)");
    if(path!="")
        ui->le_spp_weight->setText(path) ;
}

void AppSettingsDialog::on_btn_spp_cfg_clicked()
{
    QString path = browseFile("Configure Files (*.cfg)");
    if(path!="")
        ui->le_spp_cfg->setText(path) ;
}

void AppSettingsDialog::on_btn_tiny_weight_clicked()
{
    QString path = browseFile("Weights Files (*.weights)");
    if(path!="")
        ui->le_tiny_weight->setText(path) ;
}

void AppSettingsDialog::on_btn_tiny_cfg_clicked()
{
    QString path =browseFile("Configure Files (*.cfg)");
    if(path!="")
        ui->le_tiny_cfg->setText(path) ;
}

void AppSettingsDialog::on_btn_windows_weight_clicked()
{
    QString path = browseFile("Weights Files (*.weights)");
    if(path!="")
        ui->le_windows_weight->setText(path) ;
}

void AppSettingsDialog::on_btn_windows_cfg_clicked()
{
    QString path = browseFile("Configure Files (*.cfg)");
    if(path!="")
        ui->le_windows_cfg->setText(path) ;
}

void AppSettingsDialog::on_btn_source1_clicked()
{
    QString path = browseFile();
    if(path!="")
        ui->le_source1->setText(path) ;
}

void AppSettingsDialog::on_btn_source2_clicked()
{
    QString path = browseFile();
    if(path!="")
        ui->le_source2->setText(path) ;
}

void AppSettingsDialog::on_btn_save_path_clicked()
{
    QString path = browsePath();
    if(path!="")
        ui->le_save_path->setText(path) ;
}


void AppSettingsDialog::on_buttonBox_accepted()
{
    saveSettings();
}


void AppSettingsDialog::color_car_licked(){
    QColor color = QColorDialog::getColor(ui->label_color_car->color,this);
    if(color.isValid())
        setLableColor(ui->label_color_car,color) ;

}
void AppSettingsDialog::color_occupant_label1clicked(){
    QColor color = QColorDialog::getColor(ui->label_color_occupant->color,this);
    if(color.isValid())
        setLableColor(ui->label_color_occupant,color) ;

}
void AppSettingsDialog::color_window_label1clicked(){
    QColor color = QColorDialog::getColor(ui->label_color_window->color,this);
    if(color.isValid())
        setLableColor(ui->label_color_window,color) ;

}

void AppSettingsDialog::color_window_back_label1clicked(){
    QColor color = QColorDialog::getColor(ui->label_color_window_back->color,this);
    if(color.isValid())
        setLableColor(ui->label_color_window_back,color) ;

}


void AppSettingsDialog::on_cb_save_videos_stateChanged(int arg1)
{
    ui->le_save_path->setEnabled(ui->cb_save_videos->isChecked()) ;
    ui->btn_save_path->setEnabled(ui->cb_save_videos->isChecked()) ;
    settings->setSetting(settings->KEY_SAVE_VIDEOS,ui->cb_save_videos->isChecked()) ;
}

void AppSettingsDialog::on_cb_driver_detection_stateChanged(int arg1)
{
    settings->setSetting(settings->KEY_DETECT_DRIVER,ui->cb_driver_detection->isChecked()) ;
}
