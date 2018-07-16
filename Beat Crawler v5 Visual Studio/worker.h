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

#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include "curleasy.h"



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



	static void init(CURLM *cm, int i);
	void test(QString s);
	static void requestParsedEmailList(QString);
	static size_t curl_write(char *ptr, size_t size, size_t nmemb, void *stream);





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
	QList <QString> *paramsPtr;
	QString params;
	QString *urlQueryParam;
	QString *userAgentParam;
	QString *proxies;
	int *workerCounterPtr;
	int workerCounterNum;

	int *proxyServerCounterPtr;
	int proxyServerCounterNum;

	// increment proxy at cetain number of http request
	const int incrementProxy = 5;

	bool isProxyEmpty;
	bool canProxyCounterIncrement;
	bool isStopStartThreadCounter;
	int * harvesterTimerPtr;
	int  harvesterTimerNum;
	//static QStringList *parsedEmailList;
	//static const Worker *theWorker = new Worker();
	QList<QStringList>*httpRequestList;


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
	void emitEmailList(QString s);
	void emitKeywordQueue();
	void senderCurlResponseInfo(QString);



	public slots:
	/**
	* @brief Does something
	*
	* Counts 60 sec in this example.
	* Counting is interrupted if #_aborted is set to true.
	*/
	void doWork();
	//void doneWithParameters(QString * doneParam );
	//void receiverDoneWithParameters(QString * receiverParam);
	void getParam(QString url, QString userAgent, QList <QString> *proxyServers);

	void getProxyFile(QString fileName);

	void receiverEmptyProxyServer(QString isEmpty);

	void receiverStopThreadCounters(QString stopThreadCounter);
	void receiverStartThreadCounters(QString startThreadCounter);

	void receiverHarvesterTimer(int harvesterTimer);



};

#endif // WORKER_H
