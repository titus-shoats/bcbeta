#include "beatcrawler.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BeatCrawler w;
	w.show();
	return a.exec();
}
