#include "qt_vmfcc.h"



#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QHBoxLayout>
#include <QString>
#include <QtMultimedia/QSound>
#include <QtMultimedia/QSoundEffect>

#include "vWAV.h"
#include "vmfcc.h"

#include <fstream>
#include <iostream>

#include "mfcc_mainwindow.h"


// test

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	
	
	MFCC_MainWindow MainWindow;
	MainWindow.show();
	
	return a.exec();
}
