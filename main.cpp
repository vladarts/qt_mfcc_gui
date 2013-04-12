#include "qt_vmfcc.h"



#include <QApplication>
#include <QDialog>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QString>
//#include <QQSound>
//#include <QQSoundEffect>

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
