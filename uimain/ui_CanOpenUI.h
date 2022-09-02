/********************************************************************************
** Form generated from reading UI file 'CanOpenUI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANOPENUI_H
#define UI_CANOPENUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanOpenUI
{
public:
    QWidget *centralwidget;
    QTextEdit *ModBusView;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextBrowser *textBrowser;
    QLineEdit *CanIP;
    QLineEdit *CanPort;
    QPushButton *CanConn;
    QLabel *label_6;
    QLineEdit *ModPort;
    QPushButton *ModConn;
    QPushButton *sendCan;
    QWidget *widget_waterfall_plot;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CanOpenUI)
    {
        if (CanOpenUI->objectName().isEmpty())
            CanOpenUI->setObjectName(QString::fromUtf8("CanOpenUI"));
        CanOpenUI->resize(800, 712);
        centralwidget = new QWidget(CanOpenUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ModBusView = new QTextEdit(centralwidget);
        ModBusView->setObjectName(QString::fromUtf8("ModBusView"));
        ModBusView->setGeometry(QRect(50, 70, 341, 191));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(130, 10, 121, 41));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(530, 20, 131, 41));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(470, 270, 91, 20));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(670, 270, 41, 16));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(430, 70, 341, 192));
        CanIP = new QLineEdit(centralwidget);
        CanIP->setObjectName(QString::fromUtf8("CanIP"));
        CanIP->setGeometry(QRect(580, 270, 81, 21));
        CanPort = new QLineEdit(centralwidget);
        CanPort->setObjectName(QString::fromUtf8("CanPort"));
        CanPort->setGeometry(QRect(700, 270, 41, 21));
        CanConn = new QPushButton(centralwidget);
        CanConn->setObjectName(QString::fromUtf8("CanConn"));
        CanConn->setGeometry(QRect(460, 300, 281, 24));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(100, 270, 141, 20));
        ModPort = new QLineEdit(centralwidget);
        ModPort->setObjectName(QString::fromUtf8("ModPort"));
        ModPort->setGeometry(QRect(240, 270, 41, 21));
        ModConn = new QPushButton(centralwidget);
        ModConn->setObjectName(QString::fromUtf8("ModConn"));
        ModConn->setGeometry(QRect(100, 300, 201, 24));
        sendCan = new QPushButton(centralwidget);
        sendCan->setObjectName(QString::fromUtf8("sendCan"));
        sendCan->setGeometry(QRect(460, 330, 75, 24));
        widget_waterfall_plot = new QWidget(centralwidget);
        widget_waterfall_plot->setObjectName(QString::fromUtf8("widget_waterfall_plot"));
        widget_waterfall_plot->setGeometry(QRect(80, 400, 650, 240));
        CanOpenUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CanOpenUI);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        CanOpenUI->setMenuBar(menubar);
        statusbar = new QStatusBar(CanOpenUI);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CanOpenUI->setStatusBar(statusbar);

        retranslateUi(CanOpenUI);

        QMetaObject::connectSlotsByName(CanOpenUI);
    } // setupUi

    void retranslateUi(QMainWindow *CanOpenUI)
    {
        CanOpenUI->setWindowTitle(QApplication::translate("CanOpenUI", "MainWindow", nullptr));
        label->setText(QApplication::translate("CanOpenUI", "<html><head/><body><p><span style=\" font-size:20pt;\">\345\215\217\350\256\256\346\216\245\346\224\266</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("CanOpenUI", "<html><head/><body><p><span style=\" font-size:20pt;\">\345\217\221\351\200\201\346\214\207\344\273\244</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("CanOpenUI", "CanOpen  IP:", nullptr));
        label_4->setText(QApplication::translate("CanOpenUI", "Port:", nullptr));
        CanIP->setText(QApplication::translate("CanOpenUI", "127.0.0.1", nullptr));
        CanPort->setText(QApplication::translate("CanOpenUI", "502", nullptr));
        CanConn->setText(QApplication::translate("CanOpenUI", "\350\277\236\346\216\245", nullptr));
        label_6->setText(QApplication::translate("CanOpenUI", "Modbus Master Port:", nullptr));
        ModPort->setText(QApplication::translate("CanOpenUI", "502", nullptr));
        ModConn->setText(QApplication::translate("CanOpenUI", "\345\274\200\345\220\257\345\215\217\350\256\256\346\216\245\346\224\266", nullptr));
        sendCan->setText(QApplication::translate("CanOpenUI", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanOpenUI: public Ui_CanOpenUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANOPENUI_H
