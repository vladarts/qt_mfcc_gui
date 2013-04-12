#ifndef MFCC_MAINWINDOW_H
#define MFCC_MAINWINDOW_H

// Base QT-classes

#include <QString>
#include <QWidget>
#include <QFileDialog>
#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QSpinBox>
#include <QLabel>
#include <QVector>

// My classes
#include "vmfcc.h"
#include "vWAV.h"

// Plotting widget
#include "qcustomplot.h"

// Time count
#include <QElapsedTimer>

#define NUMBER_OF_MFCC 16
#define NUMBER_FOR_MFCC 8192
#define MAX_MFCC 50

class MFCC_MainWindow : public QDialog
{
	Q_OBJECT
public:
	//// ...
	MFCC_MainWindow(QWidget *parent = 0);
	~MFCC_MainWindow();
	
	
public slots:
	void openFile();
	void closeFile();
	void startAnalyse();
	void stopAnalyse();
	void analyse();
	void changePlotX(int);

private:
	// Procedures
	void UpdateInfo();
	void Replot();

	bool InitFile();
	bool CloseFile();

	bool GetMFCC();
	// Data elements
	vWAV *inFile;
	VMFCC *mfccCounter;
	QString pathToFile;
	long *buffer;
	int countOfMFCC;
	int countForMFCC;

	QVector<double> plotDataX;
	QVector<double> plotDataY;
	QVector<double> plotDataYs;

	int iteration;

	double maxY, minY;

	long int filePos;

//	QBuffer *audioBuffer;

	QElapsedTimer *timer;
	

	// Visual elements
	QPushButton *openButton;
	QPushButton *closeButton;
	QPushButton *startAnalyseButton;
	QPushButton *stopAnalyseButton;
	QTextBrowser *wavInfoText;
	QTextBrowser *mfccText;
	QTimer *updateTimer;
	QCustomPlot *plot;
	QSpinBox *countOfMfccSpin;
	QLabel *countLabel;
	QLabel *mfccTextLabel;
	QLabel *timerLabel;
	QLabel *filePosLabel;
	
};

#endif // MFCC_MAINWINDOW_H
