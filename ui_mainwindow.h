/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "clickablelabel.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    ClickableLabel *lable_front_image;
    ClickableLabel *lable_back_image;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *pb_save_images;
    QRadioButton *rb_orginal;
    QRadioButton *rb_processed;
    QVBoxLayout *verticalLayout_3;
    QComboBox *comboBox_date;
    QTableView *tableView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1014, 754);
        MainWindow->setLayoutDirection(Qt::RightToLeft);
        MainWindow->setLocale(QLocale(QLocale::Persian, QLocale::Iran));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lable_front_image = new ClickableLabel(centralWidget);
        lable_front_image->setObjectName(QStringLiteral("lable_front_image"));
        lable_front_image->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lable_front_image->sizePolicy().hasHeightForWidth());
        lable_front_image->setSizePolicy(sizePolicy);
        lable_front_image->setStyleSheet(QStringLiteral("border: 2px solid black"));

        verticalLayout_2->addWidget(lable_front_image);

        lable_back_image = new ClickableLabel(centralWidget);
        lable_back_image->setObjectName(QStringLiteral("lable_back_image"));
        lable_back_image->setEnabled(true);
        lable_back_image->setStyleSheet(QStringLiteral("border: 2px solid black"));

        verticalLayout_2->addWidget(lable_back_image);


        gridLayout->addLayout(verticalLayout_2, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_Stop = new QPushButton(centralWidget);
        pushButton_Stop->setObjectName(QStringLiteral("pushButton_Stop"));

        horizontalLayout_2->addWidget(pushButton_Stop);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout_2, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pb_save_images = new QPushButton(centralWidget);
        pb_save_images->setObjectName(QStringLiteral("pb_save_images"));
        pb_save_images->setEnabled(false);

        horizontalLayout->addWidget(pb_save_images);

        rb_orginal = new QRadioButton(centralWidget);
        rb_orginal->setObjectName(QStringLiteral("rb_orginal"));
        rb_orginal->setLayoutDirection(Qt::RightToLeft);
        rb_orginal->setChecked(true);

        horizontalLayout->addWidget(rb_orginal);

        rb_processed = new QRadioButton(centralWidget);
        rb_processed->setObjectName(QStringLiteral("rb_processed"));
        rb_processed->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(rb_processed);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        comboBox_date = new QComboBox(centralWidget);
        comboBox_date->setObjectName(QStringLiteral("comboBox_date"));

        verticalLayout_3->addWidget(comboBox_date);

        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(tableView);


        gridLayout->addLayout(verticalLayout_3, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1014, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\331\206\330\261\331\205 \330\247\331\201\330\262\330\247\330\261 \330\264\331\205\330\247\330\261\330\264\332\257\330\261 \330\252\330\271\330\257\330\247\330\257 \330\263\330\261\331\206\330\264\333\214\331\206 \331\207\330\247\333\214 \330\256\331\210\330\257\330\261\331\210", nullptr));
        lable_front_image->setText(QString());
        lable_back_image->setText(QString());
        pushButton_Stop->setText(QApplication::translate("MainWindow", "\330\252\331\206\330\270\333\214\331\205\330\247\330\252", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\330\247\330\254\330\261\330\247", nullptr));
        pb_save_images->setText(QApplication::translate("MainWindow", "\330\260\330\256\333\214\330\261\331\207 \330\252\330\265\330\247\331\210\333\214\330\261", nullptr));
        rb_orginal->setText(QApplication::translate("MainWindow", "\331\206\331\205\330\247\333\214\330\264 \330\252\330\265\331\210\333\214\330\261 \330\256\330\247\331\205", nullptr));
        rb_processed->setText(QApplication::translate("MainWindow", "\331\206\331\205\330\247\333\214\330\264 \330\252\330\265\331\210\333\214\330\261 \331\276\330\261\330\257\330\247\330\262\330\264 \330\264\330\257\331\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
