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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <cstdio>
#include <string>
#include <iostream>
#include <QVector>
#include <curl.h>
#include <stdio.h>
#include <stdlib.h>

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
#include <QFileInfo>
#include <QtConcurrent/QtConcurrent>



struct MemoryStruct {
	char *memory;
	size_t size;
};


class Worker : public QObject
{
	Q_OBJECT

public:
	explicit Worker(QObject *parent = 0);
	~Worker();
	/**
	* @brief Requests the process to start
	*
	* It is thread safe as it uses #mutex to protect access to #_working variable.
	*/
	void requestWork();
	/**
	* @brief Requests the process to abort
	*
	* It is thread safe as it uses #mutex to protect access to #_abort variable.
	*/
	void abort();



	//	static void init(CURLM *cm, int i);
	void processedEmails(QString emails);
	void processedEmails2(QString emails);
	static void requestParsedEmailList(QString);
	static size_t curl_write1(char *ptr, size_t size, size_t nmemb, void *stream);
	static size_t curl_write2(char *ptr, size_t size, size_t nmemb, void *stream);
	static size_t curl_write3(char *ptr, size_t size, size_t nmemb, void *stream);
	static size_t curl_write4(char *ptr, size_t size, size_t nmemb, void *stream);
	static size_t curl_write5(char *ptr, size_t size, size_t nmemb, void *stream);

	void curlProcess1(const char *urls[], QString threadName);
	void curlProcess2(QString url, QString threadName);
	void curlProcess3(QString url, QString threadName);
	void curlProcess4(QString url, QString threadName);
	void curlProcess5(QString url, QString threadName);

	void curlParams(QList<QVector <QString>>vectorSearchOptions, QString lineEdit_keywords_search_box,
		QList <QString> *proxyServers, QList<int>timerOptions, QList<QString>otherOptions);
	void doWork1();

	void receiverReadFile(QString fileName);
	void test(QString name);
	void parsedEmails();





private:
	/**
	* @brief Process is aborted when @em true
	*/
	bool _abort;
	/**
	* @brief @em true when Worker is doing work
	*/
	bool _working;
	/**
	* @brief Protects access to #_abort
	*/
	QMutex mutex;
	QString *proxies;
	int *workerCounterPtr;
	int workerCounterNum;

	int *proxyServerCounterPtr;
	int proxyServerCounterNum;

	// increment proxy at cetain number of http request
	int incrementProxy = 5;

	bool isProxyEmpty;
	bool canProxyCounterIncrement;
	bool isStopStartThreadCounter;
	int * harvesterTimerPtr;
	int  harvesterTimerNum;

	int * proxyRotateIntervalPtr;
	int  proxyRotateIntervalNum;
	//static QStringList *parsedEmailList;
	//static const Worker *theWorker = new Worker();
	QList<QStringList>*httpRequestList;
	bool wStop;

	QVector<QString>vectorSearchEngineOptions;
	QVector<QString>vectorEmailOptions;
	QVector<QString>vectorSocialNetworks2;
	QList <QString> *proxyServers;

	QStringList *fileList;
	int *fileListPtr;
	int fileListNum;
	QString *currentKeywordPtr;
	QString currentKeyword;

	int *curlSingleProcessPtrCounter;
	int curlSingleProcessCounterNum;

	int *curlMultiProcessPtrCounter;
	int curlMultiProcessCounterNum;


	int *searchEngineNumPtr;
	int searchEngineNum;

	int *emailOptionsNumPtr;
	int emailOptionsNum;

	int *socialNetWorkNumPtr;
	int socialNetWorkNum;

	int *searchEnginePaginationCounterPtr;
	int searchEnginePaginationCounterNum;


	char errbuf[CURL_ERROR_SIZE];
	QList<QString>curlHTTPRequestList;
	int *curlHTTPRequestCounterPtr;
	int curlHTTPRequestCounterNum;
	QTimer *deleteKeywordCheckBoxTimer;
	QString *multiURLOption;
	QString multiURLOptionString;
	QString *multiOptionOneURL;
	QString multiOptionOneURLString;
	QStringList logHarvesterStatus;


signals:
	/**
	* @brief This signal is emitted when the Worker request to Work
	* @sa requestWork()
	*/
	void workRequested();
	/**
	* @brief This signal is emitted when counted value is changed (every sec)
	*/
	void valueChanged(const QString &value);

	void workRequested1();


	/**
	* @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
	*/
	void finished();

	void emitParameters();
	void emitEmailList1(QString emails);

	void emitKeywordQueue();
	void senderCurlResponseInfo(QString);
	void emitDataTest(QString s);
	void emitFinishSenderHarvestResults(QString results);
	void emitfinishReadingKeywordFile();
	void emitsenderEnableDeleteKeywordCheckBox();
	void emitsenderEnableDeleteEmailCheckBox();
	void emitsendCurrentKeyword(QString keyword);
	void emitSenderLogHarvesterStatus(QString logStatus);
	public slots:
	/**
	* @brief Does something
	*
	* Counts 60 sec in this example.
	* Counting is interrupted if #_aborted is set to true.
	*/
	//void doWork();
	//void doneWithParameters(QString * doneParam );
	//void receiverDoneWithParameters(QString * receiverParam);

	void getProxyFile(QString fileName);

	void receiverEmptyProxyServer(QString isEmpty);

	void receiverStopThreadCounters(QString stopThreadCounter);
	void receiverStartThreadCounters(QString startThreadCounter);

	void receiverAppOptions(int harvesterTimer, int proxyRotateInterval);
	void stop();
	void receiverRemoveThreadFileList();
	void readEmailFile();
	void receiverRemoveThreadEmailList();
	//void deleteKeywordCheckBox();


};

#endif // WORKER_H
