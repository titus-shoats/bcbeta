#pragma once
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QMutex>
#include <QThread>


class FileReader : public QObject {
	Q_OBJECT

public:
	FileReader(QObject * parent = Q_NULLPTR);
	~FileReader();
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

	bool breakReadFileLoop;


signals:
	void emitSenderFileReadKeywordList(QString, QString, int);

	/**
	* @brief This signal is emitted when the Worker request to Work
	* @sa requestWork()
	*/
	void workRequested();
	/**
	* @brief This signal is emitted when counted value is changed (every sec)
	*/
	void valueChanged(const QString &value);

	/**
	* @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
	*/
	void finished();

	public slots:
	void receiverReadFile(QString fileName);
	void receiverStopFileReaderThread(bool b);
	/**
	* @brief Does something
	*
	* Counts 60 sec in this example.
	* Counting is interrupted if #_aborted is set to true.
	*/
	void doWork();

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

};
