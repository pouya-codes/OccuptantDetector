#ifndef APPSETTINGSDIALOG_H
#define APPSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <appsettings.h>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <clickablelabel.h>
#include <QCheckBox>

namespace Ui {
class AppSettingsDialog;
}

class AppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettingsDialog(AppSettings& settings,QWidget *parent = nullptr);
    ~AppSettingsDialog();

private slots:
    void on_btn_spp_weight_clicked();

    void on_btn_spp_cfg_clicked();

    void on_btn_tiny_weight_clicked();

    void on_btn_tiny_cfg_clicked();

    void on_btn_windows_weight_clicked();

    void on_btn_windows_cfg_clicked();

    void on_btn_source1_clicked();

    void on_btn_source2_clicked();

    void on_btn_save_path_clicked();

    void on_buttonBox_accepted();


    void color_car_licked();
    void color_occupant_label1clicked();
    void color_window_label1clicked();
    void color_window_back_label1clicked();



    void on_cb_save_videos_stateChanged(int arg1);

    void on_cb_driver_detection_stateChanged(int arg1);

private:
    std::vector<QString> line_edits_keys ;
    std::vector<QLineEdit*> line_edits;
    std::vector<QString> spin_boxs_keys;
    std::vector<QDoubleSpinBox*> spin_boxs;
    std::vector<QString> color_lable_keys;
    std::vector<ClickableLabel*> color_lables;
    std::vector<QString> checkbox_keys;
    std::vector<QCheckBox*> checkboxs;

    AppSettings* settings ;
    QString browseFile(QString selectFilter = "All Files (*.*)") ;
    QString browsePath() ;
    void loadSettings();
    void saveSettings();
    void setLableColor (ClickableLabel* lable, QColor color) ;
    QString settingsFile;
    Ui::AppSettingsDialog *ui;
};

#endif // APPSETTINGS_H
