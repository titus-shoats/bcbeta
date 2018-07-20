#include "thread.h"

Thread::Thread(QObject * parent) : QObject(parent) {
	mStop = false;
}

Thread::~Thread() {
	
}


void Thread::start(QString name)
{
	mStop = false;
	for(int i =0; i < 5000; i++)
	{ 
		if (mStop) return;
		qDebug() << "From thread " << i;
		emit onNumber(name, i);
		QThread::currentThread()->msleep(100);
	}

}

void Thread::stop()
{

	mStop = true;
}
