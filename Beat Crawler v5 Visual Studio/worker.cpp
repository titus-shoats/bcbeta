
/*
Copyright 2018 Titus Shoats

*/

#include "worker.h"


//QStringList *Worker::parsedEmailList;


std::string buffer;
QStringList parsedEmailList1;
QStringList parsedEmailList2;
QStringList parsedEmailList3;
QStringList parsedEmailList4;
QStringList parsedEmailList5;

Worker::Worker(QObject *parent) :
	QObject(parent)
{

	params = "";
	paramsPtr = new QList <QString>();
	urlQueryParam = new QString();
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

	wStop = false;


	proxyServers = new QList <QString>();
	fileList = new QStringList();

	fileListNum = 0;
	fileListPtr = &fileListNum;
	currentKeyword = "";
	currentKeywordPtr = &currentKeyword;
	keywordListNumPtrNum = 0;
	keywordListNumPtrCounter = &keywordListNumPtrNum;
	searchEngineNum = 0;
	searchEngineNumPtr = &searchEngineNum;
	emailOptionsNum = 0;
	emailOptionsNumPtr = &emailOptionsNum;
	socialNetWorkNum = 0;
	socialNetWorkNumPtr = &socialNetWorkNum;
	keywordListSearchEngineCounterNum = 0;
	keywordListSearchEngineCounterPtr = &keywordListSearchEngineCounterNum;
	errbuf[0] = 0;




}

Worker::~Worker()
{
	delete paramsPtr;
	delete urlQueryParam;
	delete proxies;
	delete httpRequestList;
	delete fileList;
	delete proxyServers;
	
	
	
}

void Worker::requestWork()
{
	emit workRequested();
}

void Worker::abort(){}

void Worker::stop()
{
	wStop = true;
}

void Worker::receiverRemoveThreadFileList()
{
	fileList->clear();

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
	//emit emitfinishReadingKeywordFile();

}



void Worker::curlProcess1(QString url, QString threadName)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		std::string urlString = url.toStdString();
		curl_easy_setopt(curl, CURLOPT_URL, urlString.c_str());

		/* google.com is redirected, so we tell LibCurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write1);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		long httpResponseCode;
		char* effectiveURL;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveURL);

		/*********HTTP CODE*******/
		// Log errors,or show message to user if response code is not 200
		if (httpResponseCode == 200 && res == CURLE_OK) {

			// emit senderCurlResponseInfo("Request Succeded");
			qDebug() << "HTTP  Code--> " << httpResponseCode << " " << threadName
				<< " - URL-> " << effectiveURL << "\n";
		}

		if (httpResponseCode == 503) {

			qDebug() << "503 ERROR CODE " << threadName << "URL-> " << effectiveURL;
			//  emit senderCurlResponseInfo("503");
		}

		if (res != CURLE_OK) {

			switch (res)
			{
			case 5: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;
				break;
			case 7: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 35: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 56: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;


			default: qDebug() << threadName << " Default Switch Statement Curl Code--> " << res
				<< "URL-> " << effectiveURL;;

			}
			/**************
			*checks curl erros codes
			*
			* 5 -- Couldn't resolve proxy. The given proxy host could not be resolved.
			* 7 -- Failed to connect() to host or proxy.
			*
			*********/
			if (res == 5 || res == 7 || res == 35)
			{
				// emit senderCurlResponseInfo("Proxy Error");
				qDebug() << "Proxy Error" << "URL-> " << effectiveURL << " " << threadName;
			}
		}

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}
}

void Worker::curlProcess2(QString url, QString threadName)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		std::string urlString = url.toStdString();
		curl_easy_setopt(curl, CURLOPT_URL, urlString.c_str());

		/* google.com is redirected, so we tell LibCurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write2);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		long httpResponseCode;
		char* effectiveURL;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveURL);

		/*********HTTP CODE*******/
		// Log errors,or show message to user if response code is not 200
		if (httpResponseCode == 200 && res == CURLE_OK) {

			// emit senderCurlResponseInfo("Request Succeded");
			qDebug() << "HTTP  Code--> " << httpResponseCode << " " << threadName
				<< " - URL-> " << effectiveURL << "\n";
		}

		if (httpResponseCode == 503) {

			qDebug() << "503 ERROR CODE " << threadName << "URL-> " << effectiveURL;
			//  emit senderCurlResponseInfo("503");
		}

		if (res != CURLE_OK) {

			switch (res)
			{
			case 5: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;
				break;
			case 7: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 35: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 56: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;


			default: qDebug() << threadName << " Default Switch Statement Curl Code--> " << res
				<< "URL-> " << effectiveURL;;

			}
			/**************
			*checks curl erros codes
			*
			* 5 -- Couldn't resolve proxy. The given proxy host could not be resolved.
			* 7 -- Failed to connect() to host or proxy.
			*
			*********/
			if (res == 5 || res == 7 || res == 35)
			{
				// emit senderCurlResponseInfo("Proxy Error");
				qDebug() << "Proxy Error" << "URL-> " << effectiveURL << " " << threadName;
			}
		}

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}
}

void Worker::curlProcess3(QString url, QString threadName)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		std::string urlString = url.toStdString();
		curl_easy_setopt(curl, CURLOPT_URL, urlString.c_str());

		/* google.com is redirected, so we tell LibCurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write3);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		long httpResponseCode;
		char* effectiveURL;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveURL);

		/*********HTTP CODE*******/
		// Log errors,or show message to user if response code is not 200
		if (httpResponseCode == 200 && res == CURLE_OK) {

			// emit senderCurlResponseInfo("Request Succeded");
			qDebug() << "HTTP  Code--> " << httpResponseCode << " " << threadName
				<< " - URL-> " << effectiveURL << "\n";
		}

		if (httpResponseCode == 503) {

			qDebug() << "503 ERROR CODE " << threadName << "URL-> " << effectiveURL;
			//  emit senderCurlResponseInfo("503");
		}

		if (res != CURLE_OK) {

			switch (res)
			{
			case 5: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;
				break;
			case 7: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 35: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 56: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;


			default: qDebug() << threadName << " Default Switch Statement Curl Code--> " << res
				<< "URL-> " << effectiveURL;;

			}
			/**************
			*checks curl erros codes
			*
			* 5 -- Couldn't resolve proxy. The given proxy host could not be resolved.
			* 7 -- Failed to connect() to host or proxy.
			*
			*********/
			if (res == 5 || res == 7 || res == 35)
			{
				// emit senderCurlResponseInfo("Proxy Error");
				qDebug() << "Proxy Error" << "URL-> " << effectiveURL << " " << threadName;
			}
		}

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}
}

void Worker::curlProcess4(QString url, QString threadName)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		std::string urlString = url.toStdString();
		curl_easy_setopt(curl, CURLOPT_URL, urlString.c_str());

		/* google.com is redirected, so we tell LibCurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write4);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		long httpResponseCode;
		char* effectiveURL;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveURL);

		/*********HTTP CODE*******/
		// Log errors,or show message to user if response code is not 200
		if (httpResponseCode == 200 && res == CURLE_OK) {

			// emit senderCurlResponseInfo("Request Succeded");
			qDebug() << "HTTP  Code--> " << httpResponseCode << " " << threadName
				<< " - URL-> " << effectiveURL << "\n";
		}

		if (httpResponseCode == 503) {

			qDebug() << "503 ERROR CODE " << threadName << "URL-> " << effectiveURL;
			//  emit senderCurlResponseInfo("503");
		}

		if (res != CURLE_OK) {

			switch (res)
			{
			case 5: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;
				break;
			case 7: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 35: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 56: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;


			default: qDebug() << threadName << " Default Switch Statement Curl Code--> " << res
				<< "URL-> " << effectiveURL;;

			}
			/**************
			*checks curl erros codes
			*
			* 5 -- Couldn't resolve proxy. The given proxy host could not be resolved.
			* 7 -- Failed to connect() to host or proxy.
			*
			*********/
			if (res == 5 || res == 7 || res == 35)
			{
				// emit senderCurlResponseInfo("Proxy Error");
				qDebug() << "Proxy Error" << "URL-> " << effectiveURL << " " << threadName;
			}
		}

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}
}


void Worker::curlProcess5(QString url, QString threadName)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		std::string urlString = url.toStdString();
		curl_easy_setopt(curl, CURLOPT_URL, urlString.c_str());

		/* google.com is redirected, so we tell LibCurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write5);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		long httpResponseCode;
		char* effectiveURL;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpResponseCode);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveURL);

		/*********HTTP CODE*******/
		// Log errors,or show message to user if response code is not 200
		if (httpResponseCode == 200 && res == CURLE_OK) {

			// emit senderCurlResponseInfo("Request Succeded");
			qDebug() << "HTTP  Code--> " << httpResponseCode << " " << threadName
				<< " - URL-> " << effectiveURL << "\n";
		}

		if (httpResponseCode == 503) {

			qDebug() << "503 ERROR CODE " << threadName << "URL-> " << effectiveURL;
			//  emit senderCurlResponseInfo("503");
		}

		if (res != CURLE_OK) {

			switch (res)
			{
			case 5: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;
				break;
			case 7: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 35: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;
			case 56: qDebug() << threadName << " Curl code-> " << res << " Message->" << errbuf
				<< "URL-> " << effectiveURL;;
				break;


			default: qDebug() << threadName << " Default Switch Statement Curl Code--> " << res
				<< "URL-> " << effectiveURL;;

			}
			/**************
			*checks curl erros codes
			*
			* 5 -- Couldn't resolve proxy. The given proxy host could not be resolved.
			* 7 -- Failed to connect() to host or proxy.
			*
			*********/
			if (res == 5 || res == 7 || res == 35)
			{
				// emit senderCurlResponseInfo("Proxy Error");
				qDebug() << "Proxy Error" << "URL-> " << effectiveURL << " " << threadName;
			}
		}

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}
}
void Worker::readEmailFile(){
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
			emit emitEmailList1(parsedEmailList1.at(j));
		}
	}


	if (!parsedEmailList2.isEmpty())
	{
		for (int j = 0; j< parsedEmailList2.size(); j++)
		{
			emit emitEmailList2(parsedEmailList2.at(j));
		}
	}

	if (!parsedEmailList3.isEmpty())
	{
		for (int j = 0; j< parsedEmailList3.size(); j++)
		{
			emit emitEmailList3(parsedEmailList3.at(j));
		}
	}
	if (!parsedEmailList4.isEmpty())
	{
		for (int j = 0; j< parsedEmailList4.size(); j++)
		{
			emit emitEmailList4(parsedEmailList4.at(j));
		}
	}
	if (!parsedEmailList5.isEmpty())
	{
		for (int j = 0; j< parsedEmailList5.size(); j++)
		{
			emit emitEmailList5(parsedEmailList5.at(j));
		}
	}
}


void Worker::curlParams(QList<QVector <QString>>vectorSearchOptions,
	QString lineEdit_keywords_search_box, QList <QString> *proxyServers,
	QList<int>timerOptions, QString searchResultsPages)
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
	QVector<QString>vectorSearchEngineOptions = vectorSearchOptions.at(0);
	QVector<QString>vectorEmailOptions = vectorSearchOptions.at(1);
	QVector<QString>vectorSocialNetworks2 = vectorSearchOptions.at(2);
	int harvesterTimer = timerOptions.at(0);
	int proxyRotateInterval = timerOptions.at(1);

	int appTimer;
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
	
	if (!lineEdit_keywords_search_box.isEmpty() && fileList->isEmpty() ||
		!lineEdit_keywords_search_box.isEmpty() && !fileList->isEmpty())
	{
		fileList->removeAt(0);
		fileList->insert(0, lineEdit_keywords_search_box);

	}


	for (int i = 0; i < 999999; i++)
	{

		parsedEmails();
		if (wStop) {
			///lineEdit_keywords_search_box.clear();
		
			if (keywordListNumPtrCounter != NULL && emailOptionsNumPtr!=NULL && searchEngineNumPtr!=NULL
				&& keywordListSearchEngineCounterPtr !=NULL && proxyServerCounterPtr !=NULL
				&& workerCounterPtr !=NULL) {

				keywordListNumPtrCounter = &keywordListNumPtrNum;
				emailOptionsNumPtr = &emailOptionsNum;
				searchEngineNumPtr = &searchEngineNum;
				keywordListSearchEngineCounterPtr = &keywordListSearchEngineCounterNum;
				proxyServerCounterPtr = &proxyServerCounterNum;
				workerCounterPtr = &workerCounterNum;

				*keywordListNumPtrCounter = 0;
				*emailOptionsNumPtr = 0;
				*searchEngineNumPtr = 0;
				*keywordListSearchEngineCounterPtr = 0;
				*proxyServerCounterPtr = 0;
				*workerCounterPtr = 0;

			}
			//fileList->clear();
			break;
		}

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
				*proxies = proxyServers->at(*proxyServerCounterPtr);
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




		if (!fileList->isEmpty())
		{
			// on load is 0 which is the first index value
			*currentKeywordPtr = fileList->at(*fileListPtr);
		}

		(*keywordListNumPtrCounter) += 1;

		/**********Search Engine Options ******/

		// if pointer is equal to size of vector were done
		if (*searchEngineNumPtr == vectorSearchEngineOptions.size())
		{
			*searchEngineNumPtr = 0; // done
		}

		// if vector search engine contains current search engine, assign it it to search eng var
		if (vectorSearchEngineOptions.contains(vectorSearchEngineOptions.at(*searchEngineNumPtr)))
		{
			searchEngine = vectorSearchEngineOptions.at(*searchEngineNumPtr);
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



			if (vectorEmailOptions.contains(vectorEmailOptions.at(*emailOptionsNumPtr)))
			{
				email = vectorEmailOptions.at(*emailOptionsNumPtr);
			}



			/******Social NetWork Options******/
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				*socialNetWorkNumPtr = 0; // done
			}


			if (vectorSocialNetworks2.contains(vectorSocialNetworks2.at(*socialNetWorkNumPtr))) {
				socialNetWork = vectorSocialNetworks2.at(*socialNetWorkNumPtr);
			}

			// search engines pagination number
			(*keywordListSearchEngineCounterPtr) += 10;

			/*****Cast num to string to put inside query string******/
			castSearchQueryNumPtr = QString::number(*keywordListSearchEngineCounterPtr);


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

				currentKeywordSearchBoxKeyword = fileList->at(0);
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

			if (*keywordListSearchEngineCounterPtr == 100) {
				if (*emailOptionsNumPtr == vectorEmailOptions.size() - 1) {

					*emailOptionsNumPtr = 0;

				}
				else {
					(*emailOptionsNumPtr) += 1;
				}
			}

			/****Continues social network quene until its the last item in array***/

			if (*keywordListSearchEngineCounterPtr == 100) {
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

			if (*keywordListSearchEngineCounterPtr == 100) {
				*keywordListSearchEngineCounterPtr = 0;

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
						if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.at(*searchEngineNumPtr)) {
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
			// qDebug() << *keywordListSearchEngineCounterPtr;



		}// end of checking if search engine is Google



		 /************Bing*************/

		if (searchEngine == "http://bing.com")
		{

			/*****Email Options******/

			/******Social NetWork Options******/
			if (*emailOptionsNumPtr == vectorEmailOptions.size()) {

				*emailOptionsNumPtr = 0; // done
			}



			if (vectorEmailOptions.contains(vectorEmailOptions.at(*emailOptionsNumPtr)))
			{
				email = vectorEmailOptions.at(*emailOptionsNumPtr);
			}



			/******Social NetWork Options******/
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				*socialNetWorkNumPtr = 0; // done
			}


			if (vectorSocialNetworks2.contains(vectorSocialNetworks2.at(*socialNetWorkNumPtr))) {
				socialNetWork = vectorSocialNetworks2.at(*socialNetWorkNumPtr);
			}

			// search engines pagination number
			(*keywordListSearchEngineCounterPtr) += 10;

			/*****Cast num to string to put inside query string******/
			castSearchQueryNumPtr = QString::number(*keywordListSearchEngineCounterPtr);


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

				currentKeywordSearchBoxKeyword = fileList->at(0);
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

			if (*keywordListSearchEngineCounterPtr == 100) {
				if (*emailOptionsNumPtr == vectorEmailOptions.size() - 1) {

					*emailOptionsNumPtr = 0;

				}
				else {
					(*emailOptionsNumPtr) += 1;
				}
			}

			/****Continues social network quene until its the last item in array***/

			if (*keywordListSearchEngineCounterPtr == 100) {
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

			if (*keywordListSearchEngineCounterPtr == 100) {
				*keywordListSearchEngineCounterPtr = 0;

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
						if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.at(*searchEngineNumPtr)) {
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
			// qDebug() << *keywordListSearchEngineCounterPtr;



		}// end of checking if search engine is Bing




		 /***************Yahoo***************/


		if (searchEngine == "http://yahoo.com")
		{

			/*****Email Options******/

			/******Social NetWork Options******/
			if (*emailOptionsNumPtr == vectorEmailOptions.size()) {

				*emailOptionsNumPtr = 0; // done
			}



			if (vectorEmailOptions.contains(vectorEmailOptions.at(*emailOptionsNumPtr)))
			{
				email = vectorEmailOptions.at(*emailOptionsNumPtr);
			}



			/******Social NetWork Options******/
			if (*socialNetWorkNumPtr == vectorSocialNetworks2.size()) {

				*socialNetWorkNumPtr = 0; // done
			}


			if (vectorSocialNetworks2.contains(vectorSocialNetworks2.at(*socialNetWorkNumPtr))) {
				socialNetWork = vectorSocialNetworks2.at(*socialNetWorkNumPtr);
			}

			// search engines pagination number
			(*keywordListSearchEngineCounterPtr) += 10;

			/*****Cast num to string to put inside query string******/
			castSearchQueryNumPtr = QString::number(*keywordListSearchEngineCounterPtr);


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

				currentKeywordSearchBoxKeyword = fileList->at(0);
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

			if (*keywordListSearchEngineCounterPtr == 100) {
				if (*emailOptionsNumPtr == vectorEmailOptions.size() - 1) {

					*emailOptionsNumPtr = 0;

				}
				else {
					(*emailOptionsNumPtr) += 1;
				}
			}

			/****Continues social network quene until its the last item in array***/

			if (*keywordListSearchEngineCounterPtr == 100) {
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

			if (*keywordListSearchEngineCounterPtr == 100) {
				*keywordListSearchEngineCounterPtr = 0;

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
						if (vectorSearchEngineOptionsLastItem != vectorSearchEngineOptions.at(*searchEngineNumPtr)) {
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
			// qDebug() << *keywordListSearchEngineCounterPtr;

		}// end of checking if search engine is Yahoo

		 /****if timer is less or equal to search results combox box***/
		 // qDebug() << searchResultsPages;
		 // qDebug() << *keywordListNumPtrCounter;

		 //curlProcess(searchEngineParam, "Thread 1");

		//qDebug() << searchEngineParam;
		if (QString::number(*keywordListNumPtrCounter) == searchResultsPages)
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
					*keywordListNumPtrCounter = 0;

				}

				// Current value matches the last keyword
				if (fileList->value(*fileListPtr) == fileList->last())
				{
					if (*fileListPtr > fileList->size()) {
						*fileListPtr = 0;  // just in case the pointer goes beyond fileList size()
					}
					*keywordListNumPtrCounter = 0;
					fileList->clear();
					*fileListPtr = 0;
					wStop = true;
					emit emitSenderHarvestResults("Finised File List");
				}

			}// end of checking if fileList is empty

			if (fileList->isEmpty() && !lineEdit_keywords_search_box.isEmpty())
			{
				*fileListPtr = 0;
				emit emitSenderHarvestResults("Finished Keyword Box");
				wStop = true;
			}



		}// end of checking searchResultsPages

		
		if (*curlHTTPRequestCounterPtr == 0) {
			curlHTTPRequestList.insert(0,searchEngineParam);

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
			*curlHTTPRequestCounterPtr = 0;
		}
		//qDebug() << *curlHTTPRequestCounterPtr;

		(*curlHTTPRequestCounterPtr) += 1;
		

		QThread::currentThread()->msleep(appTimer);
	}// end of for loop


	emit finished();
}

void Worker::doWork1()
{

	for (int i = 0; i < 999999; i++)
	{
		

		if (!curlHTTPRequestList.isEmpty()) {
			if (curlHTTPRequestList.size() >= 5)
			{

				/*******
				Use only 4 indexes at one time within curlHTTPRequest list
				
				******/


				if (!curlHTTPRequestList.value(0).isEmpty())
				{
					curlProcess1(curlHTTPRequestList.value(0), "Instance 1");

				}
				if (!curlHTTPRequestList.value(1).isEmpty())
				{
					curlProcess1(curlHTTPRequestList.value(1), "Instance 2");

				}
				if (!curlHTTPRequestList.value(2).isEmpty())
				{
					curlProcess1(curlHTTPRequestList.value(2), "Instance 3");

				}
				if (!curlHTTPRequestList.value(3).isEmpty())
				{
					curlProcess1(curlHTTPRequestList.value(3), "Instance 4");

				}
				if (!curlHTTPRequestList.value(4).isEmpty())
				{
					curlProcess1(curlHTTPRequestList.value(4), "Instance 5");

				}
				curlHTTPRequestList.clear();
				
				//qDebug() << curlHTTPRequest.value(0);
				//qDebug() << curlHTTPRequest.value(1);
				//qDebug() << curlHTTPRequest.value(2);
				//qDebug() << curlHTTPRequest.value(3);
				//qDebug() << curlHTTPRequest.value(4);

				
			}
		}

	
		if (wStop)
		{
			break;
		}
		QThread::currentThread()->msleep(6000);
	}// end of for loop
}



void Worker::getParam(QString url, QString userAgent, QList <QString> *proxyServers)
{
	*urlQueryParam = url;
	QStringList httpList;

	// if proxies are empty
	if (proxies->isEmpty())
	{
		httpList << url << userAgent;
		httpRequestList->append(httpList);

	}
	// if proxies are not empty
	if (!proxies->isEmpty())
	{
		httpList << url << userAgent << *proxies;
		httpRequestList->append(httpList);

	}




	// if workerCounter == *proxyRotateIntervalPtr, reset workerCounter ; if certain number of
	// http request have been made rotate proxy
	if (*workerCounterPtr <= *proxyRotateIntervalPtr)
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
			*proxies = proxyServers->at(*proxyServerCounterPtr);
			//qDebug() << "Counter-->" << *proxies;

		}
		//qDebug() << "Counter-->" << *proxyServerCounterPtr;
		//qDebug() << "Proxies-->" << *proxies;

	}


	// if workerCounter is greater than *proxyRotateIntervalPtr/ amount of http request before proxy rotates
	if (*workerCounterPtr >= *proxyRotateIntervalPtr)
	{
		// restart workerCounter
		*workerCounterPtr = 0;
		// increment proxyServerPtr to go through each proxyServer index every interval
		(*proxyServerCounterPtr) += 1;

	}

	// increment workerCounter if we have not hit our http request limit to rotate each proxy
	(*workerCounterPtr) += 1;



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
		QRegularExpressionMatch match = i.next();

		if (!match.captured(0).isEmpty())
			word = match.captured(0);
		//words << word;
		//MainWindow::emails = &words;
		if (!word.isEmpty()) {
			words << word;
			// emailList = words.at(num);
			list << words;
			//emit emitEmailList(words.at(num));
			//qDebug() << words.at(num);
			static_cast<Worker*>(stream)->processedEmails(words.at(num));
			//qDebug() << num;
			parsedEmailList1.insert(num, words.at(num));

			num++;

		} // end words !empty

	} // end while


	  //	struct MemoryStruct *mem = (struct MemoryStruct *)stream;

	  //increase the size of "memory" by the size of the bytes that we have read
	  //	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);


	return size*nmemb;

}

size_t Worker::curl_write2(char *ptr, size_t size, size_t nmemb, void *stream)
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
		QRegularExpressionMatch match = i.next();

		if (!match.captured(0).isEmpty())
			word = match.captured(0);
		if (!word.isEmpty()) {
			words << word;
			list << words;
			//emit emitEmailList(words.at(num));
			//qDebug() << words.at(num);
			static_cast<Worker*>(stream)->processedEmails2(words.at(num));			

			parsedEmailList2.insert(num, words.at(num));
			num++;

		} // end words if !empty

	} // end while

	  //	struct MemoryStruct *mem = (struct MemoryStruct *)stream;

	
	  //increase the size of "memory" by the size of the bytes that we have read
	  //	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);


	return size*nmemb;

}

size_t Worker::curl_write3(char *ptr, size_t size, size_t nmemb, void *stream)
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
		QRegularExpressionMatch match = i.next();

		if (!match.captured(0).isEmpty())
			word = match.captured(0);
		if (!word.isEmpty()) {
			words << word;
			list << words;
			//emit emitEmailList(words.at(num));
			//qDebug() << words.at(num);
			static_cast<Worker*>(stream)->processedEmails2(words.at(num));


			parsedEmailList3.insert(num, words.at(num));

			num++;

		} // end words if !empty

	} // end while

	  //	struct MemoryStruct *mem = (struct MemoryStruct *)stream;


	  //increase the size of "memory" by the size of the bytes that we have read
	  //	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);


	return size*nmemb;

}

size_t Worker::curl_write4(char *ptr, size_t size, size_t nmemb, void *stream)
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
		QRegularExpressionMatch match = i.next();

		if (!match.captured(0).isEmpty())
			word = match.captured(0);
		if (!word.isEmpty()) {
			words << word;
			list << words;
			//emit emitEmailList(words.at(num));
			//qDebug() << words.at(num);
			static_cast<Worker*>(stream)->processedEmails2(words.at(num));


			parsedEmailList4.insert(num, words.at(num));

			num++;

		} // end words if !empty

	} // end while

	  //	struct MemoryStruct *mem = (struct MemoryStruct *)stream;


	  //increase the size of "memory" by the size of the bytes that we have read
	  //	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);


	return size*nmemb;

}


size_t Worker::curl_write5(char *ptr, size_t size, size_t nmemb, void *stream)
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
		QRegularExpressionMatch match = i.next();

		if (!match.captured(0).isEmpty())
			word = match.captured(0);
		if (!word.isEmpty()) {
			words << word;
			list << words;
			//emit emitEmailList(words.at(num));
			//qDebug() << words.at(num);
			static_cast<Worker*>(stream)->processedEmails2(words.at(num));



			parsedEmailList5.insert(num, words.at(num));
			num++;

		} // end words if !empty

	} // end while

	  //	struct MemoryStruct *mem = (struct MemoryStruct *)stream;


	  //increase the size of "memory" by the size of the bytes that we have read
	  //	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);


	return size*nmemb;

}






