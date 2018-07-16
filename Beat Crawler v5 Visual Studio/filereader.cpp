#include "filereader.h"

FileReader::FileReader(QObject * parent) :
	QObject(parent)

{
	_working = false;
	_abort = false;
}

FileReader::~FileReader() {


}


void FileReader::requestWork() {

	mutex.lock();
	_working = true;
	_abort = false;
	//qDebug() << "First Thread Request worker start in Thread " << thread()->currentThreadId();
	mutex.unlock();
	emit workRequested();

}

void FileReader::abort()
{
	mutex.lock();
	if (_working) {
		_abort = true;
		//qDebug() << "First Thread Request worker aborting in Thread " << thread()->currentThreadId();
	}
	mutex.unlock();
	breakReadFileLoop = true;
}

void FileReader::doWork() {


}


void FileReader::receiverReadFile(QString fileName) {

	QByteArray result;
	QString fileLine;
	QStringList fileList;
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
	{
		qDebug() << "ERROR OPENING FILE" << file.error();
	}
	if (breakReadFileLoop == true) {
		//break;
	}

	if (breakReadFileLoop == false) {
		//continue;
	}
	while (!file.atEnd())
	{



		// Checks if the process should be aborted
		mutex.lock();
		bool abort = _abort;
		mutex.unlock();

		fileLine.append(file.readLine());
		//filteredString2 = filteredString1.remove(QRegExp(QString::fromUtf8("[^A-Za-z0-9 ]")));
		fileList = fileLine.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
		for (int i = 0; i <fileList.size(); i++)
		{
			//qDebug() << fileList.at(i);
			// access toSet() because our qstringlist inherits QList, and  we can call on QSet to remove dups, and
			// then turn that qset to a qlist. From there we can access its indexes -> .at(i)
			//fileList.toSet().toList();

			//send results   fileList.at(i)  to main gui and insert into keywordlist hash
			QEventLoop loop;
			QTimer::singleShot(2000, &loop, SLOT(quit()));
			loop.exec();
			emit emitSenderFileReadKeywordList(fileName, fileList.at(i), fileList.size());


			//if(fileList.at(i) == fileList.size())
			//{
			//emit emitSenderFileReadKeywordList(file.fileName(), fileList.at(i));
			//}
		}
		file.close();

	}
	// Set _working to false, meaning the process can't be aborted anymore.
	mutex.lock();
	_working = false;
	mutex.unlock();

}

void FileReader::receiverStopFileReaderThread(bool b) {


}
