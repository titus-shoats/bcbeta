#include "filereader.h"

FileReader::FileReader(QString fileName) :
	fileName(fileName)

{
	keywordList = new QStringList();
}
FileReader::~FileReader()
{
	delete keywordList;

}


void FileReader::run() {

	if (!keywordList->isEmpty()) 
	{
		// reset list
		//keywordList->clear();
	}
	QFile file(fileName);
	QFileInfo fi(file.fileName());
	QString fileExt = fi.completeSuffix();;
	QString strings;
	QString str;

	if (!file.open(QFile::ReadOnly))
	{
		qDebug() << "ERROR OPENING FILE READER KEYWORD FILE" << file.error();
	}


	QTextStream ts(&file);
	while (!ts.atEnd())
	{
		str = ts.readLine();
		QThread::currentThread()->msleep(5000);
		*keywordList << str;
	}
	emit senderKeywordListToTWorker(keywordList);
	file.close();
}





void FileReader::receiverReadFile(QString fileName)
{
	

}

