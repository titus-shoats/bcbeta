#ifndef BEATCRAWLER_H
#define BEATCRAWLER_H

#include <QMainWindow>
#include "options.h"
#include <QStandardItemModel>
#include <curl.h>
#include <QEvent>
#include <cstdio>
#include <string>
#include <QTimer>
#include <QVector>
#include <QString>
#include "worker.h"
#include  <QUrl>
#include <QThread>
#include <QItemSelection>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include "filereader.h"
#include <QtConcurrent/QtConcurrent>



namespace Ui {
	class BeatCrawler;
}

typedef Options* OptionsPtr;


class BeatCrawler : public QMainWindow
{
	Q_OBJECT

public:
	explicit BeatCrawler(QWidget *parent = 0);
	~BeatCrawler();
	void setProxyTable();
	void setSearchResults();
	void getKeywordsSearchBoxOrList();
	void keywordSearchBoxProcess();
	void keywordListOptionProcess();
	void setEmailTable();
	QString toDebug(const QByteArray & line);
	void receiverParameters();
	void keywordsQueueTable();



	private slots:


	void on_pushButton_Start_clicked(bool checked);
	void on_pushButton_Load_Keyword_List_clicked();
	bool eventFilter(QObject *watched, QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void on_pushButton_Add_Proxy_clicked();
	void on_checkBox_Delete_Keywords_clicked();

	void on_pushButton_Next_Email_Pagination_clicked();

	void on_pushButton_Previous_Email_Pagination_clicked();

	void on_checkBox_Delete_Emails_clicked();

	void on_pushButton_Save_Emails_clicked();



	public slots:

	void receiverEmailList(QString);
	//void receiverParameters();
	void recieverProxyTableSelection(const QItemSelection &, const QItemSelection &);
	void recieverCurlResponseInfo(QString info);
	void deleteKeyordsListTable();
	void deleteEmailsListTable();
	void receiverFileReadKeywordList(QString fileName, QString data, int keywordSize);
	void disableStartButtonLoadKeywordList();
	void enableStartButtonLoadKeywordList();
	void receiverFinishHarvestResults(QString s);
	void recieverFinishReadingKeywordFile();
	void receiverEmailList2(QString list);
	void populateEmailTable();
	void receiverEnableDeleteKeywordCheckBox();
	void receiverEnableDeleteEmailCheckBox();
	void reEnableStartButton();
	void receiverCurrentKeyword(QString keyword);


signals:
	void postParam(QString, QString, QList <QString> *);
	void emitParameters();
	void emitsenderEmptyProxyServer(QString);
	void emitsenderStopThreadCounters(QString);
	void emitsenderStartThreadCounters(QString);
	void emitSenderAppOptions(int, int);
	void emitSenderReadFile(QString);
	void senderStopFileReaderThread();
	void on_stop();
	void emitRemoveThreadFileList();
	void emitRemoveThreadEmailList();

private:
	Ui::BeatCrawler *ui;
	bool *startScrape;
	bool stopScrape;
	bool changeBool;
	const int numOptions = 67;
	// Options pointer
	OptionsPtr *options;
	// Options objects
	Options *opt;
	static QStringList * emails;
	QTimer * timer;
	QTimer *keywordsQueueTableTimer;
	int *emailOptionsNumPtr;
	int *searchEngineNumPtr;
	int *searchEngineNumPtr1[3];
	int *socialNetWorkNumPtr;
	int *keywordLoadListOptionsNumPtr;

	int *addProxyCounterPtr;
	int addProxyCounterNum;

	int *keywordBoxNumPtrCounter;
	int keywordBoxNumPtrNum;

	int *keywordListSearchEngineCounterPtr;
	int keywordListSearchEngineCounterNum;

	int *keywordSearchBoxSearchEngineCounterPtr;
	int keywordSearchBoxSearchEngineCounterNum;

	int keywordListNumPtrNum;
	int * keywordListNumPtrCounter;

	bool isSearchEngineSelectOne;
	bool  isSocialNetworkSelectOne;
	bool isEmailSelectOne;
	bool isKeywordsSelect;

	bool  isProxyPortValid;
	bool  isProxyHostValid;

	bool isMultiAndKeywordBoxSelected;

	QList <QString> *emailList;
	// sets QSet back to list
	// this is because were setting a qlist to qset to remove dups, then from a qset to qlist again
	QList<QString> setEmailList;
	QList <QString> *proxyServers;
	QList<QVector <QString>>vectorSearchOptions;
	QList<int>timerOptions;
	QList<QString>otherOptions;

	QThread *thread;
	QThread *thread1;
	Worker * worker;
	Worker threadWorker;

	int emailOptionsNum;
	int searchEngineNum;
	int socialNetWorkNum;
	int keywordLoadListOptionsNum;
	bool * searchEngineNumPtrAdded;
	bool searchEngineNumPtrAddedBool;
	int *searchEngineNumPtrCounter;
	int  searchEngineNumPtrNum;
	QVector<QString>vectorSocialNetworks2;
	QVector<QString>vectorEmailOptions;
	QVector<QString>vectorSearchEngineOptions;
	QString *currentKeywordPtr;
	QString currentKeyword;
	QVector<QString>vector;
	QString *data;

	QUrl url;
	QString testLabel;
	// gets callback data from curl
	static QString * fetchWriteCallBackCurlData;
	QString fetchWriteCallBackCurlDataString;
	int * nextEmailPaginationPtr;
	int * previousEmailPaginationPtr;
	int nextEmailPagination;
	int previousEmailPagination;
	bool clickedStartStopButton;
	QTableWidgetItem *emailListItem;
	QTableWidgetItem *keywordQueueItem;
	QTableWidgetItem *addProxyItem;
	FileReader *fileReader;
	QStringList FileReadKeywordList;
	QVector<QString> keywordListLastItem;
	QStringList *fileList;
	int *fileListPtr;
	int fileListNum;
	bool keywordCompleted;
	QStandardItemModel *emailTableModel;
	int *previousPagesPtr;
	int previousPagesNum;
	QString *currentKeyword_;
	QString currentKeywordString_;

};

#endif // BEATCRAWLER_H
