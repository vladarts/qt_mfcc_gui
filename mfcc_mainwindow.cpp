#include <QtGui/QtGui>
#include "mfcc_mainwindow.h"


MFCC_MainWindow::MFCC_MainWindow( QWidget *parent) : QDialog(parent)
{
	inFile = NULL;
	mfccCounter = NULL;

	// Creating
	openButton = new QPushButton(tr("Open File"));
	closeButton = new QPushButton(tr("Close File"));
	startAnalyseButton = new QPushButton(tr("Play"));
	stopAnalyseButton = new QPushButton(tr("Pause"));
	wavInfoText = new QTextBrowser;
	mfccText = new QTextBrowser;
	updateTimer = new QTimer(this);
	plot = new QCustomPlot();
	countOfMfccSpin = new QSpinBox();
	countLabel = new QLabel(tr("Count of MFCC"));
	mfccTextLabel = new QLabel(tr("MFCC`s:"));
	timerLabel = new QLabel(tr("Time of counting: "));
	filePosLabel = new QLabel(tr("Samples counted: "));

	// Configuring
	closeButton->setEnabled(false);
	startAnalyseButton->setEnabled(false);
	stopAnalyseButton->setEnabled(false);
	countOfMfccSpin->setMinimum(10);
	countOfMfccSpin->setMaximum(30);
	countOfMfccSpin->setValue(16);
	countOfMfccSpin->setEnabled(false);
	mfccText->setFixedWidth(90);
	mfccText->setMinimumHeight(400);
	wavInfoText->setFixedWidth(200);
	// 
	QVBoxLayout *leftLayout = new QVBoxLayout;
	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
	QHBoxLayout *rightPlayLayout = new QHBoxLayout;
	QHBoxLayout *countLayout = new QHBoxLayout;
	QVBoxLayout *centralLayout = new QVBoxLayout;

	leftLayout->addWidget(mfccTextLabel);
	leftLayout->addWidget(mfccText);

	centralLayout->addWidget(plot);
	centralLayout->addWidget(timerLabel);
	centralLayout->addWidget(filePosLabel);

	countLayout->addWidget(countOfMfccSpin);
	countLayout->addWidget(countLabel);

	rightPlayLayout->addWidget(startAnalyseButton);
	rightPlayLayout->addWidget(stopAnalyseButton);

	rightLayout->addWidget(openButton);
	rightLayout->addWidget(closeButton);
	rightLayout->addWidget(wavInfoText);
	rightLayout->addLayout(countLayout);
	rightLayout->addLayout(rightPlayLayout);

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(centralLayout);
	mainLayout->addLayout(rightLayout);

	setLayout(mainLayout);

	connect(openButton, &QPushButton::clicked, this, &MFCC_MainWindow::openFile);
	connect(closeButton, &QPushButton::clicked, this, &MFCC_MainWindow::closeFile);
	connect(startAnalyseButton, &QPushButton::clicked, this, &MFCC_MainWindow::startAnalyse);
	connect(stopAnalyseButton, &QPushButton::clicked, this, &MFCC_MainWindow::stopAnalyse);
	connect(updateTimer, &QTimer::timeout, this, &MFCC_MainWindow::analyse);
	connect(countOfMfccSpin, SIGNAL(valueChanged(int)), this, SLOT(changePlotX(int)));

	// Plot config
	plot->setMinimumSize(600,400);
	plot->addGraph();
	plot->graph(0)->setLineStyle(QCPGraph::lsLine);
	plot->xAxis->setLabel("Number");
	plot->xAxis->setTickStep(1);
	plot->yAxis->setLabel("MFCC");
	plot->replot();

	plot->addGraph();
	plot->graph(1)->setLineStyle(QCPGraph::lsLine);
	plot->graph(1)->setPen(QPen(Qt::red));
	plot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));
}

void MFCC_MainWindow::openFile()
{
	InitFile();
}

void MFCC_MainWindow::closeFile()
{
	stopAnalyse();
	CloseFile();
}

MFCC_MainWindow::~MFCC_MainWindow()
{

}

void MFCC_MainWindow::startAnalyse()
{
	updateTimer->start();
	startAnalyseButton->setEnabled(false);
	stopAnalyseButton->setEnabled(true);
}

void MFCC_MainWindow::stopAnalyse()
{
	updateTimer->stop();
	startAnalyseButton->setEnabled(true);
	stopAnalyseButton->setEnabled(false);
}

void MFCC_MainWindow::analyse()
{
	if (GetMFCC())
	{
		mfccText->clear();
		mfccText->clearHistory();
		for (int i = 0; i < countOfMFCC; i++)
		{
			mfccText->append(QString::number(mfccCounter->out[i]));
		}
		Replot();
	}
	else
	{
		mfccText->append("Reached End Of File");
		stopAnalyse();
	}
}

bool MFCC_MainWindow::GetMFCC()
{
	if (!inFile->EndOfWAV())
	{
		timer = new QElapsedTimer;
		timer->start();
		inFile->GetSamples(countForMFCC,buffer);
		mfccCounter->vmfcc_updateBuffer(buffer);
		mfccCounter->vmfcc_execute();
		filePos += countForMFCC;
		timerLabel->setText("Time of counting: " + QString::number(timer->elapsed()) + "ms" );
		filePosLabel->setText("Samples counted: " + QString::number(filePos) + "/" + QString::number(inFile->GetCountOfSamples()) + " (" + QString::number(filePos * 100/inFile->GetCountOfSamples()) + "%)");
		delete timer;
		return true;
	}
	else
	{
		stopAnalyse();
		return false;
	}
}

bool MFCC_MainWindow::CloseFile()
{
	startAnalyseButton->setEnabled(false);
	stopAnalyseButton->setEnabled(false);
	inFile->Terminate();
	inFile = NULL;
	openButton->setEnabled(true);
	closeButton->setEnabled(false);
	UpdateInfo();
	return true;
}

bool MFCC_MainWindow::InitFile()
{	
	if (inFile != NULL)
		return false;
	inFile = new vWAV;
	pathToFile = QFileDialog::getOpenFileName(this, tr("Open WAV"), tr("WAV files (*.wav)"));
	if (pathToFile.isEmpty())
	{
		return false;
	}
	if (inFile->Open(pathToFile.toLocal8Bit().data()))
	{
		openButton->setEnabled(false);
		closeButton->setEnabled(true);
		startAnalyseButton->setEnabled(true);
		countOfMfccSpin->setEnabled(true);
		countOfMFCC = countOfMfccSpin->value();
		countForMFCC = VMFCC::mfcc_GetOptimalCountIn(inFile->GetSampleRate());
		UpdateInfo();
		buffer = new long[countForMFCC];
		mfccCounter = new VMFCC;
		mfccCounter->vmfcc_init(countForMFCC, buffer, countOfMFCC, inFile->GetSampleRate());
		updateTimer->setInterval(1000 * countForMFCC / inFile->GetSampleRate());
		filePos = 0;
		// plot init
		maxY = 5;
		minY = -5;
		plotDataX.clear();
		plotDataY.clear();
		iteration = 0;
		for (int i = 1; i < countOfMFCC + 1; i++)
		{
			plotDataX.append(i);
			plotDataYs.append(0);
		}

		plot->xAxis->setRange(1,countOfMFCC);
		plot->yAxis->setRange(minY,maxY);
		//
		return true;
	}
	CloseFile();
	return false;
}

void MFCC_MainWindow::UpdateInfo()
{
	if (inFile != NULL)
	{
		wavInfoText->clearHistory();
		wavInfoText->append("Sample Rate: ");
		wavInfoText->append(QString::number(inFile->GetSampleRate()));
		wavInfoText->append("Bits per sample: ");
		wavInfoText->append(QString::number(inFile->GetBitPerSample()));
		wavInfoText->append("Number of channels: ");
		wavInfoText->append(QString::number(inFile->GetNumberOfChannels()));
		wavInfoText->append("Count of samples: ");
		wavInfoText->append(QString::number(inFile->GetCountOfSamples()));
		wavInfoText->append("Samples per channel: ");
		wavInfoText->append(QString::number(inFile->GetCountOfSamples()/inFile->GetNumberOfChannels()));
	}
	else
	{
		wavInfoText->clearHistory();
		wavInfoText->clear();
		mfccText->clearHistory();
		mfccText->clear();
	}
}

void MFCC_MainWindow::Replot()
{
	plotDataY.clear();
	for (int i = 0; i < countOfMFCC; i++)
	{
		if (mfccCounter->out[i] > MAX_MFCC)
			plotDataY.append(maxY);
		else
		{
			plotDataY.append(mfccCounter->out[i]);
			if (mfccCounter->out[i] > maxY)
			{
				maxY = mfccCounter->out[i];
				plot->yAxis->setRange(minY,maxY);
			}
			if (mfccCounter->out[i] < minY)
			{
				minY = mfccCounter->out[i];
				plot->yAxis->setRange(minY,maxY);
			}
		}
	}
	iteration++;
	for (int i = 0; i < countOfMFCC; i++)
		plotDataYs[i] = ( plotDataYs[i] / iteration ) * (iteration - 1) + plotDataY[i] / iteration;
	plot->graph(0)->setData(plotDataX,plotDataY);
	plot->graph(1)->setData(plotDataX, plotDataYs);
	plot->replot();
}

void MFCC_MainWindow::changePlotX( int newCount )
{
	plotDataX.clear();
	for (int i = 1; i < countOfMFCC + 1; i++)
		plotDataX.append(i);
	countOfMFCC = newCount;
	plot->xAxis->setRange(1,countOfMFCC);
	mfccCounter->mfcc_SetCountOfMFCC(countOfMFCC);
}
