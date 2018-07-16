
/*
Copyright 2013 Fabien Pierre-Nicolas.
- Primarily authored by Fabien Pierre-Nicolas

This file is part of simple-qt-thread-example, a simple example to demonstrate how to use threads.
This example is explained on http://fabienpn.wordpress.com/qt-thread-simple-and-stable-with-sources/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This progra is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "worker.h"
#include <QTimer>
#include <QEventLoop>
#include <QThread>
#include <QDebug>
//#include "curleasy.h"
//#include "curlmulti.h"
#include <QRegularExpressionMatchIterator>
#include <QTextDocumentFragment>
#include <QList>
#include <chrono>


#define MAX_WAIT_MSECS 30*1000 /* Wait max. 30 seconds */
#define CNT 4
//QStringList *Worker::parsedEmailList;


std::string buffer;
Worker::Worker(QObject *parent) :
	QObject(parent)
{
	_working = false;
	_abort = false;
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

}

Worker::~Worker()
{
	delete paramsPtr;
	delete urlQueryParam;
	delete proxies;
	//delete parsedEmailList;
	delete httpRequestList;
}

void Worker::requestWork()
{
	mutex.lock();
	_working = true;
	_abort = false;
	qDebug() << "First Thread Request worker start in Thread " << thread()->currentThreadId();
	mutex.unlock();

	emit workRequested();

}

void Worker::abort()
{
	mutex.lock();
	if (_working) {
		_abort = true;
		qDebug() << "First Thread Request worker aborting in Thread " << thread()->currentThreadId();
	}
	mutex.unlock();
}

void Worker::doWork()
{

	qDebug() << "First Thread Starting worker process in Thread " << thread()->currentThreadId();





	/**************
	We increment i 60 times
	Anytime i is increment we have a set interval that does something
	In our case scrape -- so every time I get incremented we have a scraping interval
	So we have to send a signal here to change the 60 to something else
	************/
	//for (int counter = 0; counter < 100000; counter ++) {
	for (;;) {
		// Checks if the process should be aborted
		mutex.lock();
		bool abort = _abort;
		mutex.unlock();
		// *workerCounter = counter;

		if (abort) {
			qDebug() << "First Thread Aborting worker process in Thread " << thread()->currentThreadId();
			break;
		}


		// This will stupidly wait 1 sec doing nothing...
		QEventLoop loop;
		QTimer::singleShot(*harvesterTimerPtr, &loop, SLOT(quit()));
		loop.exec();

		// send signal to grab parameter options
		emit emitParameters();

		// send signal to show Keywords that are currently Queue
		emit emitKeywordQueue();


		//if url query that we recieved is not empty
		if (!urlQueryParam->isEmpty()) {

			std::string url = urlQueryParam->toStdString();
			//std::string userAgent = paramsPtr->at(1).toStdString();
			std::string proxy = proxies->toStdString();


			CurlEasy *curl = new CurlEasy;
			curl->set(CURLOPT_URL, url.c_str());
			if (!proxies->isEmpty()) {
				curl->set(CURLOPT_PROXY, proxy.c_str());
			}
			if (proxies->isEmpty()) {
				curl->set(CURLOPT_PROXY, NULL);
			}
			curl->set(CURLOPT_FOLLOWLOCATION, 1L); // Tells libcurl to follow HTTP 3xx redirects
			curl->set(CURLOPT_SSL_VERIFYPEER, FALSE);
			curl->perform();

			curl->setWriteFunction([this](char *data, size_t size)->size_t {


				//If response is not 200, obviously we wont get any emails,
				// but were only going to parse emails, only
				// so this means, we only get emails if::
				//1 reponse code is 200

				// qDebug() << "Data from google.com: " << QByteArray(data, static_cast<int>(size));
				QByteArray response(data, static_cast<int>(size));
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
						emit emitEmailList(words.at(num));

						num++;

					} // end words !empty

				} // end while



				return size;
			});// end setWriteFunction

			connect(curl, &CurlEasy::done, [curl, this](CURLcode result) {
				long httpResponseCode = curl->get<long>(CURLINFO_RESPONSE_CODE);

				QString effectiveUrl = curl->get<const char*>(CURLINFO_EFFECTIVE_URL);

				/*********HTTP CODE*******/
				// Log errors,or show message to user if response code is not 200
				if (httpResponseCode == 200 && result == CURLE_OK) {

					//qDebug() << "GOOD TO SCRAPE";
					emit senderCurlResponseInfo("Request Succeded");
					qDebug() << "HTTP  Code-->" << httpResponseCode;
					qDebug() << "Result--->" << result;
					qDebug() << "From CURL 0";

				}



				if (httpResponseCode == 503) {

					// qDebug() << "503 ERROR CODE ";
					emit senderCurlResponseInfo("503");


				}

				if (result != CURLE_OK) {

					switch (result)
					{
					case 5: qDebug() << "Curl code-> " << result << " Message->" << curl->errbuf;
						break;
					case 7: qDebug() << "Curl code-> " << result << " Message->" << curl->errbuf;
						break;
					case 35: qDebug() << "Curl code-> " << result << " Message->" << curl->errbuf;
						break;
					case 56: qDebug() << "Curl code-> " << result << " Message->" << curl->errbuf;
						break;


					default: qDebug() << "Default Switch Statement Curl Code--> " << result;

					}
					/**************
					*checks curl erros codes
					*
					* 5 -- Couldn't resolve proxy. The given proxy host could not be resolved.
					* 7 -- Failed to connect() to host or proxy.
					*
					*********/
					if (result == 5 || result == 7 || result == 35)
					{
						emit senderCurlResponseInfo("Proxy Error");
					}

				}

			}); // end of lambda


			connect(curl, SIGNAL(done(CURLcode)), curl, SLOT(deleteLater()));

		} // end of urlQueryParam  if statement
		else
		{

			qDebug() << "NOT SET";
		}


		// Once we're done waiting, value is updated
		emit valueChanged(QString::number(1));
	}

	// Set _working to false, meaning the process can't be aborted anymore.
	mutex.lock();
	_working = false;
	mutex.unlock();

	qDebug() << " First Thread Worker process finished in Thread " << thread()->currentThreadId();

	//Once 60 sec passed, the finished signal is sent
	emit finished();
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


	//qDebug() << *httpRequestList << "\n";


	// if workerCounter == incrementProxy, reset workerCounter ; if certain number of
	// http request have been made rotate proxy
	if (*workerCounterPtr <= incrementProxy)
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


	// if workerCounter is greater than incrementProxy/ amount of http request before proxy rotates
	if (*workerCounterPtr >= incrementProxy)
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
	if (isEmpty == "Empty")
	{

		isProxyEmpty = true;
	}

	if (isEmpty == "Not Empty")
	{
		isProxyEmpty = false;

	}
}

void Worker::receiverStopThreadCounters(QString stopThreadCounter)
{
	isStopStartThreadCounter = false;
	*workerCounterPtr = 0;
	*proxyServerCounterPtr = 0;
}

void Worker::receiverStartThreadCounters(QString startThreadCounter)
{

	isStopStartThreadCounter = true;
	*workerCounterPtr = 0;
	*proxyServerCounterPtr = 0;



}

void Worker::receiverHarvesterTimer(int harvesterTimer) {

	/*********
	Chrono is namespace, meaning a certain class will on be in its scope
	Within this namespace we have a class thats a template name seconds, that takes a long long type -- which is seconds
	in our case. So to create a instance of seconds class, we need to create a object, that takes a long. In our case s(harvesterTimer)
	The s object of type seconds, also has members as well
	*******/
	std::chrono::seconds s(harvesterTimer);
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(s);
	// cast long long to int
	*harvesterTimerPtr = static_cast<int>(ms.count());

}


void Worker::test(QString s) {
	emit emitEmailList(s);
	//qDebug() << s;
}

size_t Worker::curl_write(char *ptr, size_t size, size_t nmemb, void *stream)
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
			static_cast<Worker*>(stream)->test(words.at(num));
			//qDebug() << num;
			num++;

		} // end words !empty

	} // end while


	  //	struct MemoryStruct *mem = (struct MemoryStruct *)stream;

	  //increase the size of "memory" by the size of the bytes that we have read
	  //	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);


	return size*nmemb;

}

