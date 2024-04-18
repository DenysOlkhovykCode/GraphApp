#include "mainwindow.h"
#include "course.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Course w;
	w.show();
	return a.exec();
}
