#include "beatcrawler.h"
#include "ui_beatcrawler.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "options.h"
#include <QUrl>
#include <QToolTip>
#include <QEvent>
#include <QMouseEvent>
#include <QHash>
#include <QVector>
#include <memory>
#include <string>
#include <cstdio>
#include <QTextDocumentFragment>
#include <QList>
#include <QModelIndex>
#include <stdio.h>
#include <string.h>
#include <QTextCodec>
#include <QCoreApplication>


QStringList* BeatCrawler::emails;
QString* BeatCrawler::fetchWriteCallBackCurlData;


BeatCrawler::BeatCrawler(QWidget *parent) :

	QMainWindow(parent),
	ui(new Ui::BeatCrawler)

{

	ui->setupUi(this);

	QSize size = this->size();
	this->setFixedSize(size);
	setSearchResults();
	setProxyTable();


	setWindowTitle("Beat Crawler V0.1.9 (C) Beatcrawler.com");
	ui->lineEdit_keywords_search_box->setPlaceholderText("my mixtape");
	fetchWriteCallBackCurlDataString = "";
	BeatCrawler::fetchWriteCallBackCurlData = &fetchWriteCallBackCurlDataString;


	// The thread and the worker are created in the constructor so it is always safe to delete them.
	timer = new QTimer();
	keywordsQueueTableTimer = new QTimer();
	thread = new QThread();
	thread1 = new QThread();
	worker = new Worker();
	worker->moveToThread(thread);

	connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
	//connect(this, SIGNAL(emitSenderReadFile(QString)), fileReader, SLOT(receiverReadFile(QString)));
	//connect(&threadWorker,&Worker::emitDataTest, this,&BeatCrawler::receiverDataTest);
	connect(&threadWorker, &Worker::emitFinishSenderHarvestResults, this, &BeatCrawler::receiverFinishHarvestResults);
	//connect(worker, SIGNAL(workRequested()), thread, SLOT(start()));
	connect(timer, SIGNAL(timeout()), this, SLOT(populateEmailTable()));

	connect(&threadWorker, &Worker::emitfinishReadingKeywordFile, this, &BeatCrawler::recieverFinishReadingKeywordFile);

	connect(this, &BeatCrawler::emitRemoveThreadFileList, &threadWorker, &Worker::receiverRemoveThreadFileList);
	connect(&threadWorker, &Worker::emitsenderEnableDeleteKeywordCheckBox, this, &BeatCrawler::receiverEnableDeleteKeywordCheckBox);


	connect(this, &BeatCrawler::emitRemoveThreadEmailList, &threadWorker, &Worker::receiverRemoveThreadEmailList);
	connect(&threadWorker, &Worker::emitsenderEnableDeleteEmailCheckBox, this, &BeatCrawler::receiverEnableDeleteEmailCheckBox);

	connect(&threadWorker, &Worker::emitEmailList1, this, &BeatCrawler::receiverEmailList);

	connect(&threadWorker, &Worker::senderCurlResponseInfo, this, &BeatCrawler::recieverCurlResponseInfo);

	connect(&threadWorker, &Worker::emitsendCurrentKeyword, this, &BeatCrawler::receiverCurrentKeyword);

	connect(&threadWorker, &Worker::emitSenderLogHarvesterStatus, this, &BeatCrawler::receiverLogHarvesterStatus);

	//connect(keywordsQueueTableTimer, SIGNAL(timeout()), this, SLOT(keywordsQueueTable()));

	connect(this, &BeatCrawler::on_stop, &threadWorker, &Worker::stop);



	//emitsenderEnableDeleteEmailCheckBox()

	//populateEmailTable();


	emailList = new QList <QString>();
	proxyServers = new QList <QString>();
	options = new OptionsPtr[numOptions];
	for (int i = 0; i < numOptions; i++) {
		options[i] = new Options();
	}
	emailTableModel = new QStandardItemModel();
	fileList = new QStringList();

	currentKeywordString_ = "";
	currentKeyword_ = &currentKeywordString_;
	// ui->lineEdit_keywords_search_box->installEventFilter(this);

	/******

	QStringList emailTableHeaders;
	emailTableHeaders << "Emails";
	ui->tableWidget_Emails->setRowCount(8);
	ui->tableWidget_Emails->setColumnCount(1);
	ui->tableWidget_Emails->setHorizontalHeaderLabels(emailTableHeaders);
	ui->tableWidget_Emails->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget_Emails->resizeRowsToContents();
	ui->tableWidget_Emails->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	********/


	ui->pushButton_Next_Email_Pagination->setEnabled(false);
	ui->pushButton_Previous_Email_Pagination->setEnabled(false);


	ui->tableWidget_Proxy->setSelectionBehavior(QAbstractItemView::SelectRows);

	QStringList keywordQueueTableHeaders;
	keywordQueueTableHeaders << "Keywords" << "Status";
	ui->tableWidget_Keywords_Queue->setRowCount(40000);
	ui->tableWidget_Keywords_Queue->setColumnCount(2);
	ui->tableWidget_Keywords_Queue->setHorizontalHeaderLabels(keywordQueueTableHeaders);
	ui->tableWidget_Keywords_Queue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableWidget_Keywords_Queue->resizeRowsToContents();


	QStringList emailTableModelHeaderLabels;
	emailTableModelHeaderLabels << "Emails";
	emailTableModel->setHorizontalHeaderLabels(emailTableModelHeaderLabels);
	emailTableModel->setRowCount(8);
	emailTableModel->setColumnCount(0);
	ui->tableView_Emails->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int row = 0; row < 8; ++row) {
		for (int column = 0; column < 1; ++column) {
			QStandardItem *item = new QStandardItem(" ");
			emailTableModel->setItem(row, column, item);

			delete item;

		}
	}

	ui->tableView_Emails->setModel(emailTableModel);
	ui->tableView_Emails->show();
	ui->tableView_Emails->resizeRowsToContents();





	ui->pushButton_Start->setCheckable(true);
	ui->checkBox_Bing->setChecked(true);
	ui->checkBox_Email_Gmail->setChecked(true);
	ui->checkBox_Email_Hotmail->setChecked(true);
	ui->checkBox_Email_Yahoo->setChecked(true);
	ui->checkBox_Google->setChecked(true);
	ui->checkBox_Social_Facebook->setChecked(true);
	ui->checkBox_Social_Instagram->setChecked(true);
	ui->checkBox_Social_Myspace->setChecked(true);
	ui->checkBox_Social_Reverbnation->setChecked(true);
	ui->checkBox_Social_Soundcloud->setChecked(true);
	ui->checkBox_Yahoo->setChecked(true);
	ui->radioButton_Android_Webkit->setChecked(true);

	ui->lineEdit_Keyword_List_File_Location->setEnabled(false);



	emailOptionsNum = 0;
	socialNetWorkNum = 0;
	keywordLoadListOptionsNum = 0;
	searchEngineNumPtrAddedBool = false;
	currentKeyword = "";
	currentKeywordPtr = &currentKeyword;


	// increments search engine pagination query on each interval

	keywordSearchBoxSearchEngineCounterNum = 0;
	keywordListSearchEngineCounterNum = 0;


	keywordSearchBoxSearchEngineCounterPtr = &keywordSearchBoxSearchEngineCounterNum;
	keywordListSearchEngineCounterPtr = &keywordListSearchEngineCounterNum;

	emailOptionsNumPtr = &emailOptionsNum;
	searchEngineNumPtrNum = 0;
	searchEngineNumPtrCounter = &searchEngineNumPtrNum;


	searchEngineNum = 0;
	searchEngineNumPtr = &searchEngineNum;

	socialNetWorkNumPtr = &socialNetWorkNum;


	keywordLoadListOptionsNumPtr = &keywordLoadListOptionsNum;
	searchEngineNumPtrAdded = &searchEngineNumPtrAddedBool;

	keywordBoxNumPtrNum = 0;
	keywordBoxNumPtrCounter = &keywordBoxNumPtrNum;

	keywordListNumPtrNum = 0;
	keywordListNumPtrCounter = &keywordListNumPtrNum;

	nextEmailPagination = 0;
	nextEmailPaginationPtr = &nextEmailPagination;

	previousEmailPagination = 0;
	previousEmailPaginationPtr = &previousEmailPagination;

	addProxyCounterNum = 0;
	addProxyCounterPtr = &addProxyCounterNum;

	fileListNum = 0;
	fileListPtr = &fileListNum;


	previousPagesNum = 0;
	previousPagesPtr = &previousPagesNum;

	logStatusCounterNum = 0;
	logStatusCounterPtr = &logStatusCounterNum;



}



BeatCrawler::~BeatCrawler()

{
	delete ui;
	delete options;
	delete emailList;
	delete proxyServers;
	thread->wait();
	qDebug() << "Deleting thread and worker in Thread " << this->QObject::thread()->currentThreadId();
	delete thread;
	delete worker;
	delete fileList;
	delete timer;
	delete keywordsQueueTableTimer;
	delete emailTableModel;
	//fileReader->abort();
	//thread1->wait();
	//delete thread1;
	//delete fileReader;


	/******

	if(keywordQueueItem != NULL)
	{
	delete keywordQueueItem;
	qDebug() << "Not null";

	}
	if(addProxyItem != NULL)
	{
	delete addProxyItem;
	qDebug() << "Not null";
	}
	if (emailListItem != NULL)
	{
	delete emailListItem;
	qDebug() << "Not null";
	}


	****/

}

void BeatCrawler::receiverCurrentKeyword(QString keyword) {
	QString keywrd = keyword;
	ui->label_Current_Keyword->setText("Current Keyword: " + keywrd.replace("+", " "));
	*currentKeyword_ = keywrd.replace("+", " ");
}

void BeatCrawler::receiverFinishHarvestResults(QString s) {
	QString itemsFound;
	if (s == "MULTI COMPLETED") {
		qDebug() << "MULTI COMPLETEDt" << s;
		ui->pushButton_Start->setText("Start");
		//ui->pushButton_Start->setChecked(false);
		// stops user from pressing start to many times in a row, which will lead to problems
		ui->pushButton_Start->setEnabled(false);
		QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));

		ui->lineEdit_keywords_search_box->setEnabled(true);
		ui->pushButton_Save_Emails->setEnabled(true);
		ui->pushButton_Load_Keyword_List->setEnabled(true);

		ui->tabWidget_Harvester_Options->setEnabled(true);
		ui->tableWidget_Proxy->setEnabled(true);
		ui->lineEdit_Proxy_Host->setEnabled(true);
		ui->lineEdit_Proxy_Port->setEnabled(true);
		ui->pushButton_Add_Proxy->setEnabled(true);
		ui->checkBox_Delete_Emails->setEnabled(true);
		ui->checkBox_Delete_Keywords->setEnabled(true);

		fileList->clear();
		itemsFound = ui->label_Items_Found->text();
		QMessageBox::information(this, "...", QString("Emails Harvested: ") + itemsFound);

	}
	if (s == "MULTI_SINGLE COMPLETED") {
		qDebug() << "MULTI_SINGLE COMPLETED" << s;
		ui->pushButton_Start->setText("Start");
		//ui->pushButton_Start->setChecked(false);
		// stops user from pressing start to many times in a row, which will lead to problems
		ui->pushButton_Start->setEnabled(false);
		QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));

		ui->lineEdit_keywords_search_box->setEnabled(true);
		ui->pushButton_Save_Emails->setEnabled(true);
		ui->pushButton_Load_Keyword_List->setEnabled(true);

		ui->tabWidget_Harvester_Options->setEnabled(true);
		ui->tableWidget_Proxy->setEnabled(true);
		ui->lineEdit_Proxy_Host->setEnabled(true);
		ui->lineEdit_Proxy_Port->setEnabled(true);
		ui->pushButton_Add_Proxy->setEnabled(true);
		ui->checkBox_Delete_Emails->setEnabled(true);
		ui->checkBox_Delete_Keywords->setEnabled(true);

		fileList->clear();
		itemsFound = ui->label_Items_Found->text();
		QMessageBox::information(this, "...", QString("Emails Harvested: ") + itemsFound);
	}
}

void BeatCrawler::setProxyTable() {

}

void BeatCrawler::setSearchResults() {
	//search engine results
	for (int i = 1000; i >= 1; i--)
	{
		QString s = QString::number(i);
		ui->comboBox_search_results_amount->addItem(s);
	}

}


void BeatCrawler::on_pushButton_Start_clicked(bool checked)

{
	QStringList keywordQueueTableHeaders;
	if (checked) {

		//check if main options are empty, our program is based around these important options
		// at least one has to be checked. (These will be empty arrays, if at least one isnt checked!)
		if (!ui->checkBox_Google->isChecked() && !ui->checkBox_Yahoo->isChecked() &&
			!ui->checkBox_Bing->isChecked())
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
			&& !ui->checkBox_Email_Yahoo->isChecked())

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

			isMultiAndKeywordBoxSelected = false;
		}
		else {
			isMultiAndKeywordBoxSelected = true;
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
			&& isEmailSelectOne == true && isMultiAndKeywordBoxSelected == true)
		{

			ui->pushButton_Start->setText("Stop");
			//important options are checked so we set the start buttin to setChecked(true)
			ui->pushButton_Start->setChecked(true);

			//Harvester Timer Value
			// Goes in Thread
			emit emitSenderAppOptions(ui->spinBox_Harvester_Timer->value(), ui->spinBox_Proxy_Rotate_Interval->value());

			// if keyword list is not empty, and keywordsearch box isnt empty add
			// the keyword from search box into keyword list hash table
			if (!ui->lineEdit_keywords_search_box->text().isEmpty() && !fileList->isEmpty())
			{
				fileList->prepend(ui->lineEdit_keywords_search_box->text());


			}
			// clears emails queue table if any emails were in it
			keywordQueueTableHeaders << "Keywords" << "Status";
			ui->tableWidget_Keywords_Queue->setHorizontalHeaderLabels(keywordQueueTableHeaders);


			// KEYWORD BOX INPUT
			if (!ui->lineEdit_keywords_search_box->text().isEmpty())
			{
				QMessageBox msgBox;
				QString cleanString = ui->lineEdit_keywords_search_box->text();

				// QString filteredString1 = cleanString.remove(QRegExp(QString::fromUtf8("[^\S+(\s\S+)+$]")));
				// creates a array from string
				//QStringList filteredString2 = filteredString1.split(" ");
				options[3]->keywordSearchBoxOptions[0] = cleanString;

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
			if (ui->checkBox_Email_Gmail->isChecked()) {

				options[1]->emailOptions[0] = "@gmail.com";

			}

			if (ui->checkBox_Email_Yahoo->isChecked()) {

				options[1]->emailOptions[1] = "@yahoo.com";

			}

			if (ui->checkBox_Email_Hotmail->isChecked()) {

				options[1]->emailOptions[2] = "@hotmail.com";

			}



			//SOCIAL NETWORK OPTION

			if (ui->checkBox_Social_Instagram->isChecked()) {

				options[2]->socialNetworkOptions[0] = "site:instagram.com";

			}

			if (ui->checkBox_Social_Facebook->isChecked()) {

				options[2]->socialNetworkOptions[1] = "site:facebook.com";

			}

			if (ui->checkBox_Social_Soundcloud->isChecked()) {

				options[2]->socialNetworkOptions[2] = "site:soundcloud.com";

			}



			if (ui->checkBox_Social_Reverbnation->isChecked()) {

				options[2]->socialNetworkOptions[3] = "site:reverbnation.com";

			}



			if (ui->checkBox_Social_Myspace->isChecked()) {

				options[2]->socialNetworkOptions[4] = "site:myspace.com";

			}



			//USER AGENT OPTIONS

			if (ui->radioButton_Android_Webkit->isChecked()) {

				options[5]->userAgentsOptions[0] = "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30";

			}



			if (ui->radioButton_Blackberry->isChecked()) {

				options[5]->userAgentsOptions[0] = "Mozilla/5.0 (BlackBerry; U; BlackBerry 9900; en) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.1.0.346 Mobile Safari/534.11+";

			}



			if (ui->radioButton_IE_Mobile->isChecked()) {

				options[5]->userAgentsOptions[0] = "Mozilla/5.0 (compatible; MSIE 9.0; Windows Phone OS 7.5; Trident/5.0; IEMobile/9.0)";

			}


			if (!proxyServers->isEmpty())
			{
				//options[6]->proxyServers[0] = proxyServers;

			}





			/*********XXX3*****/

			//SEARCH RESULTS COMBO BOX

			if (!ui->comboBox_search_results_amount->currentText().isEmpty()) {

				// ui->comboBox_search_results_amount->currentText()

			}



			// To avoid having two threads running simultaneously, the previous thread is aborted.
			//worker->abort();
			// If the thread is not running, this will immediately return.
			//thread->wait();
			//worker->requestWork();


			emit emitsenderStartThreadCounters("Start");
			ui->label_Curl_Status->setText("Status: Starting...");
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

			ui->pushButton_Next_Email_Pagination->setEnabled(false);
			ui->pushButton_Previous_Email_Pagination->setEnabled(false);


			receiverParameters();
			// initialize email table -- checks to see if emails are in tabl
			timer->start(5000);

			// stops user from pressing start to many times in a row, which will lead to problems
			ui->pushButton_Start->setEnabled(false);
			QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));
			//keywordsQueueTable();


		} //else important options are not checked so we set the start buttin to setChecked(false)
		else
		{
			ui->pushButton_Start->setChecked(false);
		}

	} // end if checked



	if (!checked) {

		//worker->abort();
		//thread->quit();
		emit on_stop();
		emit emitsenderStopThreadCounters("Stop");
		ui->label_Curl_Status->setText("Status: ");
		clickedStartStopButton = false;
		ui->pushButton_Start->setText("Start");
		*searchEngineNumPtr = 0;
		*searchEngineNumPtrCounter = 0;
		*keywordSearchBoxSearchEngineCounterPtr = 0;
		*keywordListSearchEngineCounterPtr = 0;
		*keywordListNumPtrCounter = 0;
		*keywordBoxNumPtrCounter = 0;
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
		if (emailList->isEmpty()) {
			ui->pushButton_Next_Email_Pagination->setEnabled(false);
			ui->pushButton_Previous_Email_Pagination->setEnabled(false);
		}
		else {
			ui->pushButton_Next_Email_Pagination->setEnabled(true);
			ui->pushButton_Previous_Email_Pagination->setEnabled(true);
		}



		// stops user from pressing start to many times in a row, which will lead to problems
		ui->pushButton_Start->setEnabled(false);
		QTimer::singleShot(4000, this, SLOT(reEnableStartButton()));

	}
}

void BeatCrawler::reEnableStartButton() {
	ui->pushButton_Start->setEnabled(true);

}

void::BeatCrawler::getKeywordsSearchBoxOrList() {

	//changeBool = true;

	//startScrape = &changeBool;

	if (*startScrape) {



	}
	else {

		qDebug() << "Not Scraping";

	}

}


void BeatCrawler::receiverFileReadKeywordList(QString fileName, QString data, int keywordSize)
{
	/*********
	By default, we cant start harvesing, or initiate the start button
	unless the keywordlist is NOT empty. So by default we need to insert
	a defautlt key, and value into the hash table. Then we can remove it.

	*****/
	if (options[4]->keywordLoadListOptions.isEmpty()) {
		//options[4]->keywordLoadListOptions["Default"] = 0;
		//options[4]->keywordLoadListOptions.remove("Default");


	}
	if (options[4]->keywordLoadListOptions.value(data) == 1) {
		//options[4]->keywordLoadListOptions.remove(data);

	}
	else {
		options[4]->keywordLoadListOptions[data] = 0;
		//qDebug() << "Not Tagged --->" << data;
	}
	//ui->lineEdit_Keyword_List_File_Location->setText(fileName);
	//ui->label_Keywords_Count->setText("Keyword Count: " + QString::number(keywordSize));
	//ui->pushButton_Start->setEnabled(true);

}


void BeatCrawler::on_pushButton_Load_Keyword_List_clicked()

{





	// To avoid having two threads running simultaneously, the previous thread is aborted.
	// If the thread is not running, this will immediately return.
	//if (thread1->isRunning())
	//{

	//connect(thread1, SIGNAL(started()), thread1, SLOT(quit()));
	//fileReader->abort();

	//}


	// If thread is not running we can start to emit data to our thread
	//if (!thread1->isRunning())

	//{

	// emit a file name with keywords, and receive it within the thread for processing
	//connect(this, SIGNAL(emitSenderReadFile(QString)), fileReader, SLOT(receiverReadFile(QString)));
	//connect(fileReader, SIGNAL(workRequested()), thread1, SLOT(start()));

	/*****
	Disconnect, and Connect the emitSenderFileReadKeywordList signal because we need to connect to it again with fresh data
	***/

	//}



	//ui->tableWidget_Keywords_Queue->clear();
	//ui->tableWidget_Keywords_Queue->setRowCount(0);
	//ui->tableWidget_Keywords_Queue->setColumnCount(0);


	//options[4]->keywordLoadListOptions.clear();
	//QString fileName = QFileDialog::getOpenFileName(this, "Open text file", "");
	//QFile file(fileName);
	//QString fileLine;
	//QFileInfo fi(file.fileName());
	//QString fileExt = fi.completeSuffix();
	//qDebug() << " NOT RUNNING";
	//fileReader->requestWork();


	/*****If file is a text file,
	We emit a signal from main thread to fileReader thread that sends the fileName we want to read and process.
	We also connect a signal from fileReader thread that sends out Keywords that we processed back to main thread****/
	//if (fileExt == "txt") {
	//connect(fileReader, SIGNAL(emitSenderFileReadKeywordList(QString, QString, int)), this, SLOT(receiverFileReadKeywordList(QString, QString, int)));

	//emit emitSenderReadFile(fileName);
	// disables start button until we have fully loaded the text file
	// we enable the start button within lineEdit_Keyword_List_File_Location slot because our fileReader
	// thread has sent us our file data back to us fully
	//ui->pushButton_Start->setEnabled(false);
	//}

	//else
	//{
	//QMessageBox::warning(this, "...", "Please select a text file");
	//}

	//file.close();

	//thread1->wait();
	//thread1->quit();

	//ui->lineEdit_Keyword_List_File_Location->setText(" ");
	//ui->label_Keywords_Count->setText("Keyword Count:  " + QString(" "));

	/******************Revised*************/

	for (int row = 0; row < fileList->size(); row++)
	{


		for (int col = 0; col < 2; col++)
		{
			if (col == 0) {
				ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(""));

			}

			if (col == 1) {
				ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(""));
			}
		}

	}// end for loop


	QString fileName = QFileDialog::getOpenFileName(this, "Open text file", "");
	QFile file(fileName);
	QFileInfo fi(file.fileName());
	QString fileExt = fi.completeSuffix();;
	QString strings;
	QString str;
	QString fileLine;


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
		QFuture<void> sendFileName = QtConcurrent::run(&this->threadWorker, &Worker::receiverReadFile, QString(file.fileName()));
		QTextStream ts(&file);
		while (!ts.atEnd()) {
			QApplication::processEvents();
			str = ts.readLine();
			*fileList << str;

		}


		for (int row = 0; row < fileList->size(); row++)
		{
			for (int col = 0; col < 2; col++)
			{
				if (col == 0) {
					ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(fileList->value(row)));

				}

				if (col == 1) {
					ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(""));
				}
			}

		}// end for loop

		ui->lineEdit_Keyword_List_File_Location->setText(file.fileName());
	}
	else // else user needs to select txt file
	{
		QMessageBox::information(this, "...", "Please selext a text file");

	}

	file.close();
}


void BeatCrawler::recieverFinishReadingKeywordFile()
{
	//enables checkboxes when keyword file has finished loading within thread
	ui->pushButton_Load_Keyword_List->setEnabled(true);
	ui->checkBox_Delete_Keywords->setEnabled(true);
	ui->pushButton_Start->setEnabled(true);


}


QString BeatCrawler::toDebug(const QByteArray & line) {

	QString s;
	uchar c;

	for (int i = 0; i < line.size(); i++) {
		c = line[i];
		if (c >= 0x20 && c <= 126) {
			s.append(c);
		}
		else {
			s.append(QString("<%1>").arg(c, 2, 16, QChar('0')));
		}
	}
	return s;
}

void BeatCrawler::disableStartButtonLoadKeywordList() {
	ui->pushButton_Start->setEnabled(false);


}

void BeatCrawler::enableStartButtonLoadKeywordList() {
	ui->pushButton_Start->setEnabled(true);
}


void BeatCrawler::setEmailTable()

{

}

bool BeatCrawler::eventFilter(QObject *watched, QEvent *event) {

	if (watched == ui->lineEdit_keywords_search_box && event->type() == QEvent::MouseButtonPress) {

		/*******
		if we enter keywords in keyword search box, disable keyword load list button
		*****/
		//ui->lineEdit_keywords_search_box->setEnabled(true);
		// ui->pushButton_Load_Keyword_List->setEnabled(false);

		return true;
	}

	return false;

}


void BeatCrawler::mousePressEvent(QMouseEvent *event) {

	//    if we click left button, enable both keyword load list button, and keyword search box
	if (event->button() == Qt::LeftButton)

	{
		// ui->lineEdit_keywords_search_box->setEnabled(true);
		//ui->pushButton_Load_Keyword_List->setEnabled(true);

	}

}


void BeatCrawler::receiverParameters()

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






	/******

	Loops through keyword list, and gets key, and value.

	Remember were tagging each keyword of being busy 1, or not busy 0

	If keyword is busy we move increment i++ to get the next keword in list

	to check its tagged status.

	****/



	// THREAD
	if (!fileList->isEmpty()) {
		*currentKeywordPtr = fileList->value(*fileListPtr); // on load is 0 which is the first index value
															//qDebug() <<"Current index " << *fileListPtr;
															//qDebug() << "current keyword " << *currentKeywordPtr;
	}



	// THREAD
	(*keywordListNumPtrCounter) += 1;



	/**********Search Engine Options ******/



	vectorSearchEngineOptions.clear();
	for (int i = 0; i < vectorSearchEngineOptions.size(); i++) {

		vectorSearchEngineOptions.removeAll(vectorSearchEngineOptions.value(i));
	}



	if (ui->checkBox_Google->isChecked()) {
		vectorSearchEngineOptions.resize(1);
		vectorSearchEngineOptions.push_back(options[0]->searchEngineOptions[0]);
	}



	if (ui->checkBox_Bing->isChecked()) {
		vectorSearchEngineOptions.resize(2);
		vectorSearchEngineOptions.push_back(options[0]->searchEngineOptions[1]);
	}



	if (ui->checkBox_Yahoo->isChecked()) {
		vectorSearchEngineOptions.resize(3);
		vectorSearchEngineOptions.push_back(options[0]->searchEngineOptions[2]);
	}





	// removes empty index
	for (int i = 0; i < vectorSearchEngineOptions.size(); i++) {
		vectorSearchEngineOptions.removeAll("");
	}


	// THREAD
	if (vectorSearchEngineOptions.size() == *searchEngineNumPtr) {
		*searchEngineNumPtr = 0; // done
	}


	if (vectorSearchEngineOptions.contains(vectorSearchEngineOptions.value(*searchEngineNumPtr))) {

		searchEngine = vectorSearchEngineOptions.value(*searchEngineNumPtr);

	}









	/********End Search Engine Options*****/






	if (searchEngine == "http://google.com") {





		/*******Email Options******/

		vectorEmailOptions.clear();
		for (int i = 0; i < vectorEmailOptions.size(); i++) {
			vectorEmailOptions.removeAll(vectorEmailOptions.value(i));
		}





		if (ui->checkBox_Email_Gmail->isChecked()) {

			vectorEmailOptions.resize(1);

			vectorEmailOptions.push_back(options[1]->emailOptions[0]);





		}



		if (ui->checkBox_Email_Yahoo->isChecked()) {

			vectorEmailOptions.resize(2);

			vectorEmailOptions.push_back(options[1]->emailOptions[1]);

		}



		if (ui->checkBox_Email_Hotmail->isChecked()) {

			vectorEmailOptions.resize(3);

			vectorEmailOptions.push_back(options[1]->emailOptions[2]);

		}





		// removes empty index
		for (int i = 0; i <vectorEmailOptions.size(); i++) {
			vectorEmailOptions.removeAll("");
		}





		if (vectorEmailOptions.size() == *emailOptionsNumPtr) {

			*emailOptionsNumPtr = 0; // done

		}


		if (vectorEmailOptions.contains(vectorEmailOptions.value(*emailOptionsNumPtr))) {

			email = vectorEmailOptions.value(*emailOptionsNumPtr);

		}



		/*****Email Section done****/







		// clears vector, if not we will keep resizing the vector on each timer

		vectorSocialNetworks2.clear();

		for (int i = 0; i <vectorSocialNetworks2.size(); i++) {

			vectorSocialNetworks2.removeAll(vectorSocialNetworks2.value(i));



		}




		/*******XXXX******/
		if (ui->checkBox_Social_Instagram->isChecked()) {

			vectorSocialNetworks2.resize(1);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[0]);





		}



		if (ui->checkBox_Social_Facebook->isChecked()) {

			vectorSocialNetworks2.resize(2);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[1]);

		}



		if (ui->checkBox_Social_Soundcloud->isChecked()) {

			vectorSocialNetworks2.resize(3);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[2]);

		}



		if (ui->checkBox_Social_Reverbnation->isChecked()) {

			vectorSocialNetworks2.resize(4);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[3]);

		}



		if (ui->checkBox_Social_Myspace->isChecked()) {

			vectorSocialNetworks2.resize(5);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[4]);

		}



		// removes empty index

		for (int i = 0; i <vectorSocialNetworks2.size(); i++) {

			vectorSocialNetworks2.removeAll("");



		}







		if (vectorSocialNetworks2.size() == *socialNetWorkNumPtr) {

			*socialNetWorkNumPtr = 0; // done



		}

		if (vectorSocialNetworks2.contains(vectorSocialNetworks2.value(*socialNetWorkNumPtr))) {

			socialNetWork = vectorSocialNetworks2.value(*socialNetWorkNumPtr);

		}







		(*keywordListSearchEngineCounterPtr) += 10;

		/*****Cast num to string to put inside query string******/

		castSearchQueryNumPtr = QString::number(*keywordListSearchEngineCounterPtr);

		/*********XX1*******/

		/**********
		if search box is not empty assign search box value to currentKeywordSearchBoxKeyword
		so incase keywordLoadListOptions is empty, we can allow it be appended to searchEngineParam
		on its own.

		--->>> If both keywordLoadListOptions, and searchbox keyword is NOT empty,
		were going to add the search box keyword to the keywordLoadListOptions Hash Table
		to be included for processing.

		************/
		if (!ui->lineEdit_keywords_search_box->text().isEmpty())
		{


			currentKeywordSearchBoxKeyword = options[3]->keywordSearchBoxOptions[0];
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordSearchBoxKeyword.replace("+", " "));

			searchEngineParam = "https://www.google.com/search?q=" + socialNetWork + "%20"
				+ email + "%20" + currentKeywordSearchBoxKeyword.replace(" ", "+") +
				"&ei=yv8oW8TYCOaN5wKImJ2YBQ&start=" + castSearchQueryNumPtr + "&sa=N&biw=1366&bih=613";

		}



		if (!fileList->empty())
		{

			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordPtr->replace("+", " "));
			searchEngineParam = "https://www.google.com/search?q=" + socialNetWork + "%20"
				+ email + "%20" + currentKeywordPtr->replace(" ", "+") +
				"&ei=yv8oW8TYCOaN5wKImJ2YBQ&start=" + castSearchQueryNumPtr + "&sa=N&biw=1366&bih=613";


		}



		// if both are not empty used currentKeywordPtr because the searchbox keyword is in the
		// qlist/hash and will be eventuall processed
		if (!fileList->empty() && !ui->lineEdit_keywords_search_box->text().isEmpty())
		{
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordPtr->replace("+", " "));


		}




		/****Continues email quene***/
		if (*keywordListSearchEngineCounterPtr == 100) {

			if (*emailOptionsNumPtr <= vectorEmailOptions.size()) {

				(*emailOptionsNumPtr) += 1;
			}
		}







		/****Continues social network quene***/

		if (*keywordListSearchEngineCounterPtr == 100) {
			if (*socialNetWorkNumPtr <= vectorSocialNetworks2.size()) {
				(*socialNetWorkNumPtr) += 1;
			}
		}









		/*******Stops social network quene, and moves on to next******/

		if (*keywordListSearchEngineCounterPtr == 100) {
			*keywordListSearchEngineCounterPtr = 0;

			// if social network pointer, and email options pointer is equal
			//than the size of  socialNetworkOptions arrary,
			//then were done, and move on
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size())
			{

				// if the last item in vector is true, and dosent match our current value
				// theres more elements after our current element, we need
				// this to make sure out pointer dosent get out of a range/QVector out of range.

				if (!vectorSearchEngineOptions.last().isEmpty())
				{
					vectorSearchEngineOptionsLastItem = vectorSearchEngineOptions.last();
					if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.value(*searchEngineNumPtr)) {
						(*searchEngineNumPtr) += 1;
					}
				}
			}


			if (*emailOptionsNumPtr > vectorEmailOptions.size()) {
				*emailOptionsNumPtr = 0;
			}

		}





		//qDebug() << searchEngineParam;

		// qDebug()<<  searchEngine;

		// qDebug()<<  vectorSearchEngineOptions;

		// qDebug() << *keywordListSearchEngineCounterPtr;

	}











	/********Bing********/

	if (searchEngine == "http://bing.com") {







		/*******Email Options******/

		vectorEmailOptions.clear();

		for (int i = 0; i < vectorEmailOptions.size(); i++) {

			vectorEmailOptions.removeAll(vectorEmailOptions.value(i));



		}





		if (ui->checkBox_Email_Gmail->isChecked()) {

			vectorEmailOptions.resize(1);

			vectorEmailOptions.push_back(options[1]->emailOptions[0]);





		}



		if (ui->checkBox_Email_Yahoo->isChecked()) {

			vectorEmailOptions.resize(2);

			vectorEmailOptions.push_back(options[1]->emailOptions[1]);

		}



		if (ui->checkBox_Email_Hotmail->isChecked()) {

			vectorEmailOptions.resize(3);

			vectorEmailOptions.push_back(options[1]->emailOptions[2]);

		}





		// removes empty index

		for (int i = 0; i <vectorEmailOptions.size(); i++) {

			vectorEmailOptions.removeAll("");



		}





		if (vectorEmailOptions.size() == *emailOptionsNumPtr) {

			*emailOptionsNumPtr = 0; // done



		}

		if (vectorEmailOptions.contains(vectorEmailOptions.value(*emailOptionsNumPtr))) {

			email = vectorEmailOptions.value(*emailOptionsNumPtr);

		}



		/*****Email Section done****/







		// clears vector, if not we will keep resizing the vector on each timer

		vectorSocialNetworks2.clear();

		for (int i = 0; i <vectorSocialNetworks2.size(); i++) {

			vectorSocialNetworks2.removeAll(vectorSocialNetworks2.value(i));



		}





		if (ui->checkBox_Social_Instagram->isChecked()) {

			vectorSocialNetworks2.resize(1);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[0]);





		}



		if (ui->checkBox_Social_Facebook->isChecked()) {

			vectorSocialNetworks2.resize(2);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[1]);

		}



		if (ui->checkBox_Social_Soundcloud->isChecked()) {

			vectorSocialNetworks2.resize(3);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[2]);

		}



		if (ui->checkBox_Social_Reverbnation->isChecked()) {

			vectorSocialNetworks2.resize(4);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[3]);

		}



		if (ui->checkBox_Social_Myspace->isChecked()) {

			vectorSocialNetworks2.resize(5);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[4]);

		}



		// removes empty index

		for (int i = 0; i <vectorSocialNetworks2.size(); i++) {

			vectorSocialNetworks2.removeAll("");



		}







		if (vectorSocialNetworks2.size() == *socialNetWorkNumPtr) {
			*socialNetWorkNumPtr = 0; // done
		}

		if (vectorSocialNetworks2.contains(vectorSocialNetworks2.value(*socialNetWorkNumPtr))) {

			socialNetWork = vectorSocialNetworks2.value(*socialNetWorkNumPtr);

		}







		(*keywordListSearchEngineCounterPtr) += 10;

		/*****Cast num to string to put inside query string******/

		castSearchQueryNumPtr = QString::number(*keywordListSearchEngineCounterPtr);






		if (!ui->lineEdit_keywords_search_box->text().isEmpty())
		{
			currentKeywordSearchBoxKeyword = options[3]->keywordSearchBoxOptions[0];
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordSearchBoxKeyword.replace("+", " "));

			searchEngineParam = "https://www.bing.com/search?q=" +
				socialNetWork + "%20"
				+ email + "%20" + currentKeywordSearchBoxKeyword.replace(" ", "+") +
				"&qs=n&sp=-1&pq=undefined&sc=0-45&sk=&cvid=6C577B0F2A1348BBB5AF38F9AC4CA13A&first="
				+ castSearchQueryNumPtr + "&FORM=PERE";

		}



		if (!fileList->empty())
		{
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordPtr->replace("+", " "));
			searchEngineParam = "https://www.bing.com/search?q=" +
				socialNetWork + "%20"
				+ email + "%20" + currentKeywordPtr->replace(" ", "+") +
				"&qs=n&sp=-1&pq=undefined&sc=0-45&sk=&cvid=6C577B0F2A1348BBB5AF38F9AC4CA13A&first="
				+ castSearchQueryNumPtr + "&FORM=PERE";
		}





		// if both are not empty used currentKeywordPtr because the searchbox keyword is in the
		// qlist/hash and will be eventuall processed
		if (!fileList->empty() && !ui->lineEdit_keywords_search_box->text().isEmpty())
		{
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordPtr->replace(" ", "+"));


		}



		/****Continues email quene***/

		if (*keywordListSearchEngineCounterPtr == 100) {
			if (*emailOptionsNumPtr <= vectorEmailOptions.size()) {
				(*emailOptionsNumPtr) += 1;
			}

		}



		/****Continues social network quene***/

		if (*keywordListSearchEngineCounterPtr == 100) {
			if (*socialNetWorkNumPtr <= vectorSocialNetworks2.size()) {
				(*socialNetWorkNumPtr) += 1;

			}
		}





		/*******Stops social network quene, and moves on to next******/

		if (*keywordListSearchEngineCounterPtr == 100) {
			*keywordListSearchEngineCounterPtr = 0;

			// if social network pointer, and email options pointer is
			//than the size of  socialNetworkOptions arrary,
			//then were done, and move on

			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				//(*socialNetWorkNumPtr)+=0;
				//(*socialNetWorkNumPtr)+=1; //
				//qDebug() << "MOVE ON";
				// add one to it if theres a element before it
				//vectorSocialNetwork   get current el, check to see if theres a el after it, then
				// increment it
				// (*searchEngineNumPtr)+=1;
				// vectorSearchEngineOptions.value(*searchEngineNumPtr)

				// if the last item in vector is true, and dosent match our current value

				// theres more elements after our current element, we need

				// this to make sure out pointer dosent get out of a range/QVector out of range.



				if (!vectorSearchEngineOptions.last().isEmpty()) {
					vectorSearchEngineOptionsLastItem = vectorSearchEngineOptions.last();
					if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.value(*searchEngineNumPtr)) {

						(*searchEngineNumPtr) += 1;

					}
				}






				/****Continues search engine quene***/

				if (*keywordListSearchEngineCounterPtr == 100) {
					// cant use above condition because were resetting
					//*keywordListSearchEngineCounterPtrto 0
					// (*searchEngineNumPtr )+=1; // this should only be used when were done scraping

				}



				/*****

				// MOVE ON TO NEXT

				//

				if next checkbox option is not checked skip it/add two two pointer

				by incrementing pointer by 2 we skip the checkbox option thats empty,

				and go to the next one, else dont skip it, and go to it --> increment by 1

				*****/



			}



			if (*emailOptionsNumPtr > vectorEmailOptions.size()) {
				*emailOptionsNumPtr = 0;
			}
		}


		//qDebug() << searchEngineParam;
		// qDebug()<<  searchEngine;
		// qDebug()<<  vectorSearchEngineOptions;
		// qDebug() << *keywordListSearchEngineCounterPtr;



	}



	/********Yahoo********/

	if (searchEngine == "http://yahoo.com") {

		/*******Email Options******/

		vectorEmailOptions.clear();

		for (int i = 0; i < vectorEmailOptions.size(); i++) {
			vectorEmailOptions.removeAll(vectorEmailOptions.value(i));
		}



		if (ui->checkBox_Email_Gmail->isChecked()) {
			vectorEmailOptions.resize(1);
			vectorEmailOptions.push_back(options[1]->emailOptions[0]);
		}



		if (ui->checkBox_Email_Yahoo->isChecked()) {
			vectorEmailOptions.resize(2);
			vectorEmailOptions.push_back(options[1]->emailOptions[1]);
		}



		if (ui->checkBox_Email_Hotmail->isChecked()) {
			vectorEmailOptions.resize(3);
			vectorEmailOptions.push_back(options[1]->emailOptions[2]);
		}





		// removes empty index

		for (int i = 0; i <vectorEmailOptions.size(); i++) {
			vectorEmailOptions.removeAll("");
		}



		if (vectorEmailOptions.size() == *emailOptionsNumPtr) {
			*emailOptionsNumPtr = 0; // done
		}

		if (vectorEmailOptions.contains(vectorEmailOptions.value(*emailOptionsNumPtr))) {
			email = vectorEmailOptions.value(*emailOptionsNumPtr);
		}



		/*****Email Section done****/


		// clears vector, if not we will keep resizing the vector on each timer

		vectorSocialNetworks2.clear();
		for (int i = 0; i <vectorSocialNetworks2.size(); i++) {
			vectorSocialNetworks2.removeAll(vectorSocialNetworks2.value(i));

		}





		if (ui->checkBox_Social_Instagram->isChecked()) {

			vectorSocialNetworks2.resize(1);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[0]);





		}



		if (ui->checkBox_Social_Facebook->isChecked()) {

			vectorSocialNetworks2.resize(2);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[1]);

		}



		if (ui->checkBox_Social_Soundcloud->isChecked()) {

			vectorSocialNetworks2.resize(3);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[2]);

		}



		if (ui->checkBox_Social_Reverbnation->isChecked()) {

			vectorSocialNetworks2.resize(4);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[3]);

		}



		if (ui->checkBox_Social_Myspace->isChecked()) {

			vectorSocialNetworks2.resize(5);

			vectorSocialNetworks2.push_back(options[2]->socialNetworkOptions[4]);

		}



		// removes empty index

		for (int i = 0; i <vectorSocialNetworks2.size(); i++) {

			vectorSocialNetworks2.removeAll("");



		}







		if (vectorSocialNetworks2.size() == *socialNetWorkNumPtr) {

			*socialNetWorkNumPtr = 0; // done



		}

		if (vectorSocialNetworks2.contains(vectorSocialNetworks2.value(*socialNetWorkNumPtr))) {

			socialNetWork = vectorSocialNetworks2.value(*socialNetWorkNumPtr);

		}







		(*keywordListSearchEngineCounterPtr) += 10;

		/*****Cast num to string to put inside query string******/

		castSearchQueryNumPtr = QString::number(*keywordListSearchEngineCounterPtr);

		if (!ui->lineEdit_keywords_search_box->text().isEmpty())
		{


			currentKeywordSearchBoxKeyword = options[3]->keywordSearchBoxOptions[0];
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordSearchBoxKeyword.replace("+", " "));


			searchEngineParam = "https://search.yahoo.com/search;_ylt=A2KIbNDlJS1b7nIAYNNx.9w4;_ylu=X3oDMTFjN3E2bWhuBGNvbG8DYmYxBHBvcwMxBHZ0aWQDVUkyRkJUMl8xBHNlYwNwYWdpbmF0aW9u?p=" +
				socialNetWork + "%20"
				+ email + "%20" + currentKeywordSearchBoxKeyword.replace(" ", "+") +
				"&ei=UTF-8&fr=yfp-hrmob&fr2=p%3Afp%2Cm%3Asb&_tsrc=yfp-hrmob&fp=1&b=11&pz=" + castSearchQueryNumPtr + "&xargs=0";

		}



		if (!fileList->empty())
		{

			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordPtr->replace("+", " "));

			searchEngineParam = "https://search.yahoo.com/search;_ylt=A2KIbNDlJS1b7nIAYNNx.9w4;_ylu=X3oDMTFjN3E2bWhuBGNvbG8DYmYxBHBvcwMxBHZ0aWQDVUkyRkJUMl8xBHNlYwNwYWdpbmF0aW9u?p=" +
				socialNetWork + "%20"
				+ email + "%20" + currentKeywordPtr->replace(" ", "+") +
				"&ei=UTF-8&fr=yfp-hrmob&fr2=p%3Afp%2Cm%3Asb&_tsrc=yfp-hrmob&fp=1&b=11&pz=" + castSearchQueryNumPtr + "&xargs=0";

		}

		// if both are not empty used currentKeywordPtr because the searchbox keyword is in the
		// qlist/hash and will be eventuall processed
		if (!fileList->empty() && !ui->lineEdit_keywords_search_box->text().isEmpty())
		{
			ui->label_Current_Keyword->setText("Current Keyword: " + currentKeywordPtr->replace("+", " "));


		}






		/****Continues email quene***/

		if (*keywordListSearchEngineCounterPtr == 100) {

			if (*emailOptionsNumPtr <= vectorEmailOptions.size()) {

				(*emailOptionsNumPtr) += 1;



			}

		}







		/****Continues social network quene***/

		if (*keywordListSearchEngineCounterPtr == 100) {

			if (*socialNetWorkNumPtr <= vectorSocialNetworks2.size()) {

				(*socialNetWorkNumPtr) += 1;



			}

		}









		/*******Stops social network quene, and moves on to next******/

		if (*keywordListSearchEngineCounterPtr == 100) {

			*keywordListSearchEngineCounterPtr = 0;



			// if social network pointer, and email options pointer is

			//than the size of  socialNetworkOptions arrary,

			//then were done, and move on

			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				//(*socialNetWorkNumPtr)+=0;

				//(*socialNetWorkNumPtr)+=1; //

				//qDebug() << "MOVE ON";

				// add one to it if theres a element before it

				//vectorSocialNetwork   get current el, check to see if theres a el after it, then

				// increment it

				// (*searchEngineNumPtr)+=1;

				// vectorSearchEngineOptions.value(*searchEngineNumPtr)



				// if the last item in vector is true, and dosent match our current value

				// theres more elements after our current element, we need

				// this to make sure out pointer dosent get out of a range/QVector out of range.



				if (!vectorSearchEngineOptions.last().isEmpty()) {

					vectorSearchEngineOptionsLastItem = vectorSearchEngineOptions.last();

					if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.value(*searchEngineNumPtr)) {

						(*searchEngineNumPtr) += 1;

					}

				}



				if (vectorSearchEngineOptions.size() == *searchEngineNumPtr) {

					// *searchEngineNumPtr+=1; // done



				}

				/****Continues search engine quene***/

				// (*searchEngineNumPtr )+=1; // this should only be used when were done scraping





				/****Continues search engine quene***/

				if (*keywordListSearchEngineCounterPtr == 100) {

					// cant use above condition because were resetting

					//*keywordListSearchEngineCounterPtr to 0

					// (*searchEngineNumPtr )+=1; // this should only be used when were done scraping

				}





			}



			if (*emailOptionsNumPtr > vectorEmailOptions.size()) {

				*emailOptionsNumPtr = 0;

			}

		}

		//qDebug() << searchEngineParam;
		// qDebug()<<  searchEngine;
		// qDebug()<<  vectorSearchEngineOptions;
		// qDebug() << *keywordListSearchEngineCounterPtr;
	}

	/********End of All Scraping Code******/


	// if proxy server is empty emit a signal to let worker thread know
	// if there are no proxies, use users regular ip
	if (proxyServers->isEmpty())
	{
		emit emitsenderEmptyProxyServer("Empty");
		// qDebug() << "Main thread Empty??"<< *proxyServers;

	}

	if (!proxyServers->isEmpty())
	{
		emit emitsenderEmptyProxyServer("Not Empty");
		// qDebug() << "Main thread not empty??"<< *proxyServers;
	}

	// sending params/options signal after we done
	//emit postParam(searchEngineParam, options[5]->userAgentsOptions[0], proxyServers);


	// qDebug() << "SIZE "<< vectorSearchEngineOptions.size();
	// qDebug() << "DATA "<< vectorSearchEngineOptions;
	vectorSearchOptions.insert(0, vectorSearchEngineOptions);
	vectorSearchOptions.insert(1, vectorEmailOptions);
	vectorSearchOptions.insert(2, vectorSocialNetworks2);

	timerOptions.insert(0, ui->spinBox_Harvester_Timer->value());
	timerOptions.insert(1, ui->spinBox_Proxy_Rotate_Interval->value());



	otherOptions.insert(0, ui->comboBox_search_results_amount->currentText());
	// if url multi is true, and url multi amount is true
	if (ui->radioButton_Parse_Multi_URL->isChecked() && !QString::number(ui->spinBox_Multi_URL_Amount->value()).isEmpty()) {
		otherOptions.insert(1, "MULTI_URL_SELECTED");
		otherOptions.insert(2, QString::number(ui->spinBox_Multi_URL_Amount->value()));

	}
	if (ui->radioButton_Parse_1_URL->isChecked()) {
		otherOptions.insert(1, "1_URL_SELECTED");
	}
	//otherOptions.insert(2,)


	//QThreadPool pool;
	//pool.setExpiryTimeout(-1);
	//QString keywordLabel = ui->lineEdit_keywords_search_box->text();
	QFuture<void> multithread1  = QtConcurrent::run(&this->threadWorker
		, &Worker::curlParams, vectorSearchOptions, ui->lineEdit_keywords_search_box->text(), proxyServers,
		timerOptions, otherOptions);


}

void BeatCrawler::populateEmailTable() {


	setEmailList = emailList->toSet().toList();

	if (*nextEmailPaginationPtr == 0) {

		if (emailList->size() >= 8) {

			for (int row = 0; row < 8; ++row) {
				for (int column = 0; column < 1; ++column) {
					QStandardItem *item = new QStandardItem(setEmailList.value(row).toLower());
					emailTableModel->setItem(row, column, item);

					if (row >= emailList->size()) {
						delete item;
					}
				}
				//ui->tableView_Emails->resizeRowsToContents();
			}



			timer->stop();
		}


	}
}


void BeatCrawler::receiverLogHarvesterStatus(QString logStatus)

{
	QList<QString>data;	
	if (!logStatus.isEmpty())
	{
		QListWidgetItem *listItem = new QListWidgetItem(logStatus, ui->listWidget_Log_Harvester_Status);
		//listItem->setText(logStatus);
		//ui->listWidget_Log_Harvester_Status->insertItem(*logStatusCounterPtr, listItem);
		if (*logStatusCounterPtr <= 5)
		{
		}

		if (*logStatusCounterPtr == 16)
		{
			*logStatusCounterPtr = 0;
			//delete listItem;
			
			ui->listWidget_Log_Harvester_Status->clear();	
			
		}
		(*logStatusCounterPtr) += 1;
	}	
}
void BeatCrawler::receiverEmailList2(QString list)


{

	//qDebug() <<"List2" << list;
}

void BeatCrawler::receiverEmailList(QString list)
{

	QString emailsToLowerCase;
	*emailList << list;

	// items found on bottom status bar
	ui->label_Items_Found->setText("Items Found: " + QString::number(emailList->size()));
}

void BeatCrawler::on_pushButton_Next_Email_Pagination_clicked()
{
	/**********

	(*nextEmailPaginationPtr) += 20;

	if (*nextEmailPaginationPtr >= 40) {

	(*previousEmailPaginationPtr) += 20;

	}

	******/



	if (!emailList->isEmpty()) {
		emailTableModel->clear();

		(*nextEmailPaginationPtr) += 8;
		if (*nextEmailPaginationPtr >= 16)
		{
			(*previousEmailPaginationPtr) += 8;
		}

		for (int row = (*previousEmailPaginationPtr); row < (*nextEmailPaginationPtr); ++row)
		{

			for (int column = 0; column < 1; ++column) {


				if (row <= emailList->size()) {
				}
				else {
					//ui->pushButton_Previous_Email_Pagination->setEnabled(true);

				}

				/*******as long as the email list size is greater than the previousPages*****/

				//|| (*nextEmailPaginationPtr) >= emailList->size()
				if (row >= emailList->size()) {
					ui->pushButton_Next_Email_Pagination->setEnabled(false);

				}
				else {

					QStandardItem *item = new QStandardItem(emailList->value(row));
					emailTableModel->setItem(row, column, item);

					if (row >= emailList->size())
					{
						delete item;
					}
				}

			}
		}


		for (int i = 0; i<emailTableModel->rowCount(); i++)
		{
			if (emailTableModel->item(i) == NULL) {
				// should we delete an null item like--> delete emailTableModel->item(i);
				ui->tableView_Emails->hideRow(i);
				///delete emailTableModel->item(i);

			}
		}

	}


	ui->tableView_Emails->resizeColumnsToContents();



	//qDebug() << "NEXT " << *nextEmailPaginationPtr;
	//qDebug() << "PREVIOUS" << *previousEmailPaginationPtr;

}

void BeatCrawler::on_pushButton_Previous_Email_Pagination_clicked()
{
	if (!emailList->isEmpty()) {
		emailTableModel->clear();
		if (*previousEmailPaginationPtr >= 8) {
			(*previousEmailPaginationPtr) -= 8;
			(*nextEmailPaginationPtr) -= 8;
		}

		for (int row = (*previousEmailPaginationPtr); row < (*nextEmailPaginationPtr); ++row)
		{

			for (int column = 0; column < 1; ++column) {
				qDebug() << "ROW--" << row;
				qDebug() << "PREV--" << (*previousEmailPaginationPtr);
				qDebug() << "NEXT" << (*nextEmailPaginationPtr);
				qDebug() << "EMAIL SIZET" << emailList->size();

				if (row >= emailList->size()) {

				}
				else {
					ui->pushButton_Next_Email_Pagination->setEnabled(true);
				}

				/*******as long as the email list size is greater than the previousPages*****/
				if (row >= 0) {
					//ui->pushButton_Previous_Email_Pagination->setEnabled(false);
					QStandardItem *item = new QStandardItem(emailList->value(row));
					emailTableModel->setItem(row, column, item);

					if (row >= emailList->size())
					{
						delete item;
					}
				}


			}
		}


		for (int i = 0; i<emailTableModel->rowCount(); i++)
		{
			if (emailTableModel->item(i) == NULL) {
				// should we delete an null item like--> delete emailTableModel->item(i);
				ui->tableView_Emails->hideRow(i);
				//delete emailTableModel->item(i);
			}
		}





	}

}


void BeatCrawler::recieverProxyTableSelection(const QItemSelection &selected, const QItemSelection &deselected)

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
		foreach(const QModelIndex &index, indexes) {

			// removes row user has selected
			ui->tableWidget_Proxy->removeRow(index.row());
			// removes proxy servers from qstring list, if user has added proxies
			proxyServers->removeAt(index.row());

		}
	}

}



void BeatCrawler::keywordsQueueTable() {
	for (;;) {

		for (int row = 0; row < fileList->size(); row++)
		{
			for (int col = 0; col < 2; col++)
			{

				// if current keyword matches this item, change set item string to "Currently Processing"
				// if keyword is 1/done change it to "Complete"
				// if keyword is 0 /not dont change it to "Waiting.."


				QEventLoop loop;
				QTimer::singleShot(3000, &loop, SLOT(quit()));
				loop.exec();
				if (col == 0)
				{

					keywordQueueItem = new QTableWidgetItem();
					keywordQueueItem->setFlags(keywordQueueItem->flags() ^ Qt::ItemIsEditable);
					keywordQueueItem->setText(fileList->value(row));

					ui->tableWidget_Keywords_Queue->setItem(row, col, keywordQueueItem);
				}

				if (col == 1)
				{
					// qDebug() << *currentKeywordPtr;
					// if current keyword matches a keyword in our row change it to "Processing"
					// else change it to "Waiting"
					if (*currentKeyword_ == fileList->value(row))
					{

						//ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem("Processing..."));
						// ui->tableWidget_Keywords_Queue->item(row,col)->setBackground(QBrush(QColor(250,0,0)));

						if (clickedStartStopButton == false) {

							keywordQueueItem = new QTableWidgetItem();
							keywordQueueItem->setFlags(keywordQueueItem->flags() ^ Qt::ItemIsEditable);
							//keywordQueueItem->setText("Aborted");

							//ui->tableWidget_Keywords_Queue->setItem(row, col, keywordQueueItem);

						}
						else if (clickedStartStopButton == true) {


							keywordQueueItem = new QTableWidgetItem();
							keywordQueueItem->setFlags(keywordQueueItem->flags() ^ Qt::ItemIsEditable);
							//keywordQueueItem->setText("Waiting...");

							//ui->tableWidget_Keywords_Queue->setItem(row, col, keywordQueueItem);

						}
					}


					// if keyword is done
					if (*currentKeyword_ != fileList->value(row))
					{

						//keywordQueueItem = new QTableWidgetItem();
						//keywordQueueItem->setFlags(keywordQueueItem->flags() ^ Qt::ItemIsEditable);
						//->setText("Processing");

						//Possible erros because of minus of row which might be 0-1??
						//ui->tableWidget_Keywords_Queue->setItem((row - 1), col, keywordQueueItem);
					}
					else {

						//keywordQueueItem = new QTableWidgetItem();
						//keywordQueueItem->setFlags(keywordQueueItem->flags() ^ Qt::ItemIsEditable);
						//keywordQueueItem->setText("Waiting...");

						//ui->tableWidget_Keywords_Queue->setItem(row, col, keywordQueueItem);
					}

					if (*currentKeyword_ == fileList->at(row)) {
						qDebug() << "matches";
						qDebug() << "MATCH & ROW-->" << row;
						int completedKeywordIndex = row - 1;
						//if (fileList->value(completedKeywordIndex) != *currentKeyword_) {
						// completed
						for (int j = 0; j <= completedKeywordIndex; j++) {
							keywordQueueItem = new QTableWidgetItem();
							keywordQueueItem->setFlags(keywordQueueItem->flags() ^ Qt::ItemIsEditable);
							keywordQueueItem->setText("Completed");
							qDebug() << "PAST ROWS-->" << j;

							ui->tableWidget_Keywords_Queue->setItem(j, col, keywordQueueItem);

						}

						//}
					}

				}


			}

			// if i is equal to or greater than the keywordKey size, delete pointer because were done with it
			if (row >= fileList->size())
			{
				qDebug() << "LARGER than SIZE";
				delete keywordQueueItem;
			}
		}// end of for inner loop


	} // end of for outer loop


}

void BeatCrawler::deleteKeyordsListTable() {
	// create empty table
	//*currentKeywordPtr = "";
	//if (thread1->isRunning())
	//{
	//connect(thread1, SIGNAL(started()), thread1, SLOT(terminate()));
	//if (disconnect(fileReader, &FileReader::emitSenderFileReadKeywordList, 0, 0) == true)
	//{
	//ui->label_Keywords_Count->setText(" ");
	//connect(fileReader, SIGNAL(emitSenderFileReadKeywordList(QString, QString, int)), this, SLOT(receiverFileReadKeywordList(QString, QString, int)));
	//}
	//	else {
	//	qDebug() << "ERROR DISCONNECTING -- emitSenderFileReadKeywordList Signal from fileReader Obbject";
	//}
	//receiverFileReadKeywordList
	//disconnect(fileReader, 0, receiverFileReadKeywordList, 0);

	//}
	if (!fileList->isEmpty())
	{
		for (int row = 0; row < fileList->size(); row++)
		{
			for (int col = 0; col < 2; col++)
			{
				ui->tableWidget_Keywords_Queue->setItem(row, col, new QTableWidgetItem(""));
			}
		}

		//clear QStringList
		fileList->clear();
		emit emitRemoveThreadFileList();
	}
	ui->lineEdit_Keyword_List_File_Location->setText(" ");

}

void BeatCrawler::deleteEmailsListTable()
{

	// clear email qtlist
	emailList->clear();
	// clear email qt set
	setEmailList.clear();
	// clear email table model
	emailTableModel->clear();
	emit emitRemoveThreadEmailList();

}

void BeatCrawler::receiverEnableDeleteEmailCheckBox()
{
	ui->pushButton_Load_Keyword_List->setEnabled(true);
	ui->checkBox_Delete_Emails->setEnabled(true);
	ui->pushButton_Start->setEnabled(true);

	// initialize email table -- checks to see if emails are in tabl


	ui->pushButton_Next_Email_Pagination->setEnabled(false);
	ui->pushButton_Previous_Email_Pagination->setEnabled(false);

}


// received signal from thread to enable widgets after filelist/keywords have been successfuly deleted
void BeatCrawler::receiverEnableDeleteKeywordCheckBox()
{
	ui->checkBox_Delete_Keywords->setEnabled(true);
	ui->pushButton_Load_Keyword_List->setEnabled(true);
	ui->pushButton_Start->setEnabled(true);
}



void BeatCrawler::recieverCurlResponseInfo(QString info)
{
	//qDebug() << info;

	if (info == "Proxy Error" || info == "503")
	{

		ui->label_Curl_Status->setText("<font color='black'> Status: Proxy failed, or Server is Temporarily Unavailable</font>");
		//QTimer::singleShot(10,this,SLOT(deleteEmailsListTable()));
		emailList->clear();
		setEmailList.clear();
		emailTableModel->clear();
		ui->label_Items_Found->setText("Items Found: ");
		ui->tableView_Emails->resizeRowsToContents();
		emit emitRemoveThreadEmailList();

	}
	else if (info == "Request Succeded")
	{
			ui->label_Curl_Status->setText("<b> <font color='green'> Status: Successfully Crawling</font></b>");

	}
}



void BeatCrawler::on_pushButton_Add_Proxy_clicked()
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



void BeatCrawler::on_checkBox_Delete_Keywords_clicked()
{

	if (!fileList->isEmpty()) {
		if (ui->checkBox_Delete_Keywords->isChecked())
		{
			// as were deleting keywords disable upload keyword list button, and delete keywords checkbox
			ui->checkBox_Delete_Keywords->setEnabled(false);
			ui->pushButton_Load_Keyword_List->setEnabled(false);


			ui->pushButton_Start->setEnabled(false);
			QTimer::singleShot(10, this, SLOT(deleteKeyordsListTable()));
			ui->lineEdit_Keyword_List_File_Location->setText("");
		}
	}
}


void BeatCrawler::on_checkBox_Delete_Emails_clicked()
{
	if (ui->checkBox_Delete_Emails->isChecked())
	{
		if (!emailList->isEmpty() && !setEmailList.isEmpty()) {

			ui->checkBox_Delete_Emails->setEnabled(false);
			ui->pushButton_Start->setEnabled(false);
			ui->pushButton_Load_Keyword_List->setEnabled(false);


			ui->pushButton_Next_Email_Pagination->setEnabled(false);
			ui->pushButton_Previous_Email_Pagination->setEnabled(false);
			nextEmailPaginationPtr = &nextEmailPagination;
			previousEmailPaginationPtr = &previousEmailPagination;


			QTimer::singleShot(10, this, SLOT(deleteEmailsListTable()));
			ui->label_Items_Found->setText("Items Found: ");

		}

	}
}



void BeatCrawler::on_pushButton_Save_Emails_clicked()
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

		for (int i = 0; i < emailList->size(); i++)
		{
			savedEmails = emailList->value(i);
			savedEmailsList << savedEmails.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
			qDebug() << savedEmailsList.value(i);
			outStream << savedEmailsList.value(i) << "\n";

		}

		file.close();
	}
}




