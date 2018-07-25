
/*
Copyright 2018 Titus Shoats

*/

#include "worker.h"


//QStringList *Worker::parsedEmailList;


std::string buffer;
QStringList parsedEmailList1;
#define MAX_WAIT_MSECS 30*1000 /* Wait max. 30 seconds */
#define CNT 4

Worker::Worker(QObject *parent) :
	QObject(parent)
{

	proxies = new QString();
	workerCounterNum = 0;
	workerCounterPtr = reinterpret_cast<int *>(&workerCounterNum);

	proxyServerCounterNum = 0;
	proxyServerCounterPtr = reinterpret_cast<int *>(&proxyServerCounterNum);

	harvesterTimerNum = 0;
	harvesterTimerPtr = reinterpret_cast<int *>(&harvesterTimerNum);
	httpRequestList = new QList<QStringList>();

	proxyRotateIntervalNum = 0;
	proxyRotateIntervalPtr = reinterpret_cast<int *>(&proxyRotateIntervalNum);

	curlHTTPRequestCounterNum = 0;
	curlHTTPRequestCounterPtr = reinterpret_cast<int *>(&curlHTTPRequestCounterNum);

	curlMultiProcessCounterNum = 0;
	curlMultiProcessPtrCounter = reinterpret_cast<int *>(&curlMultiProcessCounterNum);



	wStop = false;
	multiURLOptionString = "";
	multiURLOption = reinterpret_cast<QString *>(&multiURLOptionString);

	multiOptionOneURLString = "";
	multiOptionOneURL = reinterpret_cast<QString *>(&multiOptionOneURLString);



	proxyServers = new QList <QString>();
	fileList = new QStringList();

	fileListNum = 0;
	fileListPtr = &fileListNum;
	currentKeyword = "";
	currentKeywordPtr = &currentKeyword;
	curlSingleProcessCounterNum = 0;
	curlSingleProcessPtrCounter = &curlSingleProcessCounterNum;
	searchEngineNum = 0;
	searchEngineNumPtr = &searchEngineNum;
	emailOptionsNum = 0;
	emailOptionsNumPtr = &emailOptionsNum;
	socialNetWorkNum = 0;
	socialNetWorkNumPtr = &socialNetWorkNum;
	searchEnginePaginationCounterNum = 0;
	searchEnginePaginationCounterPtr = &searchEnginePaginationCounterNum;
	errbuf[0] = 0;
	deleteKeywordCheckBoxTimer = new QTimer();
	//connect(deleteKeywordCheckBoxTimer, SIGNAL(timeout), this, SLOT(deleteKeywordCheckBox()));


}

Worker::~Worker()
{
	delete proxies;
	delete httpRequestList;
	delete fileList;
	delete proxyServers;
	delete deleteKeywordCheckBoxTimer;



}

void Worker::requestWork()
{
	emit workRequested();
}

void Worker::abort() {}

void Worker::stop()
{
	wStop = true;
}


void Worker::receiverRemoveThreadEmailList()
{
	parsedEmailList1.clear();
	for (;;) {
		QEventLoop loop;
		QTimer::singleShot(5000, &loop, SLOT(quit()));
		loop.exec();
		if (parsedEmailList1.isEmpty()) {
			emit emitsenderEnableDeleteEmailCheckBox();
			break;
		}
	}
}


void Worker::receiverRemoveThreadFileList()
{
	fileList->clear();
	for (;;) {
		QEventLoop loop;
		QTimer::singleShot(5000, &loop, SLOT(quit()));
		loop.exec();
		if (fileList->isEmpty()) {
			emit emitsenderEnableDeleteKeywordCheckBox();
			break;
		}
	}
}

void Worker::receiverReadFile(QString fileName)
{
	fileList->clear();
	QFile file(fileName);
	QFileInfo fi(file.fileName());
	QString fileExt = fi.completeSuffix();;
	QString strings;
	QString str;

	if (!file.open(QFile::ReadOnly))
	{
		qDebug() << "ERROR OPENING FILE" << file.error();
	}


	QTextStream ts(&file);
	while (!ts.atEnd())
	{
		str = ts.readLine();
		*fileList << str;
		//qDebug() << *fileList;

	}
	file.close();
	// send signal to enable checkboxes  and keyword file load button
	//QThread::currentThread()->msleep(2000);
	emit emitfinishReadingKeywordFile();

}


void Worker::readEmailFile() {
	QFile file("emails.txt");
	QString strings;
	QString str;
	qDebug() << "Readig file";
	if (!file.open(QFile::ReadOnly))
	{
		qDebug() << "ERROR OPENING EMAIL FILE" << file.error();
	}


	QTextStream ts(&file);
	while (!ts.atEnd())
	{
		str = ts.readLine();
	}
	file.close();
}


void Worker::parsedEmails() {
	if (!parsedEmailList1.isEmpty())
	{
		for (int j = 0; j< parsedEmailList1.size(); j++)
		{
			if (!parsedEmailList1.value(j).isEmpty()) {
				//emit emitEmailList1(parsedEmailList1.value(j));

			}
		}
	}



}


void Worker::curlParams(QList<QVector <QString>>vectorSearchOptions,
	QString lineEdit_keywords_search_box, QList <QString> *proxyServers,
	QList<int>timerOptions, QList<QString>otherOptions)
	//QList<QString>otherOptions
	// searchResultsPages
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
	QVector<QString>vectorSearchEngineOptions = vectorSearchOptions.value(0);
	QVector<QString>vectorEmailOptions = vectorSearchOptions.value(1);
	QVector<QString>vectorSocialNetworks2 = vectorSearchOptions.value(2);
	int harvesterTimer = timerOptions.value(0);
	int proxyRotateInterval = timerOptions.value(1);
	QString searchResultsPages = otherOptions.value(0);
	int appTimer;
	QString multiOptionURLAmount;
	/*********
	Chrono is namespace, meaning a certain class will on be in its scope
	Within this namespace we have a class thats a template name seconds, that takes a long long type -- which is seconds
	in our case. So to create a instance of seconds class, we need to create a object, that takes a long. In our case s(harvesterTimer)
	The s object of type seconds, also has members as well
	*******/
	std::chrono::seconds s(harvesterTimer);
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(s);
	// cast long long to int
	appTimer = static_cast<int>(ms.count());
	wStop = false;

	// Checks if keyword box is empty, and if file list if empty, if so by default put keyword box value into filelist
	if (!lineEdit_keywords_search_box.isEmpty() && fileList->isEmpty() ||
		!lineEdit_keywords_search_box.isEmpty() && !fileList->isEmpty())
	{
		fileList->removeAt(0);
		fileList->insert(0, lineEdit_keywords_search_box);

	}

	// MULTI URL OPTION
	if (!otherOptions.value(1).isEmpty()) 
	{
		*multiURLOption = otherOptions.value(1);
	}
	if (!otherOptions.value(2).isEmpty()) {

		multiOptionURLAmount = otherOptions.value(2);
	}


	for (int i = 0; i < 999999; i++)
	{



		//parsedEmails();
		if (wStop) {
			///lineEdit_keywords_search_box.clear();

			curlSingleProcessPtrCounter = &curlSingleProcessCounterNum;
			emailOptionsNumPtr = &emailOptionsNum;
			searchEngineNumPtr = &searchEngineNum;
			searchEnginePaginationCounterPtr = &searchEnginePaginationCounterNum;
			proxyServerCounterPtr = &proxyServerCounterNum;
			workerCounterPtr = &workerCounterNum;

			*curlSingleProcessPtrCounter = 0;
			*emailOptionsNumPtr = 0;
			*searchEngineNumPtr = 0;
			*searchEnginePaginationCounterPtr = 0;
			*proxyServerCounterPtr = 0;
			*workerCounterPtr = 0;



			//fileList->clear();
			break;
		}


		/******************************************************

		PROXY CONFIGURATION

		// if workerCounter == *proxyRotateIntervalPtr, reset workerCounter ; if certain number of
		// http request have been made rotate proxy
		if (*workerCounterPtr <= proxyRotateInterval)
		{
		// only rotate each proxy if proxyCounterPtr is not greater than our proxyServer qlist
		if ((*proxyServerCounterPtr) <= proxyServers->size())
		{
		// if proxy counter is not greater than proxyServer qlist, proxyCounter can increment
		canProxyCounterIncrement = true;
		}

		// if proxy counter is equal to the size of proxyServer qlist, we cant increment
		if ((*proxyServerCounterPtr) == proxyServers->size())
		{
		canProxyCounterIncrement = false;
		// if proxyServerCounter is equal to the size of the proxyServer qlist, reset it to 0
		*proxyServerCounterPtr = 0;
		}


		// if proxies contained in qlist empty in main thread, if so clear the proxylist in this thread also
		if (isProxyEmpty == true && proxyServers->size() == 0)
		{
		//qDebug() << "Proxy Empty";
		//qDebug() << *proxyServers;
		//qDebug() <<proxyServers->size();
		proxyServers->clear();
		*proxies = "";
		}

		// if proxies contained in qlist are not empty, and we can keep incrementing,
		// our proxies are good to use/rotate
		if (isProxyEmpty == false && canProxyCounterIncrement == true)
		{
		*proxies = proxyServers->value(*proxyServerCounterPtr);
		//qDebug() << "Counter-->" << *proxies;

		}
		//qDebug() << "Counter-->" << *proxyServerCounterPtr;
		//qDebug() << "Proxies-->" << *proxies;

		}


		// if workerCounter is greater than *proxyRotateIntervalPtr/ amount of http request before proxy rotates
		if (*workerCounterPtr >= proxyRotateInterval)
		{
		// restart workerCounter
		*workerCounterPtr = 0;
		// increment proxyServerPtr to go through each proxyServer index every interval
		(*proxyServerCounterPtr) += 1;

		}

		// increment workerCounter if we have not hit our http request limit to rotate each proxy
		(*workerCounterPtr) += 1;


		if (!proxyServers->isEmpty())
		{

		//qDebug() << *proxies;
		}


		*******************************************************/

		if (!fileList->isEmpty())
		{
			// on load is 0 which is the first index value
			*currentKeywordPtr = fileList->value(*fileListPtr);
		}

		(*curlSingleProcessPtrCounter) += 1;

		/**********Search Engine Options ******/

		// if pointer is equal to size of vector were done
		if (*searchEngineNumPtr == vectorSearchEngineOptions.size())
		{
			*searchEngineNumPtr = 0; // done
		}

		// if vector search engine contains current search engine, assign it it to search eng var
		if (vectorSearchEngineOptions.contains(vectorSearchEngineOptions.value(*searchEngineNumPtr)))
		{
			searchEngine = vectorSearchEngineOptions.value(*searchEngineNumPtr);
		}

		/********End Search Engine Options*****/



		/*******Email Options******/

		if (searchEngine == "http://google.com")
		{

			/*****Email Options******/

			/******Social NetWork Options******/
			if (*emailOptionsNumPtr == vectorEmailOptions.size()) {

				*emailOptionsNumPtr = 0; // done
			}



			if (vectorEmailOptions.contains(vectorEmailOptions.value(*emailOptionsNumPtr)))
			{
				email = vectorEmailOptions.value(*emailOptionsNumPtr);
			}



			/******Social NetWork Options******/
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				*socialNetWorkNumPtr = 0; // done
			}


			if (vectorSocialNetworks2.contains(vectorSocialNetworks2.value(*socialNetWorkNumPtr))) {
				socialNetWork = vectorSocialNetworks2.value(*socialNetWorkNumPtr);
			}

			// search engines pagination number
			(*searchEnginePaginationCounterPtr) += 10;

			/*****Cast num to string to put inside query string******/
			castSearchQueryNumPtr = QString::number(*searchEnginePaginationCounterPtr);


			/**********
			if search box is not empty assign search box value to currentKeywordSearchBoxKeyword
			so incase keywordLoadListOptions is empty, we can allow it be appended to searchEngineParam
			on its own.

			--->>> If both keywordLoadListOptions, and searchbox keyword is NOT empty,
			were going to add the search box keyword to the keywordLoadListOptions List
			to be included for processing.

			************/




			if (!lineEdit_keywords_search_box.isEmpty() && fileList->isEmpty() || !lineEdit_keywords_search_box.isEmpty() && !fileList->isEmpty())
			{

				currentKeywordSearchBoxKeyword = fileList->value(0);
				searchEngineParam = "https://www.google.com/search?q=" + socialNetWork + "%20"
					+ email + "%20" + currentKeywordSearchBoxKeyword.replace(" ", "+") +
					"&ei=yv8oW8TYCOaN5wKImJ2YBQ&start=" + castSearchQueryNumPtr + "&sa=N&biw=1366&bih=613";
			}

			if (!fileList->empty())
			{

				searchEngineParam = "https://www.google.com/search?q=" + socialNetWork + "%20"
					+ email + "%20" + currentKeywordPtr->replace(" ", "+") +
					"&ei=yv8oW8TYCOaN5wKImJ2YBQ&start=" + castSearchQueryNumPtr + "&sa=N&biw=1366&bih=613";
			}



			/****Continues email quene until its the last item in array***/

			if (*searchEnginePaginationCounterPtr == 100) {
				if (*emailOptionsNumPtr == vectorEmailOptions.size() - 1) {

					*emailOptionsNumPtr = 0;

				}
				else {
					(*emailOptionsNumPtr) += 1;
				}
			}

			/****Continues social network quene until its the last item in array***/

			if (*searchEnginePaginationCounterPtr == 100) {
				if (*socialNetWorkNumPtr == vectorSocialNetworks2.size() - 1) {

					*socialNetWorkNumPtr = 0;

				}
				else {
					(*socialNetWorkNumPtr) += 1;
				}
			}





			/*******
			* If search engine pagination reaches 100
			* Stops social network, email, ans search engine quene, and moves on to next search engine
			******/

			if (*searchEnginePaginationCounterPtr == 100) {
				*searchEnginePaginationCounterPtr = 0;

				// if social network pointer, and email options pointer is equal
				//than the size of  socialNetworkOptions arrary,
				//then were done, and move on
				if (*socialNetWorkNumPtr == vectorSocialNetworks2.size() - 1)
				{

					/*******
					if the last item in vector is true, and dosent match our current value
					theres more elements after our current element, we need
					this to make sure out pointer dosent get out of a range/QVector out of range.
					*****/

					if (!vectorSearchEngineOptions.last().isEmpty())
					{
						vectorSearchEngineOptionsLastItem = vectorSearchEngineOptions.last();
						if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.value(*searchEngineNumPtr)) {
							(*searchEngineNumPtr) += 1;
						}
					}
				}// end of checking if search eng pagination has reached 100


				if (*emailOptionsNumPtr == vectorEmailOptions.size())
				{
					*emailOptionsNumPtr = 0;
				}



			}


			//            qDebug() << "Search Engine Vector -->" <<vectorSearchEngineOptions;
			//            qDebug() << "Search Engine Vector Size -->" <<vectorSearchEngineOptions.size();
			//            qDebug() << "Search Engine Vector Pointer -->" << *searchEngineNumPtr;


			//            qDebug() << "Email Vector  -->" <<vectorEmailOptions;
			//            qDebug() << "Email Vector Size -->" <<vectorEmailOptions.size();
			//            qDebug() << "Email Vector Pointer -->" << *emailOptionsNumPtr;


			//            qDebug() << "Social Vector -->" <<vectorSocialNetworks2;
			//            qDebug() << "Social Vector Size -->" <<vectorSocialNetworks2.size();
			//            qDebug() << "Social Vector Pointer -->" << *socialNetWorkNumPtr;

			//qDebug() << searchEngineParam;
			// qDebug()<<  searchEngine;
			// qDebug()<<  vectorSearchEngineOptions;
			// qDebug() << *searchEnginePaginationCounterPtr;



		}// end of checking if search engine is Google



		 /************Bing*************/

		if (searchEngine == "http://bing.com")
		{

			/*****Email Options******/

			/******Social NetWork Options******/
			if (*emailOptionsNumPtr == vectorEmailOptions.size()) {

				*emailOptionsNumPtr = 0; // done
			}



			if (vectorEmailOptions.contains(vectorEmailOptions.value(*emailOptionsNumPtr)))
			{
				email = vectorEmailOptions.value(*emailOptionsNumPtr);
			}



			/******Social NetWork Options******/
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				*socialNetWorkNumPtr = 0; // done
			}


			if (vectorSocialNetworks2.contains(vectorSocialNetworks2.value(*socialNetWorkNumPtr))) {
				socialNetWork = vectorSocialNetworks2.value(*socialNetWorkNumPtr);
			}

			// search engines pagination number
			(*searchEnginePaginationCounterPtr) += 10;

			/*****Cast num to string to put inside query string******/
			castSearchQueryNumPtr = QString::number(*searchEnginePaginationCounterPtr);


			/**********
			if search box is not empty assign search box value to currentKeywordSearchBoxKeyword
			so incase keywordLoadListOptions is empty, we can allow it be appended to searchEngineParam
			on its own.

			--->>> If both keywordLoadListOptions, and searchbox keyword is NOT empty,
			were going to add the search box keyword to the keywordLoadListOptions List
			to be included for processing.

			************/
			if (!lineEdit_keywords_search_box.isEmpty() && fileList->isEmpty() || !lineEdit_keywords_search_box.isEmpty() && !fileList->isEmpty())
			{

				currentKeywordSearchBoxKeyword = fileList->value(0);
				searchEngineParam = "https://www.bing.com/search?q=" +
					socialNetWork + "%20"
					+ email + "%20" + currentKeywordSearchBoxKeyword.replace(" ", "+") +
					"&qs=n&sp=-1&pq=undefined&sc=0-45&sk=&cvid=6C577B0F2A1348BBB5AF38F9AC4CA13A&first="
					+ castSearchQueryNumPtr + "&FORM=PERE";
			}

			if (!fileList->empty())
			{

				searchEngineParam = "https://www.bing.com/search?q=" +
					socialNetWork + "%20"
					+ email + "%20" + currentKeywordPtr->replace(" ", "+") +
					"&qs=n&sp=-1&pq=undefined&sc=0-45&sk=&cvid=6C577B0F2A1348BBB5AF38F9AC4CA13A&first="
					+ castSearchQueryNumPtr + "&FORM=PERE";
			}




			/****Continues email quene until its the last item in array***/

			if (*searchEnginePaginationCounterPtr == 100) {
				if (*emailOptionsNumPtr == vectorEmailOptions.size() - 1) {

					*emailOptionsNumPtr = 0;

				}
				else {
					(*emailOptionsNumPtr) += 1;
				}
			}

			/****Continues social network quene until its the last item in array***/

			if (*searchEnginePaginationCounterPtr == 100) {
				if (*socialNetWorkNumPtr == vectorSocialNetworks2.size() - 1) {

					*socialNetWorkNumPtr = 0;

				}
				else {
					(*socialNetWorkNumPtr) += 1;
				}
			}





			/*******
			* If search engine pagination reaches 100
			* Stops social network, email, ans search engine quene, and moves on to next search engine
			******/

			if (*searchEnginePaginationCounterPtr == 100) {
				*searchEnginePaginationCounterPtr = 0;

				// if social network pointer, and email options pointer is equal
				//than the size of  socialNetworkOptions arrary,
				//then were done, and move on
				if (*socialNetWorkNumPtr == vectorSocialNetworks2.size() - 1)
				{

					/*******
					if the last item in vector is true, and dosent match our current value
					theres more elements after our current element, we need
					this to make sure out pointer dosent get out of a range/QVector out of range.
					*****/

					if (!vectorSearchEngineOptions.last().isEmpty())
					{
						vectorSearchEngineOptionsLastItem = vectorSearchEngineOptions.last();
						if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.value(*searchEngineNumPtr)) {
							(*searchEngineNumPtr) += 1;
						}
					}
				}// end of checking if search eng pagination has reached 100


				if (*emailOptionsNumPtr == vectorEmailOptions.size())
				{
					*emailOptionsNumPtr = 0;
				}



			}


			//            qDebug() << "Search Engine Vector -->" <<vectorSearchEngineOptions;
			//            qDebug() << "Search Engine Vector Size -->" <<vectorSearchEngineOptions.size();
			//            qDebug() << "Search Engine Vector Pointer -->" << *searchEngineNumPtr;


			//            qDebug() << "Email Vector  -->" <<vectorEmailOptions;
			//            qDebug() << "Email Vector Size -->" <<vectorEmailOptions.size();
			//            qDebug() << "Email Vector Pointer -->" << *emailOptionsNumPtr;


			//            qDebug() << "Social Vector -->" <<vectorSocialNetworks2;
			//            qDebug() << "Social Vector Size -->" <<vectorSocialNetworks2.size();
			//            qDebug() << "Social Vector Pointer -->" << *socialNetWorkNumPtr;

			// qDebug() << searchEngineParam;
			// qDebug()<<  searchEngine;
			// qDebug()<<  vectorSearchEngineOptions;
			// qDebug() << *searchEnginePaginationCounterPtr;



		}// end of checking if search engine is Bing




		 /***************Yahoo***************/


		if (searchEngine == "http://yahoo.com")
		{

			/*****Email Options******/

			/******Social NetWork Options******/
			if (*emailOptionsNumPtr == vectorEmailOptions.size()) {

				*emailOptionsNumPtr = 0; // done
			}



			if (vectorEmailOptions.contains(vectorEmailOptions.value(*emailOptionsNumPtr)))
			{
				email = vectorEmailOptions.value(*emailOptionsNumPtr);
			}



			/******Social NetWork Options******/
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				*socialNetWorkNumPtr = 0; // done
			}


			if (vectorSocialNetworks2.contains(vectorSocialNetworks2.value(*socialNetWorkNumPtr))) {
				socialNetWork = vectorSocialNetworks2.value(*socialNetWorkNumPtr);
			}

			// search engines pagination number
			(*searchEnginePaginationCounterPtr) += 10;

			/*****Cast num to string to put inside query string******/
			castSearchQueryNumPtr = QString::number(*searchEnginePaginationCounterPtr);


			/**********
			if search box is not empty assign search box value to currentKeywordSearchBoxKeyword
			so incase keywordLoadListOptions is empty, we can allow it be appended to searchEngineParam
			on its own.

			--->>> If both keywordLoadListOptions, and searchbox keyword is NOT empty,
			were going to add the search box keyword to the keywordLoadListOptions List
			to be included for processing.

			************/
			if (!lineEdit_keywords_search_box.isEmpty() && fileList->isEmpty() || !lineEdit_keywords_search_box.isEmpty() && !fileList->isEmpty())
			{

				currentKeywordSearchBoxKeyword = fileList->value(0);
				searchEngineParam = "https://search.yahoo.com/search;_ylt=A2KIbNDlJS1b7nIAYNNx.9w4;_ylu=X3oDMTFjN3E2bWhuBGNvbG8DYmYxBHBvcwMxBHZ0aWQDVUkyRkJUMl8xBHNlYwNwYWdpbmF0aW9u?p=" +
					socialNetWork + "%20"
					+ email + "%20" + currentKeywordSearchBoxKeyword.replace(" ", "+") +
					"&ei=UTF-8&fr=yfp-hrmob&fr2=p%3Afp%2Cm%3Asb&_tsrc=yfp-hrmob&fp=1&b=11&pz=" + castSearchQueryNumPtr + "&xargs=0";

			}

			if (!fileList->empty())
			{

				searchEngineParam = "https://search.yahoo.com/search;_ylt=A2KIbNDlJS1b7nIAYNNx.9w4;_ylu=X3oDMTFjN3E2bWhuBGNvbG8DYmYxBHBvcwMxBHZ0aWQDVUkyRkJUMl8xBHNlYwNwYWdpbmF0aW9u?p=" +
					socialNetWork + "%20"
					+ email + "%20" + currentKeywordPtr->replace(" ", "+") +
					"&ei=UTF-8&fr=yfp-hrmob&fr2=p%3Afp%2Cm%3Asb&_tsrc=yfp-hrmob&fp=1&b=11&pz=" + castSearchQueryNumPtr + "&xargs=0";

			}




			/****Continues email quene until its the last item in array***/

			if (*searchEnginePaginationCounterPtr == 100) {
				if (*emailOptionsNumPtr == vectorEmailOptions.size() - 1) {

					*emailOptionsNumPtr = 0;

				}
				else {
					(*emailOptionsNumPtr) += 1;
				}
			}

			/****Continues social network quene until its the last item in array***/

			if (*searchEnginePaginationCounterPtr == 100) {
				if (*socialNetWorkNumPtr == vectorSocialNetworks2.size() - 1) {

					*socialNetWorkNumPtr = 0;

				}
				else {
					(*socialNetWorkNumPtr) += 1;
				}
			}





			/*******
			* If search engine pagination reaches 100
			* Stops social network, email, ans search engine quene, and moves on to next search engine
			******/

			if (*searchEnginePaginationCounterPtr == 100) {
				*searchEnginePaginationCounterPtr = 0;

				// if social network pointer, and email options pointer is equal
				//than the size of  socialNetworkOptions arrary,
				//then were done, and move on
				if (*socialNetWorkNumPtr == vectorSocialNetworks2.size() - 1)
				{

					/*******
					if the last item in vector is true, and dosent match our current value
					theres more elements after our current element, we need
					this to make sure out pointer dosent get out of a range/QVector out of range.
					*****/

					if (!vectorSearchEngineOptions.last().isEmpty())
					{
						vectorSearchEngineOptionsLastItem = vectorSearchEngineOptions.last();
						if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.value(*searchEngineNumPtr)) {
							(*searchEngineNumPtr) += 1;
						}
					}
				}// end of checking if search eng pagination has reached 100

				if (*emailOptionsNumPtr == vectorEmailOptions.size())
				{
					*emailOptionsNumPtr = 0;
				}
			}


			//            qDebug() << "Search Engine Vector -->" <<vectorSearchEngineOptions;
			//            qDebug() << "Search Engine Vector Size -->" <<vectorSearchEngineOptions.size();
			//            qDebug() << "Search Engine Vector Pointer -->" << *searchEngineNumPtr;


			//            qDebug() << "Email Vector  -->" <<vectorEmailOptions;
			//            qDebug() << "Email Vector Size -->" <<vectorEmailOptions.size();
			//            qDebug() << "Email Vector Pointer -->" << *emailOptionsNumPtr;


			//            qDebug() << "Social Vector -->" <<vectorSocialNetworks2;
			//            qDebug() << "Social Vector Size -->" <<vectorSocialNetworks2.size();
			//            qDebug() << "Social Vector Pointer -->" << *socialNetWorkNumPtr;

			// qDebug() << searchEngineParam;
			// qDebug()<<  searchEngine;
			// qDebug()<<  vectorSearchEngineOptions;
			// qDebug() << *searchEnginePaginationCounterPtr;

		}// end of checking if search engine is Yahoo

		 /****if timer is less or equal to search results combox box***/
		if (QString::number(*curlSingleProcessPtrCounter) == searchResultsPages && *multiURLOption == "1_URL_SELECTED")
		{
			if (!fileList->empty())
			{
				filterCurrentKeyword = *currentKeywordPtr;
				filterCurrentKeyword = filterCurrentKeyword.replace("+", " ");


				/***********
				If we  have more elements in list, and if so move to the next item

				-If current value dosent match the keyword
				- If current value dosent match the last item
				Then theres more elements in last
				********/

				// If Current value does not matches last item, theres more items
				if (fileList->value(*fileListPtr) != fileList->last() && fileList->value(*fileListPtr) != fileList->last().isEmpty())
				{
					(*fileListPtr) += 1;
					*curlSingleProcessPtrCounter = 0;

				}

				// Current value matches the last keyword, no more items
				if (fileList->value(*fileListPtr) == fileList->last())
				{
					if (*fileListPtr > fileList->size()) {
						*fileListPtr = 0;  // just in case the pointer goes beyond fileList size()
					}
					*curlSingleProcessPtrCounter = 0;
					//fileList->clear();
					*fileListPtr = 0;
					wStop = true;
					//emit emitSenderHarvestResults("Finised File List");
				}

			}// end of checking if fileList is empty

			if (fileList->isEmpty() && !lineEdit_keywords_search_box.isEmpty())
			{
				*fileListPtr = 0;
				//emit emitSenderHarvestResults("Finished Keyword Box");
				wStop = true;
			}

			


		}// end of checking searchResultsPages


		/*******************************CURL MULTI CONFIG*********************************/

		qDebug() << "MULTI COUNT-->" <<QString::number(*curlMultiProcessPtrCounter);
		if (otherOptions.value(2) == QString::number(*curlMultiProcessPtrCounter) && *multiURLOption == "MULTI_URL_SELECTED")
		{


			if (!fileList->empty())
			{
				filterCurrentKeyword = *currentKeywordPtr;
				filterCurrentKeyword = filterCurrentKeyword.replace("+", " ");
				qDebug() <<"Current Keyword-->"<< filterCurrentKeyword;

				/***********
				If we  have more elements in list, and if so move to the next item

				-If current value dosent match the keyword
				- If current value dosent match the last item
				Then theres more elements in last
				********/

				// If Current value does not matches last item, theres more items
				if (fileList->value(*fileListPtr) != fileList->last() && fileList->value(*fileListPtr) != fileList->last().isEmpty())
				{
					(*fileListPtr) += 1;
					*curlMultiProcessPtrCounter = 0;

				}

				// Current value matches the last keyword, no more items
				if (fileList->value(*fileListPtr) == fileList->last())
				{
					if (*fileListPtr > fileList->size()) {
						*fileListPtr = 0;  // just in case the pointer goes beyond fileList size()
					}
					*curlMultiProcessPtrCounter = 0;
					//fileList->clear();
					*fileListPtr = 0;
					wStop = true;
					qDebug() << "CURL MULTI DONE";

					//emit emitSenderHarvestResults("Finised File List");
				}

			}// end of checking if fileList is empty

			
		}

		/**********
		If multioption 1_URL_SELECTED is true and keyword box is true only use a single url
		If multioption MULTI_URL_SELECTED is true and keyword box is true only use a single url

		WORKING PROGESS BELOW TO NOT NEED MESSAGE BOX TO SHOW USER MULTI ONLY WORKS WITH A FILELIST

		if (*multiURLOption == "1_URL_SELECTED" && !lineEdit_keywords_search_box.isEmpty() ||
		*multiURLOption == "MULTI_URL_SELECTED" && !lineEdit_keywords_search_box.isEmpty())

		*********/
		if (*multiURLOption == "1_URL_SELECTED")
		{
			*multiOptionOneURL = searchEngineParam;

		}
		
		if (*multiURLOption == "MULTI_URL_SELECTED") {

			if (*curlHTTPRequestCounterPtr == 0) {
				curlHTTPRequestList.insert(0, searchEngineParam);
				

			}
			if (*curlHTTPRequestCounterPtr == 1) {
				curlHTTPRequestList.insert(1, searchEngineParam);
				
			}
			if (*curlHTTPRequestCounterPtr == 2) {
				curlHTTPRequestList.insert(2, searchEngineParam);
				

			}
			if (*curlHTTPRequestCounterPtr == 3) {
				curlHTTPRequestList.insert(3, searchEngineParam);
				
			}
			if (*curlHTTPRequestCounterPtr == 4) {
				curlHTTPRequestList.insert(4, searchEngineParam);

			}


			if (*curlHTTPRequestCounterPtr >= 5)
			{
				//reset curl multi process to 1
				(*curlMultiProcessPtrCounter) += 1;
				//reset curl http request counter to 0
				*curlHTTPRequestCounterPtr = 0;
			}

			(*curlHTTPRequestCounterPtr) += 1;

		}
		


		if (!parsedEmailList1.isEmpty())
		{
			for (int j = 0; j< parsedEmailList1.size(); j++)
			{
				if (!parsedEmailList1.value(j).isEmpty()) {
					emit emitEmailList1(parsedEmailList1.value(j));

				}
			}
		}
		//qDebug() << curlHTTPRequestList;
		QThread::currentThread()->msleep(appTimer);
	}// end of for loop


	 //emit finished();
}

void Worker::curlProcess1(const char *urls[], QString threadName)
{
	CURLM *cm = NULL;
	CURL *eh = NULL;
	CURLMsg *msg = NULL;
	CURLcode return_code;
	int still_running = 0, i = 0, msgs_left = 0;
	int http_status_code;
	const char *szUrl;
	int multiOption =5;

	/**********

	static const char *urls[] = {
	"https://www.bing.com/search?q=site%3ainstagram.com+%40gmail.com+my+dope+mixtape&qs=n&sp=-1&pq=undefined&sc=0-45&sk=&cvid=6C577B0F2A1348BBB5AF38F9AC4CA13A&first=40&FORM=PERE",
	"https://www.google.com/search?source=hp&ei=lXtWW7fyH6OatgX5o4PYAQ&q=site%3Asoundcloud.com+my+music+%40yahoo.com&oq=site%3Asoundcloud.com+my+music+%40yahoo.com&gs_l=psy-ab.3...2023.14414.0.14723.0.0.0.0.0.0.0.0..0.0....0...1.1.64.psy-ab..0.0.0....0.JXFaGFAvLuo",
	"https://www.google.com/search?ei=p3tWW6rNEYW-tQXFpILQDA&q=site%3Asoundcloud.com+my+music+%40hotmail.com&oq=site%3Asoundcloud.com+my+music+%40hotmail.com&gs_l=psy-ab.3...17574.19826.0.20059.0.0.0.0.0.0.0.0..0.0....0...1.1.64.psy-ab..0.0.0....0.9fI88o-jUE0",
	"https://www.bing.com/search?q=site%3Ainstagram.com%20%40yahoo.com%20my%20dope%20mixtape&qs=n&form=QBRE&sp=-1&pq=site%3Ainstagram.com%20%40yahoo.com%20my%20dope%20mixtape&sc=0-45&sk=&cvid=E81997583EA54E0990DB7FB7B24765B7"
	};

	*******/
	curl_global_init(CURL_GLOBAL_ALL);
	cm = curl_multi_init();

	// How many urls to parse base on user selection, 1 is default
	if (*multiURLOption == "MULTI_URL_SELECTED") {
		for (int j = 0; j < 5; ++j) {

			if (wStop) {
				break;
			}

			CURL *eh = curl_easy_init();
			curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
			curl_easy_setopt(eh, CURLOPT_URL, urls[j]);
			curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[j]);
			curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
			curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, curl_write1);
			curl_easy_setopt(eh, CURLOPT_WRITEDATA, this);
			curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L); // Tells libcurl to follow HTTP 3xx redirects
			curl_easy_setopt(eh, CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_easy_setopt(eh, CURLOPT_BUFFERSIZE, 120000L);
			curl_easy_setopt(eh, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 2.3.3; zh-tw; HTC_Pyramid Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1");

			//curl_easy_setopt(eh, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 2.3.3; zh-tw; HTC_Pyramid Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1");
			curl_multi_add_handle(cm, eh);

		}
	}
	if (*multiURLOption == "1_URL_SELECTED") {
		for (int j = 0; j < 1; ++j) {

		if (wStop) {
			break;
		}
	

		CURL *eh = curl_easy_init();
		std::string test = multiOptionOneURL->toStdString();
		curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
		curl_easy_setopt(eh, CURLOPT_URL, test.c_str());
		curl_easy_setopt(eh, CURLOPT_PRIVATE, test.c_str());
		curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
		curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, curl_write1);
		curl_easy_setopt(eh, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L); // Tells libcurl to follow HTTP 3xx redirects
		curl_easy_setopt(eh, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(eh, CURLOPT_BUFFERSIZE, 120000L);
		curl_easy_setopt(eh, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 2.3.3; zh-tw; HTC_Pyramid Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1");
		curl_multi_add_handle(cm, eh);

	 }
	}
	

	

	do {
		int numfds = 0;
		int res = curl_multi_wait(cm, NULL, 0, MAX_WAIT_MSECS, &numfds);
		if (res != CURLM_OK) {
			fprintf(stderr, "error: curl_multi_wait() returned %d\n", res);
			qDebug() << res;

		}

		if (!numfds) {
			fprintf(stderr, "error: curl_multi_wait() numfds=%d\n", numfds);
			qDebug() << res;
		}

		curl_multi_perform(cm, &still_running);

	} while (still_running);

	while ((msg = curl_multi_info_read(cm, &msgs_left))) {
		if (msg->msg == CURLMSG_DONE) {
			eh = msg->easy_handle;

			return_code = msg->data.result;
			if (return_code != CURLE_OK) {
				fprintf(stderr, "CURL error code: %d\n", msg->data.result);
				std::cout << msg->data.result;
				qDebug() << msg->data.result;
				continue;
			}

			// Get HTTP status code
			http_status_code = 0;
			szUrl = NULL;

			curl_easy_getinfo(eh, CURLINFO_RESPONSE_CODE, &http_status_code);
			curl_easy_getinfo(eh, CURLINFO_PRIVATE, &szUrl);

			if (http_status_code == 200) 
			{
				qDebug() << "200 OK";
				//qDebug() << "RESULT-->" << msg->data.result;
				//qDebug() << "URL-->" << szUrl;
			}
			else 
			{
				qDebug() << "ERROR CODE->" << http_status_code;
				qDebug() << "URL in ERROR-->" << http_status_code;
			}

			curl_multi_remove_handle(cm, eh);
			curl_easy_cleanup(eh);
		}
		else {
			qDebug() << "error: after curl_multi_info_read()-->" << msg->msg;
		}
	}

	curl_multi_cleanup(cm);



}


void Worker::doWork1()
{

	for (int i = 0; i < 999999; i++)
	{
		if (wStop) {
			break;
		}
		

		if (*multiURLOption == "1_URL_SELECTED")
		{
			if (!multiOptionOneURL->isEmpty()) {
				std::string single_url = multiOptionOneURL->toStdString();
				const char *single_urls[] =
				{
					single_url.c_str()
				};
				curlProcess1(single_urls, "Single Instance");
			}
		
		}

		if (!curlHTTPRequestList.isEmpty()) {

			if (*multiURLOption == "MULTI_URL_SELECTED") 
			{

				if (curlHTTPRequestList.size() >= 5)
				{
					/*******
					Use only 4 indexes at one time within curlHTTPRequest list

					******/

					std::string urls0;
					std::string urls1;
					std::string urls2;
					std::string urls3;
					std::string urls4;


					if (!curlHTTPRequestList.value(0).isEmpty())
					{
						urls0 = curlHTTPRequestList.value(0).toStdString();

					}
					if (!curlHTTPRequestList.value(1).isEmpty())
					{
						urls1 = curlHTTPRequestList.value(1).toStdString();

					}
					if (!curlHTTPRequestList.value(2).isEmpty())
					{
						urls2 = curlHTTPRequestList.value(2).toStdString();

					}
					if (!curlHTTPRequestList.value(3).isEmpty())
					{
						urls3 = curlHTTPRequestList.value(3).toStdString();

					}
					if (!curlHTTPRequestList.value(4).isEmpty())
					{
						urls4 = curlHTTPRequestList.value(4).toStdString();

					}

					


					const char *urls[] =
					{
						urls0.c_str(),
						urls1.c_str(),
						urls2.c_str(),
						urls3.c_str(),
						urls4.c_str()
					};


					curlProcess1(urls, "Multi");
					curlHTTPRequestList.clear();

					//qDebug() << curlHTTPRequest.value(0);
					//qDebug() << curlHTTPRequest.value(1);
					//qDebug() << curlHTTPRequest.value(2);
					//qDebug() << curlHTTPRequest.value(3);
					//qDebug() << curlHTTPRequest.value(4);

				}

			}// end if multi is selected

		}


		if (wStop)
		{
			break;
		}
		QThread::currentThread()->msleep(6000);
	}// end of for loop
}


void Worker::getProxyFile(QString fileName)
{
	//qDebug() << "Received proxy file name" << fileName ;
}

void Worker::receiverEmptyProxyServer(QString isEmpty)
{
	//    if (isEmpty == "Empty")
	//    {

	//        isProxyEmpty = true;
	//    }

	//    if (isEmpty == "Not Empty")
	//    {
	//        isProxyEmpty = false;

	//    }
}

void Worker::receiverStopThreadCounters(QString stopThreadCounter)
{
	// isStopStartThreadCounter = false;
	// *workerCounterPtr = 0;
	// *proxyServerCounterPtr = 0;
}

void Worker::receiverStartThreadCounters(QString startThreadCounter)
{

	// isStopStartThreadCounter = true;
	// *workerCounterPtr = 0;
	// *proxyServerCounterPtr = 0;



}

void Worker::receiverAppOptions(int harvesterTimer, int proxyRotateInterval) {

	/*********
	Chrono is namespace, meaning a certain class will on be in its scope
	Within this namespace we have a class thats a template name seconds, that takes a long long type -- which is seconds
	in our case. So to create a instance of seconds class, we need to create a object, that taqDebugkes a long. In our case s(harvesterTimer)
	The s object of type seconds, also has members as well
	*******/
	std::chrono::seconds s(harvesterTimer);
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(s);
	// cast long long to int
	*harvesterTimerPtr = static_cast<int>(ms.count());

	*proxyRotateIntervalPtr = proxyRotateInterval;

}

void Worker::processedEmails(QString emails) {
	//emit emitEmailList(emails);
	//qDebug() << s;
}

void Worker::processedEmails2(QString emails) {
	//emit emitEmailList2(emails);
	//qDebug() << s;
}

size_t Worker::curl_write1(char *ptr, size_t size, size_t nmemb, void *stream)
{
	//buffer.append((char*)ptr, size*nmemb);

	size_t realsize = size * nmemb;
	QByteArray response(ptr, static_cast<int>(realsize));
	QString responseString = QString(response);
	QString plainText = QTextDocumentFragment::fromHtml(responseString).toPlainText();
	QString filteredNewLine = plainText.replace("\n", " ");
	QRegularExpression re("[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}");
	QRegularExpressionMatchIterator i = re.globalMatch(filteredNewLine);
	//qDebug() << filteredNewLine;
	QStringList words;
	QStringList test;
	QString word;
	QRegularExpressionMatch match;
	QList<QString> list;
	QString emailList;
	int num = 0;


	while (i.hasNext()) {

		if (static_cast<Worker*>(stream)->wStop == true) {
			break;
		}
		QRegularExpressionMatch match = i.next();
		if (!match.captured(0).isEmpty())
			word = match.captured(0);
		//words << word;
		//MainWindow::emails = &words;
		if (!word.isEmpty()) {
			words << word;
			// emailList = words.value(num);
			list << words;
			//emit emitEmailList(words.value(num));
			//qDebug() << words.value(num);
			//static_cast<Worker*>(stream)->processedEmails(words.value(num));
			//qDebug() << num;
			if (!words.value(num).isEmpty()) {
				parsedEmailList1.insert(num, words.value(num));
			}
			num++;

		} // end words !empty

	} // end while



	return size*nmemb;

}





