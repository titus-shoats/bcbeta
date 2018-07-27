#pragma once
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <QRunnable>
#include <QThread>


class FileReader : public QObject, public QRunnable{
	Q_OBJECT

public:
	FileReader(QString fileName);
	~FileReader();
	void run();

public slots:

       void receiverReadFile(QString filename);

private:
	QString fileName;
	QStringList *keywordList;

signals:

	void senderKeywordListToTWorker(QStringList * keywordList_);
};
