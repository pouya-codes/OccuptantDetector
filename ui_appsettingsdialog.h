/********************************************************************************
** Form generated from reading UI file 'appsettingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPSETTINGSDIALOG_H
#define UI_APPSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "clickablelabel.h"

QT_BEGIN_NAMESPACE

class Ui_AppSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QPushButton *btn_source1;
    QLineEdit *le_source1;
    QLabel *label_8;
    QLineEdit *le_source2;
    QPushButton *btn_source2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_11;
    QDoubleSpinBox *sb_windows;
    QDoubleSpinBox *sb_car_tiny;
    QDoubleSpinBox *sb_occupant;
    QCheckBox *cb_use_gpu;
    QDoubleSpinBox *sb_car_spp;
    QLabel *label_9;
    QLabel *label_10;
    QCheckBox *cb_driver_detection;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_2;
    ClickableLabel *label_color_occupant;
    QLabel *label_16;
    QHBoxLayout *horizontalLayout;
    ClickableLabel *label_color_car;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_3;
    ClickableLabel *label_color_window;
    QLabel *label_17;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    ClickableLabel *label_color_window_back;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *cb_show_time;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPushButton *btn_tiny_cfg;
    QLabel *label_2;
    QLabel *label_5;
    QPushButton *btn_windows_weight;
    QLabel *label_6;
    QLabel *label_3;
    QPushButton *btn_spp_cfg;
    QLineEdit *le_windows_weight;
    QPushButton *btn_windows_cfg;
    QLabel *label;
    QLineEdit *le_spp_weight;
    QLineEdit *le_tiny_cfg;
    QLineEdit *le_spp_cfg;
    QLabel *label_4;
    QPushButton *btn_spp_weight;
    QLineEdit *le_tiny_weight;
    QPushButton *btn_tiny_weight;
    QLineEdit *le_windows_cfg;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AppSettingsDialog)
    {
        if (AppSettingsDialog->objectName().isEmpty())
            AppSettingsDialog->setObjectName(QStringLiteral("AppSettingsDialog"));
        AppSettingsDialog->resize(733, 553);
        verticalLayout = new QVBoxLayout(AppSettingsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_2 = new QGroupBox(AppSettingsDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 0, 2, 1, 1);

        btn_source1 = new QPushButton(groupBox_2);
        btn_source1->setObjectName(QStringLiteral("btn_source1"));

        gridLayout->addWidget(btn_source1, 0, 0, 1, 1);

        le_source1 = new QLineEdit(groupBox_2);
        le_source1->setObjectName(QStringLiteral("le_source1"));

        gridLayout->addWidget(le_source1, 0, 1, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 1, 2, 1, 1);

        le_source2 = new QLineEdit(groupBox_2);
        le_source2->setObjectName(QStringLiteral("le_source2"));

        gridLayout->addWidget(le_source2, 1, 1, 1, 1);

        btn_source2 = new QPushButton(groupBox_2);
        btn_source2->setObjectName(QStringLiteral("btn_source2"));

        gridLayout->addWidget(btn_source2, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(AppSettingsDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_3->addWidget(label_11, 1, 1, 1, 1);

        sb_windows = new QDoubleSpinBox(groupBox_3);
        sb_windows->setObjectName(QStringLiteral("sb_windows"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sb_windows->sizePolicy().hasHeightForWidth());
        sb_windows->setSizePolicy(sizePolicy);
        sb_windows->setMaximum(1);
        sb_windows->setSingleStep(0.05);

        gridLayout_3->addWidget(sb_windows, 0, 2, 1, 1);

        sb_car_tiny = new QDoubleSpinBox(groupBox_3);
        sb_car_tiny->setObjectName(QStringLiteral("sb_car_tiny"));
        sizePolicy.setHeightForWidth(sb_car_tiny->sizePolicy().hasHeightForWidth());
        sb_car_tiny->setSizePolicy(sizePolicy);
        sb_car_tiny->setMaximum(1);
        sb_car_tiny->setSingleStep(0.05);

        gridLayout_3->addWidget(sb_car_tiny, 0, 0, 1, 1);

        sb_occupant = new QDoubleSpinBox(groupBox_3);
        sb_occupant->setObjectName(QStringLiteral("sb_occupant"));
        sb_occupant->setMaximum(1);
        sb_occupant->setSingleStep(0.05);

        gridLayout_3->addWidget(sb_occupant, 1, 2, 1, 1);

        cb_use_gpu = new QCheckBox(groupBox_3);
        cb_use_gpu->setObjectName(QStringLiteral("cb_use_gpu"));
        cb_use_gpu->setEnabled(true);
        cb_use_gpu->setLayoutDirection(Qt::RightToLeft);

        gridLayout_3->addWidget(cb_use_gpu, 2, 2, 1, 1);

        sb_car_spp = new QDoubleSpinBox(groupBox_3);
        sb_car_spp->setObjectName(QStringLiteral("sb_car_spp"));
        sb_car_spp->setMaximum(1);
        sb_car_spp->setSingleStep(0.05);

        gridLayout_3->addWidget(sb_car_spp, 1, 0, 1, 1);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_3->addWidget(label_9, 0, 1, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(label_10, 0, 3, 1, 1);

        cb_driver_detection = new QCheckBox(groupBox_3);
        cb_driver_detection->setObjectName(QStringLiteral("cb_driver_detection"));
        cb_driver_detection->setLayoutDirection(Qt::RightToLeft);

        gridLayout_3->addWidget(cb_driver_detection, 2, 3, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_3->addWidget(label_12, 1, 3, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_color_occupant = new ClickableLabel(groupBox_3);
        label_color_occupant->setObjectName(QStringLiteral("label_color_occupant"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_color_occupant->sizePolicy().hasHeightForWidth());
        label_color_occupant->setSizePolicy(sizePolicy2);
        label_color_occupant->setMinimumSize(QSize(50, 0));

        horizontalLayout_2->addWidget(label_color_occupant);

        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_2->addWidget(label_16);


        gridLayout_3->addLayout(horizontalLayout_2, 3, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_color_car = new ClickableLabel(groupBox_3);
        label_color_car->setObjectName(QStringLiteral("label_color_car"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(50);
        sizePolicy3.setVerticalStretch(50);
        sizePolicy3.setHeightForWidth(label_color_car->sizePolicy().hasHeightForWidth());
        label_color_car->setSizePolicy(sizePolicy3);
        label_color_car->setMinimumSize(QSize(50, 0));

        horizontalLayout->addWidget(label_color_car);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout->addWidget(label_14);


        gridLayout_3->addLayout(horizontalLayout, 3, 3, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_color_window = new ClickableLabel(groupBox_3);
        label_color_window->setObjectName(QStringLiteral("label_color_window"));
        sizePolicy2.setHeightForWidth(label_color_window->sizePolicy().hasHeightForWidth());
        label_color_window->setSizePolicy(sizePolicy2);
        label_color_window->setMinimumSize(QSize(50, 0));

        horizontalLayout_3->addWidget(label_color_window);

        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_3->addWidget(label_17);


        gridLayout_3->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_color_window_back = new ClickableLabel(groupBox_3);
        label_color_window_back->setObjectName(QStringLiteral("label_color_window_back"));

        horizontalLayout_5->addWidget(label_color_window_back);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_5->addWidget(label_13);


        horizontalLayout_4->addLayout(horizontalLayout_5);


        gridLayout_3->addLayout(horizontalLayout_4, 3, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        cb_show_time = new QCheckBox(groupBox_3);
        cb_show_time->setObjectName(QStringLiteral("cb_show_time"));
        cb_show_time->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_6->addWidget(cb_show_time);


        gridLayout_3->addLayout(horizontalLayout_6, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        groupBox = new QGroupBox(AppSettingsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy4);
        groupBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        btn_tiny_cfg = new QPushButton(groupBox);
        btn_tiny_cfg->setObjectName(QStringLiteral("btn_tiny_cfg"));

        gridLayout_2->addWidget(btn_tiny_cfg, 3, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 1, 2, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 4, 2, 1, 1);

        btn_windows_weight = new QPushButton(groupBox);
        btn_windows_weight->setObjectName(QStringLiteral("btn_windows_weight"));

        gridLayout_2->addWidget(btn_windows_weight, 4, 0, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 5, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 2, 2, 1, 1);

        btn_spp_cfg = new QPushButton(groupBox);
        btn_spp_cfg->setObjectName(QStringLiteral("btn_spp_cfg"));

        gridLayout_2->addWidget(btn_spp_cfg, 1, 0, 1, 1);

        le_windows_weight = new QLineEdit(groupBox);
        le_windows_weight->setObjectName(QStringLiteral("le_windows_weight"));

        gridLayout_2->addWidget(le_windows_weight, 4, 1, 1, 1);

        btn_windows_cfg = new QPushButton(groupBox);
        btn_windows_cfg->setObjectName(QStringLiteral("btn_windows_cfg"));

        gridLayout_2->addWidget(btn_windows_cfg, 5, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 0, 2, 1, 1);

        le_spp_weight = new QLineEdit(groupBox);
        le_spp_weight->setObjectName(QStringLiteral("le_spp_weight"));

        gridLayout_2->addWidget(le_spp_weight, 0, 1, 1, 1);

        le_tiny_cfg = new QLineEdit(groupBox);
        le_tiny_cfg->setObjectName(QStringLiteral("le_tiny_cfg"));

        gridLayout_2->addWidget(le_tiny_cfg, 3, 1, 1, 1);

        le_spp_cfg = new QLineEdit(groupBox);
        le_spp_cfg->setObjectName(QStringLiteral("le_spp_cfg"));

        gridLayout_2->addWidget(le_spp_cfg, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 3, 2, 1, 1);

        btn_spp_weight = new QPushButton(groupBox);
        btn_spp_weight->setObjectName(QStringLiteral("btn_spp_weight"));

        gridLayout_2->addWidget(btn_spp_weight, 0, 0, 1, 1);

        le_tiny_weight = new QLineEdit(groupBox);
        le_tiny_weight->setObjectName(QStringLiteral("le_tiny_weight"));

        gridLayout_2->addWidget(le_tiny_weight, 2, 1, 1, 1);

        btn_tiny_weight = new QPushButton(groupBox);
        btn_tiny_weight->setObjectName(QStringLiteral("btn_tiny_weight"));

        gridLayout_2->addWidget(btn_tiny_weight, 2, 0, 1, 1);

        le_windows_cfg = new QLineEdit(groupBox);
        le_windows_cfg->setObjectName(QStringLiteral("le_windows_cfg"));

        gridLayout_2->addWidget(le_windows_cfg, 5, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(AppSettingsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AppSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AppSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AppSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AppSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *AppSettingsDialog)
    {
        AppSettingsDialog->setWindowTitle(QApplication::translate("AppSettingsDialog", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252", nullptr));
        groupBox_2->setTitle(QApplication::translate("AppSettingsDialog", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252 \331\210\330\261\331\210\330\257\333\214", nullptr));
        label_7->setText(QApplication::translate("AppSettingsDialog", "\330\257\331\210\330\261\330\250\333\214\331\206 \330\247\331\210\331\204", nullptr));
        btn_source1->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        label_8->setText(QApplication::translate("AppSettingsDialog", "\330\257\331\210\330\261\330\250\333\214\331\206 \330\257\331\210\331\205", nullptr));
        btn_source2->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        groupBox_3->setTitle(QApplication::translate("AppSettingsDialog", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252 \331\206\330\261\331\205 \330\247\331\201\330\262\330\247\330\261", nullptr));
        label_11->setText(QApplication::translate("AppSettingsDialog", "\330\255\330\263\330\247\333\214\330\252 \330\252\330\264\330\256\333\214\330\265 \331\205\330\247\330\264\333\214\331\206 \331\207\330\247", nullptr));
        cb_use_gpu->setText(QApplication::translate("AppSettingsDialog", "\330\257\333\214\332\251\330\257 \330\252\330\265\331\210\333\214\330\261 \330\250\330\247 \332\251\330\247\330\261\330\252 \332\257\330\261\330\247\331\201\333\214\332\251", nullptr));
        label_9->setText(QApplication::translate("AppSettingsDialog", "\330\255\330\263\330\247\333\214\330\252 \331\210\330\254\331\210\330\257 \331\205\330\247\330\264\333\214\331\206", nullptr));
        label_10->setText(QApplication::translate("AppSettingsDialog", "\330\255\330\263\330\247\330\263\333\214\330\252 \330\252\330\264\330\256\333\214\330\265 \331\276\331\206\330\254\330\261\331\207 \331\207\330\247", nullptr));
        cb_driver_detection->setText(QApplication::translate("AppSettingsDialog", "\330\252\330\264\330\256\333\214\330\265 \330\261\330\247\331\206\331\206\330\257\331\207", nullptr));
        label_12->setText(QApplication::translate("AppSettingsDialog", "\330\255\330\263\330\247\333\214\330\263\330\252 \330\252\330\264\330\256\333\214\330\265 \330\263\330\261\331\206\330\264\333\214\331\206 \331\207\330\247", nullptr));
        label_color_occupant->setText(QString());
        label_16->setText(QApplication::translate("AppSettingsDialog", "\330\261\331\206\332\257 \331\206\330\247\330\255\333\214\331\207 \330\263\330\261\331\206\330\264\333\214\331\206", nullptr));
        label_color_car->setText(QString());
        label_14->setText(QApplication::translate("AppSettingsDialog", "\330\261\331\206\332\257 \331\206\330\247\330\255\333\214\331\207 \331\205\330\247\330\264\333\214\331\206", nullptr));
        label_color_window->setText(QString());
        label_17->setText(QApplication::translate("AppSettingsDialog", "\330\261\331\206\332\257 \331\206\330\247\330\255\333\214\331\207 \331\276\331\206\330\254\330\261\331\207 \330\254\331\204\331\210", nullptr));
        label_color_window_back->setText(QString());
        label_13->setText(QApplication::translate("AppSettingsDialog", "\330\261\331\206\332\257 \331\206\330\247\330\255\333\214\331\207 \331\276\331\206\330\254\330\261\331\207 \330\271\331\202\330\250", nullptr));
        cb_show_time->setText(QApplication::translate("AppSettingsDialog", "\331\206\331\205\330\247\333\214\330\264 \330\262\331\205\330\247\331\206 \331\276\330\261\330\257\330\247\330\262\330\264", nullptr));
        groupBox->setTitle(QApplication::translate("AppSettingsDialog", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252 \331\205\330\257\331\204 \331\207\330\247", nullptr));
        btn_tiny_cfg->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        label_2->setText(QApplication::translate("AppSettingsDialog", "\331\205\330\271\331\205\330\247\330\261\333\214 \331\205\330\257\331\204 \330\252\330\264\330\256\333\214\330\265 \330\263\330\261\331\206\330\264\333\214\331\206", nullptr));
        label_5->setText(QApplication::translate("AppSettingsDialog", "\331\210\330\262\331\206 \331\205\330\257\331\204 \330\252\330\264\330\256\333\214\330\265 \331\276\331\206\330\254\330\261\331\207 \331\207\330\247", nullptr));
        btn_windows_weight->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        label_6->setText(QApplication::translate("AppSettingsDialog", "\331\205\330\271\331\205\330\247\330\261\333\214 \331\205\330\257\331\204 \330\252\330\264\330\256\333\214\330\265 \331\276\331\206\330\254\330\261\331\207 \331\207\330\247", nullptr));
        label_3->setText(QApplication::translate("AppSettingsDialog", "\331\210\330\262\331\206 \331\205\330\257\331\204 \330\252\330\264\330\256\333\214\330\265 \331\205\330\247\330\264\333\214\331\206", nullptr));
        btn_spp_cfg->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        btn_windows_cfg->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        label->setText(QApplication::translate("AppSettingsDialog", "\331\210\330\262\331\206 \331\205\330\257\331\204 \330\252\330\264\330\256\333\214\330\265 \330\263\330\261\331\206\330\264\333\214\331\206", nullptr));
        label_4->setText(QApplication::translate("AppSettingsDialog", "\331\205\330\271\331\205\330\247\330\261\333\214 \331\205\330\257\331\204 \330\252\330\264\330\256\333\214\330\265 \331\205\330\247\330\264\333\214\331\206", nullptr));
        btn_spp_weight->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
        btn_tiny_weight->setText(QApplication::translate("AppSettingsDialog", "\330\247\331\206\330\252\330\256\330\247\330\250 \331\201\330\247\333\214\331\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppSettingsDialog: public Ui_AppSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPSETTINGSDIALOG_H
