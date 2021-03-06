#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "options.h"
#include <QUrl>
#include <QEvent>
#include <QMouseEvent>
#include <QVector>
#include <QList>
#include <QModelIndex>
#include <QCoreApplication>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),qry(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Beat Crawler V0.1.9 (C) Beatcrawler.com");
    ui->lineEdit_keywords_search_box->setPlaceholderText("my mixtape");

    QSize size = this->size();
    this->setFixedSize(size);
    setSearchResults();
    webViewProcess = new QProcess(this);


    proxyServers = new QList <QString>();
    emailTableModel = new QStandardItemModel();
    // 67 is the max amount of options that we will have for now
    options = new OptionsPtr[67];
    for (int i = 0; i < 67; i++) {
        options[i] = new Options();
    }
    fileList = new QStringList();

    currentKeywordString_ = "";
    currentKeyword_ = &currentKeywordString_;


    ui->tableWidget_Proxy->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList keywordQueueTableHeaders;
    keywordQueueTableHeaders << "Keywords";
    ui->tableWidget_Keywords_Queue->setRowCount(40000);
    ui->tableWidget_Keywords_Queue->setColumnCount(1);
    ui->tableWidget_Keywords_Queue->setHorizontalHeaderLabels(keywordQueueTableHeaders);
    ui->tableWidget_Keywords_Queue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Keywords_Queue->resizeRowsToContents();


    ui->pushButton_Next_Email_Pagination->hide();
    ui->pushButton_Previous_Email_Pagination->hide();



    queueKeywords = new QueueKeywords();
    worker = new Worker();
    emailTableTimer = new QTimer();
    completeHarvestTimer = new QTimer();
    emailCountTimer = new QTimer();
    connect(ui->tableWidget_Proxy->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
            SLOT(recieverProxyTableSelection(const QItemSelection &, const QItemSelection &)));
   connect( emailTableTimer, SIGNAL(timeout()), this, SLOT(populateEmailTable()));
   connect(this,SIGNAL(emitRemoveEmailList()),this,SLOT(receiverRemoveEmailList()));
   connect(this,SIGNAL(emitSenderEnableDeleteEmailCheckBox()),this,SLOT(receiverEnableDeleteEmailCheckBox()));

   connect(this,SIGNAL(emitRemoveKeywordList()),this,SLOT(receiverRemoveKeywordList()));
   connect(this,SIGNAL(emitSenderEnableKeywordCheckBox()),this,SLOT(receiverEnableDeleteKeywordCheckBox()));

   connect(worker,SIGNAL(emitDisplayCurrentKeywords(QString ,QString, QString, QString)),
          this,SLOT(receiverDisplayCurrentKeywords(QString ,QString, QString, QString)));

   connect(worker,SIGNAL(emitDisplayCurrentKeyword(QString)),
          this,SLOT(receiverDisplayCurrentKeyword(QString)));

   connect(emailTableTimer,SIGNAL(timeout()),this,SLOT(emailCount()));
  // connect(worker,SIGNAL(emitKeywordsQueueTable()),this,SLOT(receiverKeywordsQueueTable()));

   connect(this,SIGNAL(emitStopQueueKeywords()),queueKeywords,SLOT(receiverStopQueueKeywords()));
   connect(this,SIGNAL(emitStopWorker()),worker,SLOT(receiverStopWorker()));

   connect(this,SIGNAL(emitQueueKeywordListFile(QString)),queueKeywords,SLOT(receiverQueueKeywordListFile(QString)));

   connect(worker,SIGNAL(emitEmailTableModel(QSqlQueryModel *)),this,SLOT(receiverEmailTableModel(QSqlQueryModel*)));

   connect(worker,SIGNAL(emitLogHarvesterStatus(QString)),this,SLOT(receiverLogHarvesterStatus(QString)));

   connect(completeHarvestTimer,SIGNAL(timeout()),this,SLOT(completeHarvesterTimer()));




   //ui->pushButton_Next_Email_Pagination->setEnabled(false);
  // ui->pushButton_Previous_Email_Pagination->setEnabled(false);

   /*****initialize default email table****/

    QStringList emailTableModelHeaderLabels;
    emailTableModelHeaderLabels << "Emails";
    emailTableModel->setHorizontalHeaderLabels(emailTableModelHeaderLabels);
    emailTableModel->setRowCount(4000);
    emailTableModel->setColumnCount(1);
    ui->tableView_Emails->setModel(emailTableModel);
    ui->tableView_Emails->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_Emails->resizeRowsToContents();


    ui->radioButton_Parse_Multi_URL->hide();

    ui->label_Items_Found->hide();

    ui->pushButton_Start->setCheckable(true);
    ui->checkBox_Email_Gmail->setChecked(true);
    ui->checkBox_Email_Hotmail->setChecked(true);
    ui->checkBox_Email_Yahoo->setChecked(true);
    ui->checkBox_Email_Outlook->setChecked(true);


    ui->checkBox_Google->setChecked(true);
    ui->checkBox_Bing->setChecked(true);
    ui->checkBox_Yahoo->setChecked(true);

    ui->checkBox_Social_Facebook->setChecked(true);
    ui->checkBox_Social_Instagram->setChecked(true);
    ui->checkBox_Social_Myspace->setChecked(true);
    ui->checkBox_Social_Soundcloud->setChecked(true);
    ui->checkBox_Social_Reverbnation->setChecked(true);



    ui->radioButton_Android_Webkit->setChecked(true);

    ui->lineEdit_Keyword_List_File_Location->setEnabled(false);

    createMultiKeywordList();
    logStatusCounterNum =0;
    logStatusCounterPtr = &logStatusCounterNum;

    nextEmailPagination = 0;
    nextEmailPaginationPtr = &nextEmailPagination;

    previousEmailPagination = 0;
    previousEmailPaginationPtr = &previousEmailPagination;

    addProxyCounterNum = 0;
    addProxyCounterPtr = &addProxyCounterNum;

    previousPagesNum = 0;
    previousPagesPtr = &previousPagesNum;


    emailRowStartNum =0;
    emailRowStartPtr = &emailRowStartNum;

    emailRowEndNum =0;
    emailRowEndPtr = &emailRowEndNum;



    queryModel = new QSqlQueryModel();
    connOpen();


}

MainWindow::~MainWindow()
{
   delete ui;
   delete options;
   delete proxyServers;
   delete fileList;
   killWebViewProcess();
   delete webViewProcess;

   delete worker;
   delete  emailTableTimer;
   connClose();
   delete queryModel;
   delete qry;
   delete queueKeywords;
   delete emailTableModel;
   delete completeHarvestTimer;
   delete emailCountTimer;
}

void MainWindow::completeHarvesterTimer()
{

    /*******Checks harvest status******/
    QString harvestVal;
    QFile harvestFile;
    QString filename = getRelativePath("harvesterstatus.json");

    harvestFile.setFileName(filename);
    if(!harvestFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error opening json file in webview" << harvestFile.errorString();
    }
    harvestVal = harvestFile.readAll();
    harvestFile.close();


    QJsonDocument harvestDocument = QJsonDocument::fromJson(harvestVal.toUtf8());
    QJsonObject harvestjsonObject = harvestDocument.object();
    QString harvestResults = harvestjsonObject.value(QString("HarvestStatus")).toString();

    if(!harvestResults.isEmpty())
    {
        ui->label_Curl_Status->setText("Status: " + harvestResults );
    }else
    {
        ui->label_Curl_Status->setText("Status: BUSY...");

    }



    /*******Checks if harvester is complete******/
    QString val;
    QFile file;

    QString completeFileName = getRelativePath("complete.json");

    file.setFileName(completeFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error opening json file in webview" << file.errorString();
    }
    val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonDocument jsonDocument;
    QJsonObject jsonObject = document.object();
    QString completeResults = jsonObject.value(QString("COMPLETED_RESULTS")).toString();

    if(!completeResults.isEmpty())
    {
       // qDebug() << "NOT EMPTY";
        if(completeResults == "1_URL_SELECTED_RESULTS_COMPLETED"
           || completeResults == "MULTI_URL_SELECTED_RESULTS_COMPLETED"
           || completeResults == "MULTI_URL_SELECTED_COMPLETED_USING_DEFAULT_SEARCH_OPTION"
           ||  completeResults == "MULTI_URL_SELECTED_ALL_LIST_COMPLETED_USING_BYPASS_OPTION")
        {
            (*emailRowStartPtr) =0;
            (*emailRowEndPtr) =0;
             killWebViewProcess();
             emailTableTimer->stop();
             completeHarvestTimer->stop();
             emit emitStopQueueKeywords();
             emit emitStopWorker();
             ui->label_Curl_Status->setText("Status: ");
             clickedStartStopButton = false;
             ui->pushButton_Start->setText("Start");
             ui->tabWidget_Harvester_Options->setTabEnabled(0, true);
             ui->tabWidget_Harvester_Options->setTabEnabled(1, true);
             ui->tabWidget_Harvester_Options->setTabEnabled(2, true);
             ui->tabWidget_Harvester_Options->setTabEnabled(3, true);
             ui->tableWidget_Proxy->setEnabled(true);
             ui->lineEdit_Proxy_Host->setEnabled(true);
             ui->lineEdit_Proxy_Port->setEnabled(true);
             ui->pushButton_Add_Proxy->setEnabled(true);
             ui->checkBox_Delete_Emails->setEnabled(true);
             ui->checkBox_Delete_Keywords->setEnabled(true);
             ui->lineEdit_keywords_search_box->setEnabled(true);
             ui->pushButton_Save_Emails->setEnabled(true);
             ui->pushButton_Load_Keyword_List->setEnabled(true);

             connOpen();
             QSqlQuery *qry = new QSqlQuery(mydb);
             if(!qry->exec("select * from crawleremails"))
             {
                 qDebug() << "Error selecting rows from crawleremails in mainwinow " << qry->lastError().text();
                 return;
             }
             queryModel->setQuery(*qry);
             QMessageBox::information(this, "Harvest Complete.", "Harvest Completed: Items Found " +QString::number(queryModel->rowCount()));
             delete qry;
             connClose();

             ui->pushButton_Start->setChecked(false);
             // stops user from pressing start to many times in a row, which will lead to problems
             ui->pushButton_Start->setEnabled(false);
             QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));
             clearFiles();
             ui->listWidget_Log_Harvester_Status->clear();


        }
    }else
    {
       // qDebug() << "EMPTY";

    }


}
void MainWindow::emailCount()
{
    for (int i = 0; i<emailTableModel->rowCount(); i++)
    {
        if (emailTableModel->item(i) != nullptr)
        {
            emailListCount.prepend(emailTableModel->item(i)->text());
        }
    }
  // ui->label_Items_Found->setText("Items Found: " + QString::number(emailListCount.toSet().toList().size()));
  // ui->label_Items_Found->setText("Items Found: " + QString::number(emailListCount.size()));

   if(!clickedStartStopButton)
   {
      // emailListCount.clear();
   }

}

void MainWindow::receiverDisplayCurrentKeywords(QString keyword1,QString keyword2, QString keyword3, QString keyword4)
{


    ui->label_Current_Keyword->setText("Current Keywords: " + QString("Queue Keywords Status"));


//    currentKeywordList1 =keyword1;
//    currentKeywordList2 =keyword2;
//    currentKeywordList3 =keyword3;
//    currentKeywordList4 =keyword4;


}

void MainWindow::receiverDisplayCurrentKeyword(QString keyword)
{
    ui->label_Current_Keyword->setText("Current Keyword: " + QString(keyword));
     currentKeyword =keyword;

}

void MainWindow::setSearchResults()
{
    //search engine results
    for (int i = 100; i >= 1; i--)
    {
        QString s = QString::number(i);
        ui->comboBox_search_results_amount->addItem(s);
    }
}

void MainWindow::on_pushButton_Start_clicked(bool checked)

{
    QStringList keywordQueueTableHeaders;
    QJsonObject obj;
    QJsonDocument jsonDocument;
    QFile file;
    if (checked) {

        //check if main options are empty, our program is based around these important options
        // at least one has to be checked. (These will be empty arrays, if at least one isnt checked!)
        if (!ui->checkBox_Google->isChecked() &&
            !ui->checkBox_Bing->isChecked() && !ui->checkBox_Yahoo->isChecked())
        {

            QMessageBox::information(this, "...", "Please select at least one search engine option");
            isSearchEngineSelectOne = false;
        }

        else
        {
            isSearchEngineSelectOne = true;
        }


        if (!ui->checkBox_Social_Instagram->isChecked() && !ui->checkBox_Social_Facebook->isChecked()
            && !ui->checkBox_Social_Soundcloud->isChecked() && !ui->checkBox_Social_Reverbnation->isChecked()
            && !ui->checkBox_Social_Myspace->isChecked() &&

            !ui->checkBox_Social_Youtube->isChecked() && !ui->checkBox_Social_Spotify->isChecked()
            && !ui->checkBox_Social_LiveMixtapes->isChecked() && !ui->checkBox_Social_Datpiff->isChecked()
            && !ui->checkBox_Social_Vine->isChecked() &&

            !ui->checkBox_Social_HipHopDX->isChecked() && !ui->checkBox_Social_Twitter->isChecked()
            && !ui->checkBox_Social_WorldStarHiphop->isChecked() && !ui->checkBox_Social_BandCamp->isChecked()
            && !ui->checkBox_Social_Myspace->isChecked())
        {
            QMessageBox::information(this, "...", "Please select at least one social network option");
            isSocialNetworkSelectOne = false;
        }

        else
        {
            isSocialNetworkSelectOne = true;
        }

        if (!ui->checkBox_Email_Gmail->isChecked() && !ui->checkBox_Email_Hotmail->isChecked()
            && !ui->checkBox_Email_Yahoo->isChecked() && !ui->checkBox_Email_Outlook->isChecked())

        {

            QMessageBox::information(this, "...", "Please select at least one email option");
            isEmailSelectOne = false;
        }

        else {
            isEmailSelectOne = true;
        }

        // if multi url option, and keywordbox are both true, inform user. At this time Multi can only be used with
        // a single keyword or list
        if (ui->radioButton_Parse_Multi_URL->isChecked() && !ui->lineEdit_keywords_search_box->text().isEmpty()) {
            QMessageBox::information(this, "...", "Multi Option can only be used with keyword list");

            isMulti_KeywordBox_Selected = false;
        }
        else {
            isMulti_KeywordBox_Selected = true;
        }

        //radioButton_Bypass_Search_Results

        // if multi url option,keywordbox and bypass search results are true, inform user.
        //At this time Multi can only be used with a list
        if (ui->radioButton_Parse_Multi_URL->isChecked() && !ui->lineEdit_keywords_search_box->text().isEmpty()
              && ui->radioButton_Bypass_Search_Results->isChecked()) {
            QMessageBox::information(this, "...", "Multi Option, and Bypass Results can only be used with keyword list");

            isMulti_KeywordBox_Bypass_Results_Selected = false;
        }
        else {
            isMulti_KeywordBox_Bypass_Results_Selected = true;
        }

        // if multi url option,keywordbox and bypass search results are true, inform user.
        //Bypass search result can only be used with a list
        if (!ui->lineEdit_keywords_search_box->text().isEmpty() && fileList->isEmpty()
              && ui->radioButton_Bypass_Search_Results->isChecked()) {
            QMessageBox::information(this, "...", "Multi Option, and Bypass Results can only be used with keyword list");

            isMulti_KeywordBox_Bypass_Results_Selected = false;
        }
        else {
            isMulti_KeywordBox_Bypass_Results_Selected = true;
        }

        //if we press start buttonand keyword searchb box and keyword list hasnt beeen choosen
        if (ui->lineEdit_keywords_search_box->text().isEmpty() && fileList->isEmpty())
        {
            QMessageBox::information(this, "...", "Please enter a keyword, or Load a list of keywords");
            isKeywordsSelect = false;
        }

        else
        {
            isKeywordsSelect = true;
        }

        // search box is empty but we have keywords in list // ok
        if (ui->lineEdit_keywords_search_box->text().isEmpty() && !fileList->isEmpty())
        {
            isKeywordsSelect = true;
        }

        // search box is  not empty but we dont have keywords in list // ok
        if (!ui->lineEdit_keywords_search_box->text().isEmpty() && fileList->isEmpty()) {
            isKeywordsSelect = true;
        }


        if (isSocialNetworkSelectOne == true && isSearchEngineSelectOne == true && isKeywordsSelect == true
            && isEmailSelectOne == true && isMulti_KeywordBox_Selected == true
             && isMulti_KeywordBox_Bypass_Results_Selected== true)
        {

            ui->pushButton_Start->setText("Stop");
            //important options are checked so we set the start buttin to setChecked(true)
            ui->pushButton_Start->setChecked(true);

            //Harvester Timer Value
            // Goes in Thread

            // if keyword list is not empty, and keywordsearch box isnt empty add
            // the keyword from search box into keyword list hash table
            if (!ui->lineEdit_keywords_search_box->text().isEmpty() && !fileList->isEmpty())
            {
               fileList->prepend(ui->lineEdit_keywords_search_box->text());


            }
            // clears emails queue table if any emails were in it
            keywordQueueTableHeaders << "Keywords";
            ui->tableWidget_Keywords_Queue->setHorizontalHeaderLabels(keywordQueueTableHeaders);


            // KEYWORD BOX INPUT
            if (!ui->lineEdit_keywords_search_box->text().isEmpty())
            {
                QMessageBox msgBox;
                QString cleanString = ui->lineEdit_keywords_search_box->text();

                // QString filteredString1 = cleanString.remove(QRegExp(QString::fromUtf8("[^\S+(\s\S+)+$]")));
                // creates a array from string
                //QStringList filteredString2 = filteredString1.split(" ");
              //  options[3]->keywordSearchBoxOptions[0] = cleanString;

            }


            //SEARCH ENGINE OPTION
            if (ui->checkBox_Google->isChecked())
            {
                options[0]->searchEngineOptions[0] = "http://google.com";
            }

            if (ui->checkBox_Bing->isChecked())
            {
                options[0]->searchEngineOptions[1] = "http://bing.com";
            }

            if (ui->checkBox_Yahoo->isChecked())
            {
                options[0]->searchEngineOptions[2] = "http://yahoo.com";
            }



            //EMAIL OPTION
            if (ui->checkBox_Email_Gmail->isChecked())
            {
                options[1]->emailOptions[0] = "@gmail.com";
            }

            if (ui->checkBox_Email_Yahoo->isChecked())
            {
                options[1]->emailOptions[1] = "@yahoo.com";
            }

            if (ui->checkBox_Email_Hotmail->isChecked())
            {
                options[1]->emailOptions[2] = "@hotmail.com";
            }
            if (ui->checkBox_Email_Outlook->isChecked())
            {
                options[1]->emailOptions[3] = "@outlook.com";
            }


            //SOCIAL NETWORK OPTION

            if (ui->checkBox_Social_Instagram->isChecked())
            {
                options[2]->socialNetworkOptions[0] = "site:instagram.com";
            }

            if (ui->checkBox_Social_Facebook->isChecked())
            {
                options[2]->socialNetworkOptions[1] = "site:facebook.com";
            }

            if (ui->checkBox_Social_Soundcloud->isChecked())
            {
                options[2]->socialNetworkOptions[2] = "site:soundcloud.com";
            }

            if (ui->checkBox_Social_Reverbnation->isChecked())
            {
                options[2]->socialNetworkOptions[3] = "site:reverbnation.com";
            }

            if (ui->checkBox_Social_Myspace->isChecked())
            {
                options[2]->socialNetworkOptions[4] = "site:myspace.com";
            }

            if (ui->checkBox_Social_Instagram->isChecked())
            {
                options[2]->socialNetworkOptions[5] = "site:youtube.com";
            }

            if (ui->checkBox_Social_Spotify->isChecked())
            {
                options[2]->socialNetworkOptions[6] = "site:spotify.com";
            }

            if (ui->checkBox_Social_LiveMixtapes->isChecked())
            {
                options[2]->socialNetworkOptions[7] = "site:livemixtapes.com";
            }

            if (ui->checkBox_Social_Datpiff->isChecked())
            {
                options[2]->socialNetworkOptions[8] = "site:datpiff.com";
            }

            if (ui->checkBox_Social_Vine->isChecked())
            {
                options[2]->socialNetworkOptions[9] = "site:vine.co";
            }


            if (ui->checkBox_Social_HipHopDX->isChecked())
            {
                options[2]->socialNetworkOptions[10] = "site:hiphopdx.com";
            }

            if (ui->checkBox_Social_Twitter->isChecked())
            {
                options[2]->socialNetworkOptions[11] = "site:twitter.com";
            }

            if (ui->checkBox_Social_WorldStarHiphop->isChecked())
            {
                options[2]->socialNetworkOptions[12] = "site:worldstarhiphop.com";
            }

            if (ui->checkBox_Social_BandCamp->isChecked())
            {
                options[2]->socialNetworkOptions[13] = "site:bandcamp.com";
            }



            //USER AGENT OPTIONS

            if (ui->radioButton_Android_Webkit->isChecked())
            {
                options[5]->userAgentsOptions[0] = "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30";
            }

            if (ui->radioButton_Blackberry->isChecked())
            {
                options[5]->userAgentsOptions[0] = "Mozilla/5.0 (BlackBerry; U; BlackBerry 9900; en) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.1.0.346 Mobile Safari/534.11+";
            }


           if (ui->radioButton_IE_Mobile->isChecked())
             {
                options[5]->userAgentsOptions[0] = "Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0)";
              }

            //SEARCH RESULTS COMBO BOX

            if (!ui->comboBox_search_results_amount->currentText().isEmpty()) {

                // ui->comboBox_search_results_amount->currentText()

            }



            //create a object of value start_harvest when we press stop
           obj.insert("StartHarvest",QJsonValue("START_HARVEST").toString());
           QString stopHarvest = getRelativePath("stopharvest.json");

           file.setFileName(stopHarvest);
           if(!file.open(QIODevice::WriteOnly)){
               qDebug() << "Error opening stopharvest json.. file in mainwindow";
               return;
           }
           jsonDocument.setObject(obj);
           file.write(jsonDocument.toJson());
           file.close();

           QString userName =  qgetenv("USERNAME");
           QString crawlerAppDataResourcesDir =  QStandardPaths::writableLocation(QStandardPaths::DataLocation);
           QString dir = "C:/Users/" + userName+"/AppData/Local/BeatCrawler/resources/";
           QDir resourcesDir(dir);
           QStringList args;
            if(RESOURCES_DIR == "RELEASE")
            {

                if(resourcesDir.exists())
                {

                  // qDebug() << resourcesDir.path() +"/" + QString("WebView.exe");
                   webViewProcess->start(resourcesDir.path() +"/" + QString("WebView.exe"),args);
                }

            }else
            {
                webViewProcess->start(QCoreApplication::applicationDirPath() + "C:/Users/ace/Documents/QT_Projects/WebView/build-WebView-Desktop_Qt_5_9_4_MSVC2015_32bit2-Release/release/"  + QString("WebView.exe"),args);

            }

          // webViewProcess->start(QCoreApplication::applicationDirPath()+ "/resources/webview/debug/"+ "WebView.exe",args);

           // QString webViewResult = webViewProcess->readAllStandardOutput();
           // qDebug() <<"WebView Process Error String-> " << webViewProcess->errorString();
           // qDebug() << "WebView Process Results- > " << webViewResult;

            //serverProcess->start("C:/Users/ace/Documents/QT_Projects/WebViewServer/build-Server-Desktop_Qt_5_9_4_MSVC2015_64bit2-Debug/debug/Server.exe",args);
           // QString serverResult = serverProcess->readAllStandardOutput();
           // qDebug() <<"Server Process Error String-> " << serverProcess->errorString();
           // qDebug() << "Server Process Results- > " <<serverResult;


            ui->label_Curl_Status->setText("Status: Starting...");
            ui->label_Current_Keyword->setText("Current Keyword: ");
            ui->label_Items_Found->setText("Items Found: ");
            clickedStartStopButton = true;
            //disables options, stops user from altering options while harvesting
            ui->tabWidget_Harvester_Options->setTabEnabled(0, false);
            ui->tabWidget_Harvester_Options->setTabEnabled(1, false);
            ui->tabWidget_Harvester_Options->setTabEnabled(2, false);
            ui->tabWidget_Harvester_Options->setTabEnabled(3, false);

            // ui->tabWidget_Global->setTabEnabled(3,false);
            ui->tableWidget_Proxy->setEnabled(false);
            ui->lineEdit_Proxy_Host->setEnabled(false);
            ui->lineEdit_Proxy_Port->setEnabled(false);
            ui->pushButton_Add_Proxy->setEnabled(false);
            ui->checkBox_Delete_Emails->setEnabled(false);
            ui->checkBox_Delete_Keywords->setEnabled(false);
            ui->lineEdit_keywords_search_box->setEnabled(false);
            ui->pushButton_Save_Emails->setEnabled(false);
            ui->pushButton_Load_Keyword_List->setEnabled(false);

          //  ui->pushButton_Next_Email_Pagination->setEnabled(false);
          //  ui->pushButton_Previous_Email_Pagination->setEnabled(false);


            receiverParameters();
            emailTableTimer->start(5000);
            completeHarvestTimer->start(5000);
            // initialize email table -- checks to see if emails are in tabl

            // stops user from pressing start to many times in a row, which will lead to problems
            ui->pushButton_Start->setEnabled(false);
            QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));
            //keywordsQueueTable();

            //worker->getCurrentKeywords();
            QFuture<void> multithread1  = QtConcurrent::run(this->worker, &Worker::getCurrentKeywords);
            QFuture<void> multithread2  = QtConcurrent::run(this->queueKeywords,&QueueKeywords::getCurrentKeywords);
            if(ui->checkBox_Enable_AutoMailer->isChecked())
            {
               // QFuture<void> multithread3  = QtConcurrent::run(this,&MainWindow::sendMail);
                sendMail();
                ui->label_Email_Sent_SMTP->setText("Emails Sent via SMTP: Enabled");
            }





        } //else important options are not checked so we set the start buttin to setChecked(false)
        else
        {
            ui->pushButton_Start->setChecked(false);
        }

    } // end if checked



    if (!checked) {

        ui->listWidget_Log_Harvester_Status->clear();
       (*emailRowStartPtr) =0;
       (*emailRowEndPtr) =0;
        killWebViewProcess();
        emailTableTimer->stop();
        completeHarvestTimer->stop();
         //create a object of value cancel_harvest when we press stop
        obj.insert("StartHarvest",QJsonValue("CANCEL_HARVEST").toString());
        QDir dir("/resources");
        QString filename = getRelativePath("stopharvest.json");

        file.setFileName(filename);
        if(!file.open(QIODevice::WriteOnly)){
            qDebug() << "Error opening stopharvest json file in mainwindow";
            return;
        }
        jsonDocument.setObject(obj);
        file.write(jsonDocument.toJson());
        file.close();
        emit emitStopQueueKeywords();
        emit emitStopWorker();
        ui->label_Curl_Status->setText("Status: ");
        clickedStartStopButton = false;
        ui->pushButton_Start->setText("Start");
        ui->tabWidget_Harvester_Options->setTabEnabled(0, true);
        ui->tabWidget_Harvester_Options->setTabEnabled(1, true);
        ui->tabWidget_Harvester_Options->setTabEnabled(2, true);
        ui->tabWidget_Harvester_Options->setTabEnabled(3, true);
        // ui->tabWidget_Global->setTabEnabled(3,true);
        ui->tableWidget_Proxy->setEnabled(true);
        ui->lineEdit_Proxy_Host->setEnabled(true);
        ui->lineEdit_Proxy_Port->setEnabled(true);
        ui->pushButton_Add_Proxy->setEnabled(true);
        ui->checkBox_Delete_Emails->setEnabled(true);
        ui->checkBox_Delete_Keywords->setEnabled(true);
        ui->lineEdit_keywords_search_box->setEnabled(true);
        ui->pushButton_Save_Emails->setEnabled(true);
        ui->pushButton_Load_Keyword_List->setEnabled(true);

        clearFiles();

        // stops user from pressing start to many times in a row, which will lead to problems
       ui->pushButton_Start->setEnabled(false);
       QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));






    }
}
void MainWindow::reEnableStartButton() {
    ui->pushButton_Start->setEnabled(true);

}

void MainWindow::createMultiKeywordList(){


}

void MainWindow::on_pushButton_Load_Keyword_List_clicked()
{



        QString fileName = QFileDialog::getOpenFileName(this, "Open text file", "");
        QFile file(fileName);
        QFileInfo fi(file.fileName());
        QString fileExt = fi.completeSuffix();;
        QString strings;
        QString str;
        QString fileLine;
        int readLines =0;

        if (fileExt == "txt") {

                if (!file.open(QIODevice::ReadOnly)) {
                    //QMessageBox::warning(this,"...","error in opening keyword file");
                    return;
                }

                //data = file.read(file.size());
                //fileLine.append(data);
                //fileReadLines << fileLine.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
                ui->pushButton_Load_Keyword_List->setEnabled(false);
                ui->checkBox_Delete_Keywords->setEnabled(false);
                ui->pushButton_Start->setEnabled(false);

                fileList->clear();
                removeDuplicatesFileList.clear();
                multi1KeywordList.clear();
                multi2KeywordList.clear();

                QTextStream ts(&file);
                while (!ts.atEnd()) {
                    QApplication::processEvents();

                    str = ts.readLine();
                    removeDuplicatesFileList << str;

                    // queue keywords signal for main keyword list
                    emit emitQueueKeywordListFile(str);
                    finishReadingKeywordFile = false;
                    readLines++;
                }

                // if file lines are less than 10
                if(removeDuplicatesFileList.toSet().toList().size() < 10)
                {
                    QMessageBox::information(this, "...", "Keyword List must be unique, 10 words minimum, and 1 per line.");
                    ui->pushButton_Load_Keyword_List->setEnabled(true);
                    ui->checkBox_Delete_Keywords->setEnabled(true);
                    ui->pushButton_Start->setEnabled(true);
                    return;

                }else
                {
                    *fileList  = removeDuplicatesFileList.toSet().toList();
                    while(ts.atEnd()){
                        finishReadingKeywordFile = true;
                        qDebug() << "Done";
                        break;
                    }
                 if(finishReadingKeywordFile == true)

                 {
                     ui->pushButton_Load_Keyword_List->setEnabled(true);
                     ui->checkBox_Delete_Keywords->setEnabled(true);
                     ui->pushButton_Start->setEnabled(true);
                     finishReadingKeywordFile = true;
                     for (int row = 0; row < fileList->size(); row++)
                     {
                         for (int col = 0; col < 2; col++)
                         {
                             if (col == 0) {
                                 ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(fileList->value(row)));
                             }

                         }


                     }// end for loop

                     ui->lineEdit_Keyword_List_File_Location->setText(file.fileName());


                     /*********When were finish reading file, check if multi was checked, create 5 multi keyword list files********/
                     // if list was uploaded and MULRI_URL_SELECTED was choosen

                     if (ui->radioButton_Parse_Multi_URL->isChecked())
                     {


                         QFile multi1KeywordListFile;
                         QString multi1 = getRelativePath("multi1keywordList.txt");

                         multi1KeywordListFile.setFileName(multi1);
                         if(!multi1KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
                             qDebug() << "Error opening multi keyword list file1 in mainwindow";
                             return;
                         }
                         QFile multi2KeywordListFile;
                         QString multi2 = getRelativePath("multi2keywordList.txt");

                         multi2KeywordListFile.setFileName(multi2);
                         if(!multi2KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
                             qDebug() << "Error opening multi keyword list file2 in mainwindow";
                             return;
                         }
                         QFile multi3KeywordListFile;
                         QString multi3 = getRelativePath("multi3keywordList.txt");

                         multi3KeywordListFile.setFileName(multi3);
                         if(!multi3KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
                             qDebug() << "Error opening multi keyword list file3 in mainwindow";
                             return;
                         }
                         QFile multi4KeywordListFile;
                         QString multi4 = getRelativePath("multi4keywordList.txt");

                         multi4KeywordListFile.setFileName(multi4);
                         if(!multi4KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
                             qDebug() << "Error opening multi keyword list file4 in mainwindow";
                             return;
                         }

                         QTextStream multi1KeywordListFileStream(&multi1KeywordListFile);
                         QTextStream multi2KeywordListFileStream(&multi2KeywordListFile);
                         QTextStream multi3KeywordListFileStream(&multi3KeywordListFile);
                         QTextStream multi4KeywordListFileStream(&multi4KeywordListFile);




                         // loop through main keyword list to get 2 seperate list, if multi option is checked
                         for (int i = 0; i < fileList->size(); i++)
                         {
                             // if j is a even int, put keywords into this list
                             if (i % 2 == 0)
                             {
                                multi1KeywordListFileStream.flush();
                                multi1KeywordList << fileList->value(i);
                             }
                             // else put odd keywords into a new list
                             else
                             {
                                multi2KeywordListFileStream.flush();
                                multi2KeywordList << fileList->value(i);

                             }
                         }// end of looping through fileList


                         // loop through multi1KeywordList  to get 2 more list, if multi option is checked
                         for (int i = 0; i < multi1KeywordList.size(); i++)
                         {
                             // if i is a even int, put keywords into this list
                             if (i % 2 == 0 )
                             {
                                multi3KeywordListFileStream.flush();
                                multi3KeywordListFileStream << multi1KeywordList.value(i) << "\n";
                             }
                             // else put odd keywords into a new list
                             else
                             {
                                 multi4KeywordListFileStream.flush();
                                 multi4KeywordListFileStream << multi1KeywordList.value(i) << "\n";
                             }
                         }// end of looping through multi1KeywordList


                         // loop through multi2KeywordList  to get 4 unique list if multi option is checked
                         for (int i = 0; i < multi2KeywordList.size(); i++)
                         {
                             // if i is a even int, put keywords into this list
                             if (i % 2 == 0)
                             {
                                multi1KeywordListFileStream.flush();
                                multi1KeywordListFileStream << multi2KeywordList.value(i) << "\n";
                             }else{
                                 multi2KeywordListFileStream.flush();
                                 multi2KeywordListFileStream << multi2KeywordList.value(i) << "\n";
                             }
                         }// end of looping through multi2KeywordList



                         // close keyword list files
                         multi1KeywordListFile.close();
                         multi2KeywordListFile.close();
                         multi3KeywordListFile.close();
                         multi4KeywordListFile.close();

                     }// end checking if MULTI_URL_SELECTED radio button was checked

                     // if list was uploaded and 1_URL_SELECTED was choosen
                     if (ui->radioButton_Parse_1_URL->isChecked())
                     {


                         QFile multi1KeywordListFile;
                         QString multi1 = getRelativePath("multi1keywordList.txt");

                         multi1KeywordListFile.setFileName(multi1);
                         if(!multi1KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
                             qDebug() << "Error opening multi keyword list file1 in mainwindow";
                             return;
                         }
                         QTextStream multi1KeywordListFileStream(&multi1KeywordListFile);


                         // loop through main keyword list to get 1 seperate list, if 1 url option is checked
                         for (int i = 0; i < fileList->size(); i++)
                         {
                                multi1KeywordListFileStream.flush();
                                multi1KeywordListFileStream << fileList->value(i) << "\n";

                         }// end of looping through fileList

                         multi1KeywordListFile.close();

                     }// end checking if 1_URL_SELECTED radio button was checked


                 }// end of checking if finishReadingKeywordFile

                }//end of checking if file lines are less than 10


    }
    else // else user needs to select txt file
    {
        QMessageBox::information(this, "...", "Please selext a text file");

    }

        file.close();

}

void MainWindow::receiverParameters()

{

    QString searchEngineParam;
    QString castSearchQueryNumPtr;
    QString socialNetWork;
    QString email;
    QString searchEngine;
    QString vectorSearchEngineOptionsLastItem;
    QString currentKeywordSearchBoxKeyword;
    QString filterCurrentKeyword;
    QString itemsFound;
    enum defaultSearchResult{DEFAULT_SEARCH_RESULT =0,BYPASS_SEARCH_RESULT=1};
    enum fileListUploaded{FILELIST_UPLOADED_FALSE =0, FILELIST_UPLOADED_TRUE =1};

   // worker->requestNewEmails();
    //QTimer::singleShot(9000,worker,SLOT(requestNewEmails()));

    /**********Search Engine Options ******/



    vectorSearchEngineOptions.clear();
    for (int i = 0; i < vectorSearchEngineOptions.size(); i++)
    {
        vectorSearchEngineOptions.removeAll(vectorSearchEngineOptions.value(i));
    }



    if (ui->checkBox_Google->isChecked())
    {
        vectorSearchEngineOptions.resize(1);
        vectorSearchEngineOptions.push_back(options[0]->searchEngineOptions[0]);
    }



    if (ui->checkBox_Bing->isChecked())
     {
        vectorSearchEngineOptions.resize(2);
        vectorSearchEngineOptions.push_back(options[0]->searchEngineOptions[1]);
    }



    if (ui->checkBox_Yahoo->isChecked())
    {
        vectorSearchEngineOptions.resize(3);
        vectorSearchEngineOptions.push_back(options[0]->searchEngineOptions[2]);
    }


   // removes empty index
    for (int i = 0; i < vectorSearchEngineOptions.size(); i++)
     {
        vectorSearchEngineOptions.removeAll("");
    }


    vectorEmailOptions.clear();
    for (int i = 0; i < vectorEmailOptions.size(); i++)
    {
        vectorEmailOptions.removeAll(vectorEmailOptions.value(i));
    }


    if (ui->checkBox_Email_Gmail->isChecked())
    {
        vectorEmailOptions.resize(1);
        vectorEmailOptions.push_back(options[1]->emailOptions[0]);
    }



    if (ui->checkBox_Email_Yahoo->isChecked())
    {
        vectorEmailOptions.resize(2);
        vectorEmailOptions.push_back(options[1]->emailOptions[1]);
    }


    if (ui->checkBox_Email_Hotmail->isChecked())
    {
        vectorEmailOptions.resize(3);
        vectorEmailOptions.push_back(options[1]->emailOptions[2]);
    }

    if (ui->checkBox_Email_Outlook->isChecked())
    {
        vectorEmailOptions.resize(4);
        vectorEmailOptions.push_back(options[1]->emailOptions[3]);
    }


    // removes empty index
    for (int i = 0; i <vectorEmailOptions.size(); i++)
    {
        vectorEmailOptions.removeAll("");
    }



    vectorSocialNetworks2.clear();

    for (int i = 0; i <vectorSocialNetworks2.size(); i++)
    {

        vectorSocialNetworks2.removeAll(vectorSocialNetworks2.value(i));
    }


    if (ui->checkBox_Social_Instagram->isChecked())
    {
        vectorSocialNetworks2.resize(1);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[0]);
    }

    if (ui->checkBox_Social_Facebook->isChecked())
    {
        vectorSocialNetworks2.resize(2);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[1]);
    }

    if (ui->checkBox_Social_Soundcloud->isChecked())
    {
        vectorSocialNetworks2.resize(3);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[2]);
    }

    if (ui->checkBox_Social_Reverbnation->isChecked())
    {
        vectorSocialNetworks2.resize(4);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[3]);
    }


    if (ui->checkBox_Social_Myspace->isChecked())
    {
        vectorSocialNetworks2.resize(5);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[4]);
    }


    if (ui->checkBox_Social_Youtube->isChecked())
    {
        vectorSocialNetworks2.resize(6);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[5]);
    }

    if (ui->checkBox_Social_Spotify->isChecked())
    {
        vectorSocialNetworks2.resize(7);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[6]);
    }

    if (ui->checkBox_Social_LiveMixtapes->isChecked())
    {
        vectorSocialNetworks2.resize(8);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[7]);
    }

    if (ui->checkBox_Social_Datpiff->isChecked())
    {
        vectorSocialNetworks2.resize(9);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[8]);
    }


    if (ui->checkBox_Social_Vine->isChecked())
    {
        vectorSocialNetworks2.resize(10);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[9]);
    }


    if (ui->checkBox_Social_HipHopDX->isChecked())
    {
        vectorSocialNetworks2.resize(11);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[10]);
    }

    if (ui->checkBox_Social_Twitter->isChecked())
    {
        vectorSocialNetworks2.resize(12);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[11]);
    }

    if (ui->checkBox_Social_WorldStarHiphop->isChecked())
    {
        vectorSocialNetworks2.resize(13);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[12]);
    }

    if (ui->checkBox_Social_BandCamp->isChecked())
    {
        vectorSocialNetworks2.resize(14);
        vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[13]);
    }



    // removes empty index
    for (int i = 0; i <vectorSocialNetworks2.size(); i++)
    {
      vectorSocialNetworks2.removeAll("");
    }



   // qDebug() << vectorSearchEngineOptions;
   // qDebug() << vectorEmailOptions;
   // qDebug() <<vectorSocialNetworks2;

    QJsonObject obj;
    QStringList searchEngineOptionsList;
    QStringList emailOptionsList;
    QStringList socialNetworkOptionsList;
    QStringList proxyServersOptionsList;
    QJsonObject timerOptionsJson;
    QJsonObject otherOptionsJson;
    QJsonDocument jsonDocument;

    for(int i=0; i <vectorSearchEngineOptions.size(); i++)
    {
         searchEngineOptionsList.insert(i,vectorSearchEngineOptions.value(i));
    }
    for(int i=0; i <vectorEmailOptions.size(); i++)
    {
         emailOptionsList.insert(i,vectorEmailOptions.value(i));
    }
    for(int i=0; i <vectorSocialNetworks2.size(); i++)
    {
         socialNetworkOptionsList.insert(i,vectorSocialNetworks2.value(i));
    }
    for(int i=0; i <proxyServers->size(); i++)
    {
         proxyServersOptionsList.insert(i,proxyServers->value(i));
    }

    timerOptionsJson.insert("ProxyRotate",QJsonValue(ui->spinBox_Proxy_Rotate_Interval->value()).toInt());
    timerOptionsJson.insert("HarvesterTimer",QJsonValue(ui->spinBox_Harvester_Timer->value()).toInt());

    otherOptionsJson.insert("searchResultsPagesNumber",QJsonValue(ui->comboBox_search_results_amount->currentText().toInt()).toInt());

    if (ui->radioButton_Parse_Multi_URL->isChecked()) {
        otherOptionsJson.insert("MULTI_URL_OPTION",QJsonValue("MULTI_URL_SELECTED").toString());

    }
    if (ui->radioButton_Parse_1_URL->isChecked()) {
        otherOptionsJson.insert("MULTI_URL_OPTION",QJsonValue("1_URL_SELECTED").toString());
    }

    if(ui->radioButton_Default_Search_Results->isChecked())
    {
        otherOptionsJson.insert("SEARCH_RESULTS_OPTION",QJsonValue(DEFAULT_SEARCH_RESULT).toInt());

    }
    if(ui->radioButton_Bypass_Search_Results->isChecked())
    {
        otherOptionsJson.insert("SEARCH_RESULTS_OPTION",QJsonValue(BYPASS_SEARCH_RESULT).toInt());
    }

    if(!fileList->isEmpty())
    {
        obj.insert("FILE_LIST_UPLOADED",QJsonValue(FILELIST_UPLOADED_TRUE).toInt());
    }
    if(fileList->isEmpty())
    {
        obj.insert("FILE_LIST_UPLOADED",QJsonValue(FILELIST_UPLOADED_FALSE).toInt());
    }
    otherOptionsJson.insert("USER_AGENT",QJsonValue(options[5]->userAgentsOptions[0]).toString());

    /*******Insert json data into json object********/
    obj.insert("SearchEngineOptions",QJsonArray::fromStringList(searchEngineOptionsList));
    obj.insert("EmailOptions",QJsonArray::fromStringList(emailOptionsList));
    obj.insert("SocialNetworkOptions",QJsonArray::fromStringList( socialNetworkOptionsList));
    obj.insert("LineEditKeywords",QJsonValue(ui->lineEdit_keywords_search_box->text()).toString());
    obj.insert("TimerOptions",timerOptionsJson);
    obj.insert("OtherOptions",otherOptionsJson);
    obj.insert("ProxyServers",QJsonArray::fromStringList(proxyServersOptionsList));


   // qDebug() << obj;
    QFile file;
    QString filename = getRelativePath("params.json");


    file.setFileName(filename);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Error opening json file in mainwindow";
        return;
    }
   jsonDocument.setObject(obj);
   file.write(jsonDocument.toJson());
   file.close();



    //QString keywordLabel = ui->lineEdit_keywords_search_box->text();
    /*******

    QFuture<void> multithread1  = QtConcurrent::run(&this->threadWorker
        , &Worker::curlParams, vectorSearchOptions, ui->lineEdit_keywords_search_box->text(), proxyServers,
        timerOptions, otherOptions);

   **********/
   //QFuture<void> multithread1  = QtConcurrent::run(this->worker,&Worker::getEmailList);
//   QFuture<void> multithread1  = QtConcurrent::run(this->worker,
//                                &Worker::getCurrentKeywords);


}


void MainWindow::on_radioButton_Parse_Multi_URL_clicked(bool checked)

{


    /*********When were finish reading file, check if multi was checked, create two multi keyword list files********/
    if (checked == true && fileList->isEmpty())
    {

        QMessageBox::information(this, "...", "Please upload a keyword list");
        //ui->radioButton_Parse_Multi_URL->setChecked(false);
        ui->radioButton_Parse_Multi_URL->setChecked(false);
        ui->radioButton_Parse_1_URL->setChecked(true);

        return;


    }

    if(checked == true && !fileList->isEmpty())
    {
        ui->radioButton_Parse_Multi_URL->setChecked(true);
        ui->radioButton_Parse_1_URL->setChecked(false);

        QFile multi1KeywordListFile;
        QString multi1 = getRelativePath("multi1keywordList.txt");

        multi1KeywordListFile.setFileName(multi1);
        if(!multi1KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Error opening multi keyword list file1 in mainwindow";
            return;
        }
        QFile multi2KeywordListFile;
        QString multi2 = getRelativePath("multi2keywordList.txt");

        multi2KeywordListFile.setFileName(multi2);
        if(!multi2KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Error opening multi keyword list file2 in mainwindow";
            return;
        }
        QFile multi3KeywordListFile;
        QString multi3 = getRelativePath("multi3keywordList.txt");

        multi3KeywordListFile.setFileName(multi3);
        if(!multi3KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Error opening multi keyword list file3 in mainwindow";
            return;
        }
        QFile multi4KeywordListFile;
        QString multi4 = getRelativePath("multi4keywordList.txt");

        multi4KeywordListFile.setFileName(multi4);
        if(!multi4KeywordListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Error opening multi keyword list file4 in mainwindow";
            return;
        }

        QTextStream multi1KeywordListFileStream(&multi1KeywordListFile);
        QTextStream multi2KeywordListFileStream(&multi2KeywordListFile);
        QTextStream multi3KeywordListFileStream(&multi3KeywordListFile);
        QTextStream multi4KeywordListFileStream(&multi4KeywordListFile);



        // loop through main keyword list to get 2 seperate list, if multi option is checked
        for (int i = 0; i < fileList->size(); i++)
        {
            // if j is a even int, put keywords into this list
            if (i % 2 == 0)
            {
               multi1KeywordListFileStream.flush();
               multi1KeywordList << fileList->value(i);

            }
            // else put odd keywords into a new list
            else
            {
               multi2KeywordListFileStream.flush();
               multi2KeywordList << fileList->value(i);

            }
        }// end of looping through fileList


        // loop through multi1KeywordList  to get 2 more list, if multi option is checked
        for (int i = 0; i < multi1KeywordList.size(); i++)
        {
            // if i is a even int, put keywords into this list
            if (i % 2 == 0 )
            {
               multi3KeywordListFileStream.flush();
               multi3KeywordListFileStream << multi1KeywordList.value(i) << "\n";
            }
            // else put odd keywords into a new list
            else
            {
                multi4KeywordListFileStream.flush();
                multi4KeywordListFileStream << multi1KeywordList.value(i) << "\n";
            }
        }// end of looping through multi1KeywordList


        // loop through multi2KeywordList  to get 4 unique list if multi option is checked
        for (int i = 0; i < multi2KeywordList.size(); i++)
        {
            // if i is a even int, put keywords into this list
            if (i % 2 == 0)
            {
               multi1KeywordListFileStream.flush();
               multi1KeywordListFileStream << multi2KeywordList.value(i) << "\n";
            }else{
                multi2KeywordListFileStream.flush();
                multi2KeywordListFileStream << multi2KeywordList.value(i) << "\n";
            }
        }// end of looping through multi2KeywordList



        // close keyword list files
        multi1KeywordListFile.close();
        multi2KeywordListFile.close();
        multi3KeywordListFile.close();
        multi4KeywordListFile.close();

    }

}

void MainWindow::on_pushButton_Add_Proxy_clicked()
{
    QStringList proxyTableHeaders;
    if (!ui->lineEdit_Proxy_Port->text().isEmpty() && !ui->lineEdit_Proxy_Host->text().isEmpty())

    {



    }
    // Anytime add proxy button is clicked we increment a counter to become the index for the current proxy
    (*addProxyCounterPtr) += 1;
    //insert proxy into qlist
    proxyServers->insert(*addProxyCounterPtr, ui->lineEdit_Proxy_Host->text() + ":" + ui->lineEdit_Proxy_Port->text());
    proxyTableHeaders << "Proxy Server" << "Proxy Port";
    ui->tableWidget_Proxy->setRowCount(proxyServers->size());
    ui->tableWidget_Proxy->setColumnCount(2);
    // loops through the size of the proxyServer qlist
    for (int row = 0; row < proxyServers->size(); row++)
    {

        QString url = "http://" + proxyServers->value(row);
        QUrl server(url);

        // validates proxy host
        QRegExp host("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
        QRegExpValidator hostValidator(host, 0);
        QString proxyHostString;
        int proxyHostPosition = 0;
        proxyHostString = ui->lineEdit_Proxy_Host->text();

        // validates proxy port
        QRegExp port("((?:))(?:[0-9]+)$");
        QRegExpValidator portValidator(port, 0);
        QString  proxyPortString;
        int proxyPortPosition = 0;
        proxyPortString = ui->lineEdit_Proxy_Port->text();

        // if proxy host regex does not validate to 2,its invalid
        if (hostValidator.validate(proxyHostString, proxyHostPosition) != 2)
        {

            // remove current proxy from proxyServer qlist
            proxyServers->removeAll(ui->lineEdit_Proxy_Host->text() + ":" + ui->lineEdit_Proxy_Port->text());
            QMessageBox::warning(this, "...", "Proxy server error, please enter a valid proxy server");
            isProxyHostValid = false;
            break;

        }

        else

        {
            isProxyHostValid = true;
        }


        // if proxy port regex does not validate to 2,its invalid
        if (portValidator.validate(proxyPortString, proxyPortPosition) != 2)
        {
            // remove current proxy from proxyServer qlist
            proxyServers->removeAll(ui->lineEdit_Proxy_Host->text() + ":" + ui->lineEdit_Proxy_Port->text());
            QMessageBox::warning(this, "...", "Proxy port error, please enter a valid port");
            isProxyPortValid = false;
            break;
        }

        else

        {
            isProxyPortValid = true;
        }


        // qDebug() <<*proxyServers;


        for (int col = 0; col < 2; col++)
        {


            // if both proxy port, and proxy sever is valid, add it to table
            if ((portValidator.validate(proxyPortString, proxyPortPosition) == 2) && (hostValidator.validate(proxyHostString, proxyHostPosition) == 2))
            {
                if (col == 1)
                {

                    addProxyItem = new QTableWidgetItem();
                    //addProxyItem->setFlags(addProxyItem->flags() ^ Qt::ItemIsEditable);
                    addProxyItem->setText(QString::number(server.port()));

                    ui->tableWidget_Proxy->setItem(row, col, addProxyItem);
                }

            }


            if ((portValidator.validate(proxyPortString, proxyPortPosition) == 2) && (hostValidator.validate(proxyHostString, proxyHostPosition) == 2))
            {

                if (col == 0)
                {

                    addProxyItem = new QTableWidgetItem();
                    //addProxyItem->setFlags(addProxyItem->flags() ^ Qt::ItemIsEditable);
                    addProxyItem->setText(server.host());

                    ui->tableWidget_Proxy->setItem(row, col, addProxyItem);

                }
            }

        }// end of inner loop


         // if i is equal to or greater than the prozyServer size, delete pointer because were done with it
        if (row >= proxyServers->size())
        {

            delete addProxyItem;
        }

    }// outer for loop

    ui->tableWidget_Proxy->setHorizontalHeaderLabels(proxyTableHeaders);
    ui->tableWidget_Proxy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Proxy->resizeRowsToContents();

}

void MainWindow::on_pushButton_Next_Email_Pagination_clicked()
{

    (*nextEmailPaginationPtr) += 8;
    if (*nextEmailPaginationPtr >= 16)
    {
        (*previousEmailPaginationPtr) += 8;
    }
    connOpen();
    QSqlQuery *qry = new QSqlQuery(mydb);


    if(!qry->exec("select * from crawleremails LIMIT '"+QString::number(*previousEmailPaginationPtr)
                  +"','"+QString::number(*nextEmailPaginationPtr)+"'"))
    {
        qDebug() << "Error selecting rows from crawleremails:: Line 1246 " << qry->lastError().text();
        return;
    }
    int fieldNo = qry->record().indexOf("email");
    int num = 0;
    while(qry->next())
    {
     emailsInTable <<   qry->value(fieldNo).toString();
     num++;
    }
   // qDebug() << emailsInTable;

    for(int i =0; i  <queryModel->rowCount(); i++)
    {
     //  ui->tableView_Emails->hideRow(i);
    }

    if(*previousEmailPaginationPtr >= queryModel->rowCount())
    {
       // ui->pushButton_Next_Email_Pagination->setEnabled(false);
    }

   // qDebug() << " query model count-->" <<queryModel->rowCount();
   // qDebug() << "NEXT " << *nextEmailPaginationPtr;
  //  qDebug() << "PREVIOUS" << *previousEmailPaginationPtr;
    queryModel->setQuery(*qry);
    for(int i =0; i  <queryModel->rowCount(); i++)
    {
        QAbstractItemModel * model = ui->tableView_Emails->model();
        QModelIndex index1 = model->index(i,0,QModelIndex());
        QString emailsFromDataBase = ui->tableView_Emails->model()->data(index1).toString();
       // qDebug() << ui->tableView_Emails->model()->rem
        qDebug() << i;

        for(int j =(*previousEmailPaginationPtr); j  <(*nextEmailPaginationPtr); j++)
        {
            QAbstractItemModel * model = ui->tableView_Emails->model();
            QModelIndex index2 = model->index(j,0,QModelIndex());
            QString emailsWithinPagination =  ui->tableView_Emails->model()->data(index2).toString();
            qDebug() << ui->tableView_Emails->model()->data(index2).toString();

            if(emailsFromDataBase != emailsWithinPagination)
            {
                ui->tableView_Emails->model()->removeRow(i);
            }

        }

    }

    ui->tableView_Emails->setModel(queryModel);
//    for(int i =(*previousEmailPaginationPtr); i  <(*nextEmailPaginationPtr); i++)
//    {
//        QAbstractItemModel * model = ui->tableView_Emails->model();
//        QModelIndex index = model->index(i,0,QModelIndex());
//        qDebug() << ui->tableView_Emails->model()->data(index).toString();
//    }
    connClose();
    ui->tableView_Emails->resizeColumnsToContents();
    delete qry;


//    qDebug() << "NEXT " << *nextEmailPaginationPtr;
//    qDebug() << "PREVIOUS" << *previousEmailPaginationPtr;

}

void MainWindow::on_pushButton_Previous_Email_Pagination_clicked()
{
    if (*previousEmailPaginationPtr >= 8) {
        (*previousEmailPaginationPtr) -= 8;
        (*nextEmailPaginationPtr) -= 8;
    }

    connOpen();
    QSqlQuery *qry = new QSqlQuery(mydb);

    for(int i =0; i< emailsInTable.size(); i++)
    {
        if(!qry->exec("select * from crawleremails where email != '"+emailsInTable.value(i)+"' limit '"+QString::number(*previousEmailPaginationPtr)
                      +"','"+QString::number(*nextEmailPaginationPtr)+"'"))
        {
            qDebug() << "Error selecting rows from crawleremails:: Line 1246 " << qry->lastError().text();
            return;
        }
    }





    for(int i =(*previousEmailPaginationPtr); i  <(*nextEmailPaginationPtr); i++)
    {
        QAbstractItemModel * model = ui->tableView_Emails->model();
        QModelIndex index = model->index(i,0,QModelIndex());
        qDebug() << ui->tableView_Emails->model()->data(index).toString();
    }
   // qDebug() << qry->
    qDebug() << " query model count-->" <<queryModel->rowCount();
    qDebug() << "NEXT " << *nextEmailPaginationPtr;
    qDebug() << "PREVIOUS" << *previousEmailPaginationPtr;
    queryModel->setQuery(*qry);
    ui->tableView_Emails->setModel(queryModel);
    emailsInTable.clear();
    connClose();
    ui->tableView_Emails->resizeColumnsToContents();
    delete qry;



}

void MainWindow::receiverEmailTableModel(QSqlQueryModel * queryModel)
{
    ui->tableView_Emails->setModel(queryModel);
    ui->tableView_Emails->resizeRowsToContents();
}

void MainWindow::populateEmailTable() {

    connOpen();
    QSqlQuery *qry = new QSqlQuery(mydb);
    if(ui->checkBox_Exclude_Email_Duplicates->isChecked())
    {
        if(!qry->exec("select distinct email from crawleremails"))
        {
            qDebug() << "Error selecting rows from crawleremails in mainwindow populatetable " << qry->lastError().text();
            return;
        }
    }
    if(!ui->checkBox_Exclude_Email_Duplicates->isChecked())
    {
        if(!qry->exec("select * from crawleremails"))
        {
            qDebug() << "Error selecting rows from crawleremails in mainwindow populatetable " << qry->lastError().text();
            return;
        }
    }

    int fieldNo = qry->record().indexOf("email");
    int num =0;
    while(qry->next())
    {
       // QApplication::processEvents();
        //qDebug() << qry->value(fieldNo).toString();
        QStandardItem *item = new QStandardItem(qry->value(fieldNo).toString());
        emailTableModel->setItem(num, 0, item);
        getEmailList << qry->value(fieldNo).toString();
        autoMailerEmailList << qry->value(fieldNo).toString();
        QEventLoop loop;
        QTimer::singleShot(1000,&loop,SLOT(quit()));
        loop.exec();
        if(ui->checkBox_Delete_Emails->isChecked())
        {
            // From our understanding, we may not need to delete this item if were using clear() method
           // delete item;
        }
        if(!clickedStartStopButton)
        {
            break;
        }
        num++;
    }

    ui->tableView_Emails->setModel(emailTableModel);
    ui->tableView_Emails->show();

    delete qry;
    connClose();

}

void MainWindow::recieverProxyTableSelection(const QItemSelection &selected, const QItemSelection &deselected)

{


    /**********
    selected which is a type of QItemSelection object
    is a type, which is QModelIndexList, this type gives us a method-
    called indexes(), which return a type of  QModelIndexList object.
    This object holds nested objects, objects within objects.
    These inner objects are type QModelIndex
    So we have to iterate/loop through the QModelIndex;

    We can do that by a foreach loop.
    We create a type of QModelIndex that will be a object.
    Through each iteration/loop we assign the inner objects to &index
    which is a reference. We then can now access the QModexlIndexes,
    which were once nested.

    *******/

    QModelIndexList  indexes = selected.indexes();
    if (ui->checkBox_Delete_Proxy->isChecked()) {
        foreach(const QModelIndex &index, indexes)
        {
            // removes row user has selected
            ui->tableWidget_Proxy->removeRow(index.row());
            // removes proxy servers from qstring list, if user has added proxies
            proxyServers->removeAt(index.row());
        }
    }

}


void MainWindow::connOpen()
{
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = getRelativePath("emails.db");

    mydb.setDatabaseName(dbPath);
    if(!mydb.open())
    {
        qDebug() << "Error connecting to database";
        return;
    }else
    {
      qDebug() << "Successfully connected to database";
    }
}

void MainWindow::connClose()
{
    mydb.close();
    mydb.removeDatabase(QSqlDatabase::defaultConnection);
}


/*****Slot that handles delete emails checkbox*****/
void MainWindow::on_checkBox_Delete_Emails_clicked()
{
    connOpen();
    QSqlQuery *qry = new QSqlQuery(mydb);
    if(!qry->exec("select * from  crawleremails"))
    {
        qDebug() << "Error checking if emails are in dabase "
                 << "on_checkBox_Delete_Emails_clicked():: "<< qry->lastError().text();
        return;
    }

    queryModel->setQuery(*qry);
    //user can only delete emails if only there are emails in database
    if (ui->checkBox_Delete_Emails->isChecked())
    {
        if (queryModel->rowCount() !=0) {

            ui->checkBox_Delete_Emails->setDisabled(true);
            ui->pushButton_Start->setDisabled(true);
            ui->pushButton_Load_Keyword_List->setDisabled(true);

            QTimer::singleShot(10, this, SLOT(deleteEmailsListTable()));
            ui->label_Items_Found->setText("Items Found: ");
        }

    }
    connClose();
    delete qry;
}

/******Slot that deletes all emails from database*****/
void MainWindow::deleteEmailsListTable()
{


    connOpen();
    QSqlQuery *qry = new QSqlQuery(mydb);
    if(!qry->exec("delete from crawleremails"))
    {
        qDebug() << "Error deleting emails from crawleremails " << qry->lastError().text();
        return;
    }
    if(!qry->exec("delete from crawlermaileremails"))
    {
        qDebug() << "Error deleting emails from crawlermaileremails " << qry->lastError().text();
        return;
    }

    if(!qry->exec("UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name ='crawlermaileremails'"))
    {
        qDebug() << "Error sqlite sequence from crawlermaileremails " << qry->lastError().text();
        return;
    }

    queryModel->setQuery(*qry);
    ui->tableView_Emails->setModel(queryModel);
    ui->tableView_Emails->resizeRowsToContents();
    connClose();
    emailTableModel->clear();
    emailListCount.clear();
    getEmailList.clear();
    delete qry;
    emit emitRemoveEmailList();


}

/*******Check to see if emails were deleted from database********/
void MainWindow::receiverRemoveEmailList()
{

    connOpen();
    QSqlQuery *qry = new QSqlQuery(mydb);
    if(!qry->exec("select * from crawleremails"))
    {
        qDebug() << "Error checking if emails are deleted " << qry->lastError().text();
        return;
    }
    queryModel->setQuery(*qry);

    for (;;) {
        QEventLoop loop;
        QTimer::singleShot(5000, &loop, SLOT(quit()));
        loop.exec();
        ui->pushButton_Start->hide();
        if (queryModel->rowCount() == 0)
        {
            emit emitSenderEnableDeleteEmailCheckBox();
            break;
        }
    }
    delete qry;
    connClose();

}

/******Receives signal to enable email delete checkbox after emails are deleted******/
void MainWindow::receiverEnableDeleteEmailCheckBox()
{

    ui->pushButton_Load_Keyword_List->setEnabled(true);
    ui->checkBox_Delete_Emails->setEnabled(true);
    ui->pushButton_Start->setEnabled(true);
    ui->pushButton_Start->show();

}



/*****Slot that handles delete keywords checkbox*****/
void MainWindow::on_checkBox_Delete_Keywords_clicked()
{

    if (ui->checkBox_Delete_Keywords->isChecked())
    {
        if ((!fileList->isEmpty() && ui->radioButton_Parse_1_URL->isChecked()) ||(( ui->radioButton_Parse_Multi_URL->isChecked())  &&
                !fileList->isEmpty() && !multi1KeywordList.isEmpty() && !multi2KeywordList.isEmpty()))
        {

            ui->checkBox_Delete_Emails->setEnabled(false);
            ui->pushButton_Start->setDisabled(true);
            ui->pushButton_Load_Keyword_List->setDisabled(true);
            ui->checkBox_Delete_Keywords->setDisabled(true);
            QTimer::singleShot(10, this, SLOT(deleteKeyordsListTable()));
            ui->label_Items_Found->setText("Items Found: ");

        }

    }
}



/******Slot that deletes all keywords from database*****/
void MainWindow::deleteKeyordsListTable() {

    if (!fileList->isEmpty())
    {
        for (int row = 0; row < fileList->size(); row++)
        {
            for (int col = 0; col < 2; col++)
            {
                ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(""));
            }
        }

        //clear all keyword list
        fileList->clear();
        multi1KeywordList.clear();
        multi2KeywordList.clear();
    }
    emit emitRemoveKeywordList();

    ui->lineEdit_Keyword_List_File_Location->setText(" ");
}

/*******Check to see if keywords were deleted from database********/
void MainWindow::receiverRemoveKeywordList()
{



    for (;;) {
        QEventLoop loop;
        QTimer::singleShot(5000, &loop, SLOT(quit()));
        loop.exec();
        ui->pushButton_Start->setDisabled(true);
        ui->pushButton_Load_Keyword_List->setDisabled(true);
        ui->checkBox_Delete_Keywords->setDisabled(true);


        if (fileList->isEmpty() && multi1KeywordList.isEmpty() && multi2KeywordList.isEmpty())
        {
            emit emitSenderEnableKeywordCheckBox();
            break;
        }
    }

}

/******Receives signal to enable keywords delete checkbox after emails are deleted******/
void MainWindow::receiverEnableDeleteKeywordCheckBox()
{

    ui->checkBox_Delete_Keywords->setEnabled(true);
    ui->pushButton_Load_Keyword_List->setEnabled(true);
    ui->pushButton_Start->setEnabled(true);
}


void MainWindow::on_pushButton_Queue_Keywords_Table_clicked()
{

    queueKeywords->setModal(true);
    queueKeywords->exec();

}

void MainWindow::receiverLogHarvesterStatus(QString logStatus)

{
    if(clickedStartStopButton==false)
    {
        ui->listWidget_Log_Harvester_Status->clear();

    }
    if (!logStatus.isEmpty())
    {
        QListWidgetItem *listItem = new QListWidgetItem(logStatus, ui->listWidget_Log_Harvester_Status);
        listItem->setSizeHint(QSize(listItem->sizeHint().width(),20));

        if (*logStatusCounterPtr == 16)
        {
            *logStatusCounterPtr = 0;
            //delete listItem;
            ui->listWidget_Log_Harvester_Status->clear();
        }
        (*logStatusCounterPtr) += 1;
    }
}

void MainWindow::clearFiles()
{
    QString emptyString;

    // cleans complete json file
    QFile completeFile;

    QString completeFileName = getRelativePath("complete.json");


    completeFile.setFileName(completeFileName);

    if(!completeFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug() << "Error opening json file in mainwindow";
        return;
    }
    QTextStream completeFileStream(&completeFile);
    completeFileStream << emptyString;
    completeFile.close();

    // cleans httpStatus log file
    QFile httpStatusLogFile;
    QString httpStatusFileName = getRelativePath("httpstatus.json");


    httpStatusLogFile.setFileName(httpStatusFileName);
    if(!httpStatusLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error opening complete json file in mainwinodw " << httpStatusLogFile.errorString();
        return;
    }
    QTextStream httpStatusLogStream(&httpStatusLogFile);
    httpStatusLogStream << emptyString;
    httpStatusLogFile.close();



    // cleans webview log file
    QFile webViewLogFile;
    QString logFileName= getRelativePath(".log");


    webViewLogFile.setFileName(logFileName);
    if(!webViewLogFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error opening webview json file in mainwinodw " << webViewLogFile.errorString();
        return;
    }
    QTextStream webViewLogFileStream(&webViewLogFile);
    webViewLogFileStream << emptyString;
    webViewLogFile.close();

    // cleans currentKeywords file
    QFile currentKeywordsFile;
    QString currentKeywordsFileName = getRelativePath("currentkeywords.json");


    currentKeywordsFile.setFileName(currentKeywordsFileName);
    if(!currentKeywordsFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error opening current keywords json file in mainwinodw " << currentKeywordsFile.errorString();
        return;
    }
    QTextStream currentKeywordsFileStream(&currentKeywordsFile);
    currentKeywordsFileStream << emptyString;
    currentKeywordsFile.close();


    // cleans params file
    QFile paramsFile;
    QString paramsFileName= getRelativePath("params.json");

    paramsFile.setFileName(paramsFileName);
    if(!paramsFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error opening params json file in mainwinodw " << paramsFile.errorString();
        return;
    }
    QTextStream paramsFileStream(&paramsFile);
    paramsFileStream << emptyString;
    paramsFile.close();


    // cleans harvest status file
    QFile harvesterStatusFile;
    QString harvesterStatusFileName= getRelativePath("harvesterstatus.json");


    harvesterStatusFile.setFileName(harvesterStatusFileName);
    if(!harvesterStatusFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Error opening params json file in mainwinodw " << harvesterStatusFile.errorString();
        return;
    }
    QTextStream harvesterStatusFileStream(&harvesterStatusFile);
    harvesterStatusFileStream << emptyString;
    harvesterStatusFile.close();
}

void MainWindow::on_pushButton_Save_Emails_clicked()
{
    QString savedEmails;
    QStringList savedEmailsList;
    QString fileName = QFileDialog::getSaveFileName(this, "Save Emails", "", "Text files (*.txt)");
    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, "unable to open file", file.errorString());
            return;
        }
        QTextStream outStream(&file);

        for (int i = 0; i < getEmailList.toSet().toList().size(); i++)
        {
            savedEmails = getEmailList.toSet().toList().value(i);
            savedEmailsList << savedEmails.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
            outStream << savedEmailsList.value(i) << "\n";
        }

        file.close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{



        if(clickedStartStopButton == true || clickedStartStopButton ==false)
        {
            QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Beat Crawler",
                                                                        "Are you sure you want to exit?\n",
                                                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                        QMessageBox::Yes);

            if (resBtn != QMessageBox::Yes)
            {
                event->ignore();
            }
            else
            {

                (*emailRowStartPtr) =0;
                (*emailRowEndPtr) =0;
                killWebViewProcess();
                emit emitStopQueueKeywords();
                emit emitStopWorker();
                clearFiles();

                connOpen();
                QSqlQuery *qry = new QSqlQuery(mydb);
                if(!qry->exec("delete from crawleremails"))
                {
                    qDebug() << "Error deleting emails from crawleremails " << qry->lastError().text();
                    return;
                }
                if(!qry->exec("delete from crawlermaileremails"))
                {
                    qDebug() << "Error deleting emails from crawlermaileremails " << qry->lastError().text();
                    return;
                }
                if(!qry->exec("UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name ='crawlermaileremails'"))
                {
                    qDebug() << "Error sqlite sequence from crawlermaileremails " << qry->lastError().text();
                    return;
                }


                queryModel->setQuery(*qry);
                connClose();
                delete qry;
                killBeatCrawlerProcess();
                killWebViewProcess();

                event->accept();

            }

        }
        else
        {

        }

}

QString MainWindow::getRelativePath(QString fileName)
{
   // QDir dir("resources");
   // QString filename = dir.relativeFilePath(fileName);
   // return fileName;
   // qDebug()<<  "r--> "<< QDir::current().path() +"/resources/"+ fileName;
    //qDebug() <<  "a--> " << QDir::current().cd(QDir::current().path() +QString("/resources/"));
   // qDebug() << QCoreApplication::applicationDirPath()+ "/resources/";
    //C:/Users/ace/Documents/QT_Projects/WebView/build-WebView-Desktop_Qt_5_9_4_MSVC2015_64bit2-Debug/debug/resources/
    //return QCoreApplication::applicationDirPath()+ "/resources/"+ fileName;

    QString userName =  qgetenv("USERNAME");
    QString crawlerAppDataResourcesDir =  QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QString dir = "C:/Users/" + userName+"/AppData/Local/BeatCrawler/resources/";
    QDir resourcesDir(dir);
    QStringList args;
    if(RESOURCES_DIR == "RELEASE")
    {


        if(resourcesDir.exists())
        {
            return resourcesDir.path() + "/" +fileName;
        }

    }else
    {
        return "C:/Users/ace/Documents/QT_Projects/WebView/build-WebView-Desktop_Qt_5_9_4_MSVC2015_32bit2-Release/release/" + fileName;

    }


}


bool MainWindow::findSpintax(char *s, char *&from, char *&to)
{
    if(!s) return false;
    from = s;
    char *temp = to = NULL;
    while(!to)
    {
        if(!(from = strchr(from,     '{'))) return false; // Find opening bracket
        if(!(to   = strchr(from,     '}'))) return false; // Find closing bracket
             temp = strchr(from + 1, '{');                // Check if there's any other opening bracket between the first one and closing bracket
        if(temp && temp < to)                             // If so start over from that other bracket
        {
            from = temp;
            to = NULL;
        }
        else return true;                                 // Otherwise lets say we've got it
    }
    return false;
}

void MainWindow::_textSpin(char *from, char *to)
{
    unsigned element = 0;
    char *pos1 = from, *pos2 = NULL;
    while((pos1 = strchr(pos1 + 1, '|')) && pos1 < to) ++element;
    element = rand() % (element + 1);
    pos1 = from;
    for(unsigned i = 0; i < element; ++i) pos1 = strchr(pos1 + 1, '|');
    ++pos1; pos2 = strchr(pos1, '|');
    if(!pos2 || pos2 >= to) pos2 = to;
    --pos2;
    strcpy(from, pos1);
    strcpy(pos2 + 1 - (pos1 - from), to - (pos1 - from) + 1);
}

void MainWindow::textSpin(char *s)
{
    if(!s) return;
    char *from, *to;
    while(findSpintax(s, from, to)) _textSpin(from, to);
}

void MainWindow::sendTestEmail()
{
            QByteArray mailerTextBody = ui->plainTextEdit_Mailer_Body->toPlainText().toLatin1();
            textSpin( mailerTextBody.data());

            QByteArray mailerTextSubject = ui->lineEdit_Mailer_Subject->text().toLatin1();
            textSpin( mailerTextSubject.data());


            Smtp* smtp = new Smtp(ui->lineEdit_Mailer_Login->text(), ui->lineEdit_Mailer_Password->text(),
                           ui->lineEdit_Mailer_SMTP_Server->text(), ui->spinBox_Mailer_SMTP_Port->text().toInt());
            connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
            smtp->sendMail(ui->lineEdit_Mailer_Login->text(), ui->lineEdit_Mailer_Email_Provider->text() ,
                           mailerTextSubject.data(),mailerTextBody.data());
            mailerEmailSent = ui->lineEdit_Mailer_Email_Provider->text();
           // qDebug() << ba.data();

            ui->textBrowser_Preview_Mailer_Message->setPlainText
                    (
                      // QString("From: ") + ui->lineEdit_Mailer_Login->text() + "\n" +
                       QString("Subject: ") + mailerTextSubject.data() +"\n\n"+
                       mailerTextBody.data()

                    );

}

void MainWindow::sendMail()
{

    QStringList emailsSent;
    QHash<QString, int> emailListSentHash;
    int trackSentEmails =1;


    for(;;){


        connOpen();
        QSqlQuery *qry = new QSqlQuery(mydb);
        if(!qry->exec("select * from crawlermaileremails where id= '"+ QString::number(trackSentEmails)+"' "))
        {
            qDebug() << "Error selecting rows from crawleremails in mainwindow sendEmail " << qry->lastError().text();
            return;
        }
        QEventLoop loop;

        if(ui->checkBox_Mailer_Timer_Delayed->isChecked())
        {
            QTimer::singleShot(ui->checkBox_Mailer_Timer_Delayed->text().toInt()*1000,&loop,SLOT(quit()));

        }
        if(!ui->checkBox_Mailer_Timer_Delayed->isChecked())
        {
            QTimer::singleShot(5000,&loop,SLOT(quit()));

        }
        loop.exec();
       // qDebug() << "track emails counter " << trackSentEmails;
        // If theres more results, send email, and increment counter
        while(qry->next())
        {


//            for(unsigned i = 0; i < 20; ++i)
//            {
//                char foo[] = "This a Spintax {demo|demonstration}. It{{'s| is} showing| shows} how it works";
//                textSpin(foo);

//                qDebug() <<  "Result " << i << foo;
//            }
            QByteArray mailerTextBody = ui->plainTextEdit_Mailer_Body->toPlainText().toLatin1();
            textSpin( mailerTextBody.data());

            QByteArray mailerTextSubject = ui->lineEdit_Mailer_Subject->text().toLatin1();
            textSpin( mailerTextSubject.data());


            Smtp* smtp = new Smtp(ui->lineEdit_Mailer_Login->text(), ui->lineEdit_Mailer_Password->text(),
                           ui->lineEdit_Mailer_SMTP_Server->text(), ui->spinBox_Mailer_SMTP_Port->text().toInt());
            connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
            smtp->sendMail(ui->lineEdit_Mailer_Login->text(), qry->value(1).toString() ,
                           mailerTextSubject.data(),mailerTextBody.data());
           // qDebug() << ba.data();

            ui->textBrowser_Preview_Mailer_Message->setPlainText
                    (
                      // QString("From: ") + ui->lineEdit_Mailer_Login->text() + "\n" +
                       QString("Subject: ") + mailerTextSubject.data() +"\n\n"+
                       mailerTextBody.data()

                    );
           mailerEmailSent = qry->value(1).toString();
           qDebug() << qry->value(1).toString();
           trackSentEmails++;

           if(!clickedStartStopButton)
           {
               break;
           }

        }

        if(!clickedStartStopButton)
        {
            break;
        }
        delete qry;
        connClose();

}







}

void MainWindow::mailSent(QString status)
{

    if(status == "Message sent")
    {
       // mailerLog.append(status);
       ui->listWidget_Mailer_Log->addItem(status +" to "+ QString(mailerEmailSent));
    }
    else
    {
       //ui->listWidget_Mailer_Log->addItem(status);
    }

}

void MainWindow::on_pushButton_Mailer_Test_clicked()
{
   ui->pushButton_Mailer_Test->setDisabled(true);
   QTimer::singleShot(4000, this, SLOT(reEnableMailerTestButton()));

   sendTestEmail();

}

void MainWindow::reEnableMailerTestButton() {
    ui->pushButton_Mailer_Test->setDisabled(false);
}


void MainWindow::killWebViewProcess()
{
    QProcess::execute("taskkill /im WebView.exe /f");
    webViewProcess->close();

}

void MainWindow::killBeatCrawlerProcess()
{

    QProcess::execute("taskkill /im BeatCrawler.exe /f");
    close();
}

