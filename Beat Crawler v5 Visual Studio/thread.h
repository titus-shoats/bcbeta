#pragma once
#ifndef THREAD_H
#define THREAD_H
#include <QObject>
#include<QThread>
#include <QDebug>


class Thread : public QObject {
	Q_OBJECT

public:
	Thread(QObject * parent = Q_NULLPTR);
	~Thread();
	void start(QString name);

private:
	bool mStop;

signals:
	   void onNumber(QString name, int number);

public slots:
       void stop();


	
};

#endif // THREAD_H