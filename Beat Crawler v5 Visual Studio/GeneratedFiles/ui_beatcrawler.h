/********************************************************************************
** Form generated from reading UI file 'beatcrawler.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEATCRAWLER_H
#define UI_BEATCRAWLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "beatcrawler.h"

QT_BEGIN_NAMESPACE

class Ui_BeatCrawler
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit_keywords_search_box;
    QComboBox *comboBox_search_results_amount;
    QLabel *label_search_results_amount;
    QLabel *label_search_engine;
    QLabel *label_search_engine_keywords;
    QFrame *line;
    QPushButton *pushButton_Start;
    QTabWidget *tabWidget_Global;
    QWidget *tab_3;
    QTableWidget *tableWidget_Emails;
    QPushButton *pushButton_Save_Emails;
    QPushButton *pushButton_Next_Email_Pagination;
    QPushButton *pushButton_Previous_Email_Pagination;
    QCheckBox *checkBox_Delete_Emails;
    QWidget *tab;
    QCheckBox *checkBox_Delete_Keywords;
    QTableWidget *tableWidget_Keywords_Queue;
    QWidget *tab_4;
    QTabWidget *tabWidget_Harvester_Options;
    QWidget *tab_6;
    QLabel *label_Search_Engine_Label_Option;
    QListView *listView_Search_Engine_Option;
    QCheckBox *checkBox_Google;
    QCheckBox *checkBox_Bing;
    QCheckBox *checkBox_Yahoo;
    QListView *listView;
    QLabel *label_UserAgent;
    QRadioButton *radioButton_Android_Webkit;
    QRadioButton *radioButton_Blackberry;
    QRadioButton *radioButton_IE_Mobile;
    QWidget *tab_7;
    QLabel *label_Email_Provider_Option_Label;
    QListView *listView_Email_Provider_Option_Label;
    QCheckBox *checkBox_Email_Gmail;
    QCheckBox *checkBox_Email_Yahoo;
    QCheckBox *checkBox_Email_Hotmail;
    QWidget *tab_8;
    QListView *listView_Social_Networks_Option;
    QLabel *label_Social_Networks_Option_Label;
    QCheckBox *checkBox_Social_Instagram;
    QCheckBox *checkBox_Social_Facebook;
    QCheckBox *checkBox_Social_Soundcloud;
    QCheckBox *checkBox_Social_Reverbnation;
    QCheckBox *checkBox_Social_Myspace;
    QWidget *tab_2;
    QListView *listView_2;
    QLabel *label_3;
    QSpinBox *spinBox_Harvester_Timer;
    QWidget *tab_5;
    QTableWidget *tableWidget_Proxy;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_Proxy_Host;
    QLineEdit *lineEdit_Proxy_Port;
    QPushButton *pushButton_Add_Proxy;
    QCheckBox *checkBox_Delete_Proxy;
    QFrame *line_2;
    QPushButton *pushButton_Load_Keyword_List;
    QLabel *label_Current_Keyword;
    QLabel *label_Curl_Status;
    QLabel *label_Items_Found;
    QLineEdit *lineEdit_Keyword_List_File_Location;
    QLabel *label_Keywords_Count;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(BeatCrawler *BeatCrawler)
    {
        if (BeatCrawler->objectName().isEmpty())
            BeatCrawler->setObjectName(QStringLiteral("BeatCrawler"));
        BeatCrawler->resize(638, 420);
        centralWidget = new QWidget(BeatCrawler);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setGeometry(QRect(0, 0, 651, 422));
        lineEdit_keywords_search_box = new QLineEdit(centralWidget);
        lineEdit_keywords_search_box->setObjectName(QStringLiteral("lineEdit_keywords_search_box"));
        lineEdit_keywords_search_box->setGeometry(QRect(190, 10, 291, 21));
        comboBox_search_results_amount = new QComboBox(centralWidget);
        comboBox_search_results_amount->setObjectName(QStringLiteral("comboBox_search_results_amount"));
        comboBox_search_results_amount->setGeometry(QRect(190, 40, 72, 22));
        label_search_results_amount = new QLabel(centralWidget);
        label_search_results_amount->setObjectName(QStringLiteral("label_search_results_amount"));
        label_search_results_amount->setGeometry(QRect(20, 40, 171, 16));
        label_search_engine = new QLabel(centralWidget);
        label_search_engine->setObjectName(QStringLiteral("label_search_engine"));
        label_search_engine->setGeometry(QRect(20, 10, 101, 16));
        label_search_engine_keywords = new QLabel(centralWidget);
        label_search_engine_keywords->setObjectName(QStringLiteral("label_search_engine_keywords"));
        label_search_engine_keywords->setGeometry(QRect(130, 10, 61, 16));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(-10, 60, 651, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pushButton_Start = new QPushButton(centralWidget);
        pushButton_Start->setObjectName(QStringLiteral("pushButton_Start"));
        pushButton_Start->setGeometry(QRect(10, 80, 80, 21));
        tabWidget_Global = new QTabWidget(centralWidget);
        tabWidget_Global->setObjectName(QStringLiteral("tabWidget_Global"));
        tabWidget_Global->setGeometry(QRect(10, 120, 621, 271));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        tableWidget_Emails = new QTableWidget(tab_3);
        tableWidget_Emails->setObjectName(QStringLiteral("tableWidget_Emails"));
        tableWidget_Emails->setGeometry(QRect(10, 10, 591, 201));
        pushButton_Save_Emails = new QPushButton(tab_3);
        pushButton_Save_Emails->setObjectName(QStringLiteral("pushButton_Save_Emails"));
        pushButton_Save_Emails->setGeometry(QRect(10, 220, 61, 21));
        pushButton_Next_Email_Pagination = new QPushButton(tab_3);
        pushButton_Next_Email_Pagination->setObjectName(QStringLiteral("pushButton_Next_Email_Pagination"));
        pushButton_Next_Email_Pagination->setGeometry(QRect(300, 220, 61, 21));
        pushButton_Previous_Email_Pagination = new QPushButton(tab_3);
        pushButton_Previous_Email_Pagination->setObjectName(QStringLiteral("pushButton_Previous_Email_Pagination"));
        pushButton_Previous_Email_Pagination->setGeometry(QRect(231, 220, 61, 21));
        checkBox_Delete_Emails = new QCheckBox(tab_3);
        checkBox_Delete_Emails->setObjectName(QStringLiteral("checkBox_Delete_Emails"));
        checkBox_Delete_Emails->setGeometry(QRect(510, 220, 91, 20));
        tabWidget_Global->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        checkBox_Delete_Keywords = new QCheckBox(tab);
        checkBox_Delete_Keywords->setObjectName(QStringLiteral("checkBox_Delete_Keywords"));
        checkBox_Delete_Keywords->setGeometry(QRect(450, 220, 161, 20));
        tableWidget_Keywords_Queue = new QTableWidget(tab);
        tableWidget_Keywords_Queue->setObjectName(QStringLiteral("tableWidget_Keywords_Queue"));
        tableWidget_Keywords_Queue->setGeometry(QRect(0, 0, 621, 211));
        tabWidget_Global->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tabWidget_Harvester_Options = new QTabWidget(tab_4);
        tabWidget_Harvester_Options->setObjectName(QStringLiteral("tabWidget_Harvester_Options"));
        tabWidget_Harvester_Options->setGeometry(QRect(10, 10, 601, 231));
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        label_Search_Engine_Label_Option = new QLabel(tab_6);
        label_Search_Engine_Label_Option->setObjectName(QStringLiteral("label_Search_Engine_Label_Option"));
        label_Search_Engine_Label_Option->setGeometry(QRect(10, 10, 181, 16));
        listView_Search_Engine_Option = new QListView(tab_6);
        listView_Search_Engine_Option->setObjectName(QStringLiteral("listView_Search_Engine_Option"));
        listView_Search_Engine_Option->setGeometry(QRect(10, 30, 221, 141));
        checkBox_Google = new QCheckBox(tab_6);
        checkBox_Google->setObjectName(QStringLiteral("checkBox_Google"));
        checkBox_Google->setGeometry(QRect(20, 40, 72, 19));
        checkBox_Bing = new QCheckBox(tab_6);
        checkBox_Bing->setObjectName(QStringLiteral("checkBox_Bing"));
        checkBox_Bing->setGeometry(QRect(20, 60, 72, 19));
        checkBox_Yahoo = new QCheckBox(tab_6);
        checkBox_Yahoo->setObjectName(QStringLiteral("checkBox_Yahoo"));
        checkBox_Yahoo->setGeometry(QRect(20, 80, 72, 19));
        listView = new QListView(tab_6);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(280, 30, 221, 141));
        label_UserAgent = new QLabel(tab_6);
        label_UserAgent->setObjectName(QStringLiteral("label_UserAgent"));
        label_UserAgent->setGeometry(QRect(280, 10, 181, 16));
        radioButton_Android_Webkit = new QRadioButton(tab_6);
        radioButton_Android_Webkit->setObjectName(QStringLiteral("radioButton_Android_Webkit"));
        radioButton_Android_Webkit->setGeometry(QRect(290, 40, 111, 19));
        radioButton_Blackberry = new QRadioButton(tab_6);
        radioButton_Blackberry->setObjectName(QStringLiteral("radioButton_Blackberry"));
        radioButton_Blackberry->setGeometry(QRect(290, 60, 84, 19));
        radioButton_IE_Mobile = new QRadioButton(tab_6);
        radioButton_IE_Mobile->setObjectName(QStringLiteral("radioButton_IE_Mobile"));
        radioButton_IE_Mobile->setGeometry(QRect(290, 80, 84, 19));
        tabWidget_Harvester_Options->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        label_Email_Provider_Option_Label = new QLabel(tab_7);
        label_Email_Provider_Option_Label->setObjectName(QStringLiteral("label_Email_Provider_Option_Label"));
        label_Email_Provider_Option_Label->setGeometry(QRect(10, 10, 181, 16));
        listView_Email_Provider_Option_Label = new QListView(tab_7);
        listView_Email_Provider_Option_Label->setObjectName(QStringLiteral("listView_Email_Provider_Option_Label"));
        listView_Email_Provider_Option_Label->setGeometry(QRect(10, 30, 221, 141));
        checkBox_Email_Gmail = new QCheckBox(tab_7);
        checkBox_Email_Gmail->setObjectName(QStringLiteral("checkBox_Email_Gmail"));
        checkBox_Email_Gmail->setGeometry(QRect(20, 40, 72, 19));
        checkBox_Email_Yahoo = new QCheckBox(tab_7);
        checkBox_Email_Yahoo->setObjectName(QStringLiteral("checkBox_Email_Yahoo"));
        checkBox_Email_Yahoo->setGeometry(QRect(20, 60, 72, 19));
        checkBox_Email_Hotmail = new QCheckBox(tab_7);
        checkBox_Email_Hotmail->setObjectName(QStringLiteral("checkBox_Email_Hotmail"));
        checkBox_Email_Hotmail->setGeometry(QRect(20, 80, 72, 19));
        tabWidget_Harvester_Options->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        listView_Social_Networks_Option = new QListView(tab_8);
        listView_Social_Networks_Option->setObjectName(QStringLiteral("listView_Social_Networks_Option"));
        listView_Social_Networks_Option->setGeometry(QRect(10, 30, 221, 141));
        label_Social_Networks_Option_Label = new QLabel(tab_8);
        label_Social_Networks_Option_Label->setObjectName(QStringLiteral("label_Social_Networks_Option_Label"));
        label_Social_Networks_Option_Label->setGeometry(QRect(10, 10, 181, 16));
        checkBox_Social_Instagram = new QCheckBox(tab_8);
        checkBox_Social_Instagram->setObjectName(QStringLiteral("checkBox_Social_Instagram"));
        checkBox_Social_Instagram->setGeometry(QRect(20, 40, 72, 19));
        checkBox_Social_Facebook = new QCheckBox(tab_8);
        checkBox_Social_Facebook->setObjectName(QStringLiteral("checkBox_Social_Facebook"));
        checkBox_Social_Facebook->setGeometry(QRect(20, 60, 72, 19));
        checkBox_Social_Soundcloud = new QCheckBox(tab_8);
        checkBox_Social_Soundcloud->setObjectName(QStringLiteral("checkBox_Social_Soundcloud"));
        checkBox_Social_Soundcloud->setGeometry(QRect(20, 80, 81, 19));
        checkBox_Social_Reverbnation = new QCheckBox(tab_8);
        checkBox_Social_Reverbnation->setObjectName(QStringLiteral("checkBox_Social_Reverbnation"));
        checkBox_Social_Reverbnation->setGeometry(QRect(20, 100, 91, 19));
        checkBox_Social_Myspace = new QCheckBox(tab_8);
        checkBox_Social_Myspace->setObjectName(QStringLiteral("checkBox_Social_Myspace"));
        checkBox_Social_Myspace->setGeometry(QRect(20, 120, 72, 19));
        tabWidget_Harvester_Options->addTab(tab_8, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        listView_2 = new QListView(tab_2);
        listView_2->setObjectName(QStringLiteral("listView_2"));
        listView_2->setGeometry(QRect(10, 30, 131, 51));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 10, 141, 20));
        spinBox_Harvester_Timer = new QSpinBox(tab_2);
        spinBox_Harvester_Timer->setObjectName(QStringLiteral("spinBox_Harvester_Timer"));
        spinBox_Harvester_Timer->setGeometry(QRect(30, 40, 71, 22));
        spinBox_Harvester_Timer->setReadOnly(false);
        spinBox_Harvester_Timer->setMinimum(1);
        spinBox_Harvester_Timer->setMaximum(60);
        spinBox_Harvester_Timer->setValue(5);
        tabWidget_Harvester_Options->addTab(tab_2, QString());
        tabWidget_Global->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        tableWidget_Proxy = new QTableWidget(tab_5);
        tableWidget_Proxy->setObjectName(QStringLiteral("tableWidget_Proxy"));
        tableWidget_Proxy->setGeometry(QRect(20, 20, 231, 181));
        label = new QLabel(tab_5);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(360, 20, 71, 20));
        label_2 = new QLabel(tab_5);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(360, 50, 51, 20));
        lineEdit_Proxy_Host = new QLineEdit(tab_5);
        lineEdit_Proxy_Host->setObjectName(QStringLiteral("lineEdit_Proxy_Host"));
        lineEdit_Proxy_Host->setGeometry(QRect(430, 20, 113, 21));
        lineEdit_Proxy_Port = new QLineEdit(tab_5);
        lineEdit_Proxy_Port->setObjectName(QStringLiteral("lineEdit_Proxy_Port"));
        lineEdit_Proxy_Port->setGeometry(QRect(430, 50, 61, 21));
        pushButton_Add_Proxy = new QPushButton(tab_5);
        pushButton_Add_Proxy->setObjectName(QStringLiteral("pushButton_Add_Proxy"));
        pushButton_Add_Proxy->setGeometry(QRect(430, 80, 80, 21));
        checkBox_Delete_Proxy = new QCheckBox(tab_5);
        checkBox_Delete_Proxy->setObjectName(QStringLiteral("checkBox_Delete_Proxy"));
        checkBox_Delete_Proxy->setGeometry(QRect(160, 210, 91, 19));
        tabWidget_Global->addTab(tab_5, QString());
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 100, 661, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        pushButton_Load_Keyword_List = new QPushButton(centralWidget);
        pushButton_Load_Keyword_List->setObjectName(QStringLiteral("pushButton_Load_Keyword_List"));
        pushButton_Load_Keyword_List->setGeometry(QRect(490, 10, 71, 21));
        label_Current_Keyword = new QLabel(centralWidget);
        label_Current_Keyword->setObjectName(QStringLiteral("label_Current_Keyword"));
        label_Current_Keyword->setGeometry(QRect(346, 80, 271, 20));
        label_Curl_Status = new QLabel(centralWidget);
        label_Curl_Status->setObjectName(QStringLiteral("label_Curl_Status"));
        label_Curl_Status->setGeometry(QRect(11, 391, 291, 21));
        label_Items_Found = new QLabel(centralWidget);
        label_Items_Found->setObjectName(QStringLiteral("label_Items_Found"));
        label_Items_Found->setGeometry(QRect(464, 391, 131, 20));
        lineEdit_Keyword_List_File_Location = new QLineEdit(centralWidget);
        lineEdit_Keyword_List_File_Location->setObjectName(QStringLiteral("lineEdit_Keyword_List_File_Location"));
        lineEdit_Keyword_List_File_Location->setGeometry(QRect(452, 40, 151, 21));
        label_Keywords_Count = new QLabel(centralWidget);
        label_Keywords_Count->setObjectName(QStringLiteral("label_Keywords_Count"));
        label_Keywords_Count->setGeometry(QRect(247, 391, 121, 20));
        mainToolBar = new QToolBar(BeatCrawler);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setGeometry(QRect(0, 0, 4, 12));
        statusBar = new QStatusBar(BeatCrawler);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setGeometry(QRect(0, 0, 3, 18));

        retranslateUi(BeatCrawler);

        tabWidget_Global->setCurrentIndex(1);
        tabWidget_Harvester_Options->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(BeatCrawler);
    } // setupUi

    void retranslateUi(BeatCrawler *BeatCrawler)
    {
        BeatCrawler->setWindowTitle(QApplication::translate("BeatCrawler", "MainWindow", nullptr));
        label_search_results_amount->setText(QApplication::translate("BeatCrawler", "Number of search results to parse", nullptr));
        label_search_engine->setText(QApplication::translate("BeatCrawler", "Use Search Engine", nullptr));
        label_search_engine_keywords->setText(QApplication::translate("BeatCrawler", "Keyword", nullptr));
        pushButton_Start->setText(QApplication::translate("BeatCrawler", "Start", nullptr));
        pushButton_Save_Emails->setText(QApplication::translate("BeatCrawler", "Save", nullptr));
        pushButton_Next_Email_Pagination->setText(QApplication::translate("BeatCrawler", "Next", nullptr));
        pushButton_Previous_Email_Pagination->setText(QApplication::translate("BeatCrawler", "Previous", nullptr));
        checkBox_Delete_Emails->setText(QApplication::translate("BeatCrawler", "Remove Emails", nullptr));
        tabWidget_Global->setTabText(tabWidget_Global->indexOf(tab_3), QApplication::translate("BeatCrawler", "Harvested Emails", nullptr));
        checkBox_Delete_Keywords->setText(QApplication::translate("BeatCrawler", "Remove Keywords or List", nullptr));
        tabWidget_Global->setTabText(tabWidget_Global->indexOf(tab), QApplication::translate("BeatCrawler", "Queue Keywords", nullptr));
        label_Search_Engine_Label_Option->setText(QApplication::translate("BeatCrawler", "Parse selected Search Engines Only", nullptr));
        checkBox_Google->setText(QApplication::translate("BeatCrawler", "google", nullptr));
        checkBox_Bing->setText(QApplication::translate("BeatCrawler", "bing", nullptr));
        checkBox_Yahoo->setText(QApplication::translate("BeatCrawler", "yahoo", nullptr));
        label_UserAgent->setText(QApplication::translate("BeatCrawler", "User Agents", nullptr));
        radioButton_Android_Webkit->setText(QApplication::translate("BeatCrawler", "Android Web Kit", nullptr));
        radioButton_Blackberry->setText(QApplication::translate("BeatCrawler", "Blackberry", nullptr));
        radioButton_IE_Mobile->setText(QApplication::translate("BeatCrawler", "IE Mobile", nullptr));
        tabWidget_Harvester_Options->setTabText(tabWidget_Harvester_Options->indexOf(tab_6), QApplication::translate("BeatCrawler", "Search Engine", nullptr));
        label_Email_Provider_Option_Label->setText(QApplication::translate("BeatCrawler", "Parse selected Emails Only", nullptr));
        checkBox_Email_Gmail->setText(QApplication::translate("BeatCrawler", "gmail", nullptr));
        checkBox_Email_Yahoo->setText(QApplication::translate("BeatCrawler", "yahoo", nullptr));
        checkBox_Email_Hotmail->setText(QApplication::translate("BeatCrawler", "hotmail", nullptr));
        tabWidget_Harvester_Options->setTabText(tabWidget_Harvester_Options->indexOf(tab_7), QApplication::translate("BeatCrawler", "Emails", nullptr));
        label_Social_Networks_Option_Label->setText(QApplication::translate("BeatCrawler", "Parse selected Social Networks  Only", nullptr));
        checkBox_Social_Instagram->setText(QApplication::translate("BeatCrawler", "instagram", nullptr));
        checkBox_Social_Facebook->setText(QApplication::translate("BeatCrawler", "facebook", nullptr));
        checkBox_Social_Soundcloud->setText(QApplication::translate("BeatCrawler", "soundcloud", nullptr));
        checkBox_Social_Reverbnation->setText(QApplication::translate("BeatCrawler", "reverbnation", nullptr));
        checkBox_Social_Myspace->setText(QApplication::translate("BeatCrawler", "myspace", nullptr));
        tabWidget_Harvester_Options->setTabText(tabWidget_Harvester_Options->indexOf(tab_8), QApplication::translate("BeatCrawler", "Social Networks", nullptr));
        label_3->setText(QApplication::translate("BeatCrawler", "Harvester Timer in seconds", nullptr));
        tabWidget_Harvester_Options->setTabText(tabWidget_Harvester_Options->indexOf(tab_2), QApplication::translate("BeatCrawler", "Program behaviour", nullptr));
        tabWidget_Global->setTabText(tabWidget_Global->indexOf(tab_4), QApplication::translate("BeatCrawler", "Options", nullptr));
        label->setText(QApplication::translate("BeatCrawler", "HTTP Proxy", nullptr));
        label_2->setText(QApplication::translate("BeatCrawler", "Proxy Port", nullptr));
        pushButton_Add_Proxy->setText(QApplication::translate("BeatCrawler", "Add Proxy", nullptr));
        checkBox_Delete_Proxy->setText(QApplication::translate("BeatCrawler", "Delete Proxy", nullptr));
        tabWidget_Global->setTabText(tabWidget_Global->indexOf(tab_5), QApplication::translate("BeatCrawler", "Proxies", nullptr));
        pushButton_Load_Keyword_List->setText(QApplication::translate("BeatCrawler", "Load List", nullptr));
        label_Current_Keyword->setText(QApplication::translate("BeatCrawler", "Current Keyword:", nullptr));
        label_Curl_Status->setText(QApplication::translate("BeatCrawler", "Status:", nullptr));
        label_Items_Found->setText(QApplication::translate("BeatCrawler", "Items Found:", nullptr));
        lineEdit_Keyword_List_File_Location->setText(QString());
        label_Keywords_Count->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BeatCrawler: public Ui_BeatCrawler {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEATCRAWLER_H
