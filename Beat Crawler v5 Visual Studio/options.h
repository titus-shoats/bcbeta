#pragma once
#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QVector>
#include <QByteArray>


class Options {

public:
	// valid options "google","bing","yahoo"
	QString searchEngineOptions[3];
	//{"gmail","hotmail","outlook"};
	QString emailOptions[3];
	//valid options {"instagram","facebook","soundcloud"};
	QString socialNetworkOptions[5];
	//valid options , keywords that user inserts;
	QString keywordSearchBoxOptions[1];
	//valid options , userAgent will hold the current user agent string
	QString userAgentsOptions[1];
	//valid options , load list of keywords into array;
	QHash<QString, int> keywordLoadListOptions;
	// QHash<QByteArray, int> keywordLoadListOptions;
	//valid options , load completed list/tagged of keywords into array;
	QHash<QString, int> keywordTaggeddListOptions;




};

#endif // OPTIONS_H
