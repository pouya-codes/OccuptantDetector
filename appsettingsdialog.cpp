#include "appsettingsdialog.h"
#include "ui_appsettingsdialog.h"
#include "QFileDialog"
#include <iostream>

AppSettingsDialog::AppSettingsDialog(AppSettings& settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsDialog)
{
    ui->setupUi(this);
    this->settings = &settings;



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

void AppSettingsDialog::loadSettings() {

    line_edits = { ui->le_source1, ui->le_source2 ,
                                ui->le_spp_cfg , ui->le_spp_weight,
                                ui->le_tiny_cfg, ui->le_tiny_weight,
                                ui->le_windows_cfg, ui->le_windows_weight } ;

    line_edits_keys = {settings->KEY_SOURCE_1, settings->KEY_SOURCE_2,
                      settings->KEY_OCCUPANT_CFG, settings->KEY_OCCUPANT_WEIGHTS,
                      settings->KEY_TINY_CFG, settings->KEY_TINY_WEIGHTS,
                      settings->KEY_WINDOWS_CFG, settings->KEY_WINDOWS_WEIGHTS} ;

    spin_boxs = {ui->sb_car_spp,ui->sb_car_tiny,
                 ui->sb_occupant,ui->sb_windows};

    spin_boxs_keys = {settings->KEY_CAR_THREADSHOLD,settings->KEY_TINY_CAR_THREADSHOLD,
                     settings->KEY_OCCUPANT_THREADSHOLD,settings->KEY_WINDOW_THREADSHOLD};



    for (unsigned idx = 0; idx < line_edits_keys.size(); ++idx)
        line_edits[idx]->setText(settings->getSetting(line_edits_keys[idx]).toString()) ;


    for (unsigned idx = 0; idx < spin_boxs_keys.size(); ++idx)
        spin_boxs[idx]->setValue(settings->getSetting(spin_boxs_keys[idx]).toDouble()) ;

}

void AppSettingsDialog::saveSettings() {
    for (unsigned idx = 0; idx < line_edits_keys.size(); ++idx)
        settings->setSetting(line_edits_keys[idx],line_edits[idx]->text());

    for (unsigned idx = 0; idx < spin_boxs_keys.size(); ++idx)
        settings->setSetting(spin_boxs_keys[idx],spin_boxs[idx]->value());

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

void AppSettingsDialog::on_label_14_linkActivated(const QString &link)
{
    std::cout << "salam" << std::endl ;
}
