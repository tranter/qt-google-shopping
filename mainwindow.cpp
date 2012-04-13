#include <QDebug>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionLoad_list, SIGNAL(triggered()), this, SLOT(onLoadList()));
    connect(ui->actionShow_details, SIGNAL(triggered()), this, SLOT(onShowDetails()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(onRefresh()));
    connect(ui->actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
    connect(ui->actionBack, SIGNAL(triggered()), this, SLOT(onBack()));
    connect(ui->actionHistogram, SIGNAL(triggered()), this, SLOT(onHistogram()));

    m_pForm = new Form(this);
    setCentralWidget(m_pForm);
    ui->actionHistogram->setEnabled(false);
    connect((QObject*)(m_pForm->getManager()), SIGNAL(sigProductsListReady()),this,SLOT(onProductsListReady()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoadList()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    m_pForm->startSearch();
}

void MainWindow::onShowDetails()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    if(m_pForm->getManager()->isKey()) m_pForm->showDetails();
}

void MainWindow::onRefresh()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    if(m_pForm->getManager()->isKey()) m_pForm->refresh();
}

void MainWindow::onNext()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    if(m_pForm->getManager()->isKey()) m_pForm->next();
}

void MainWindow::onBack()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    if(m_pForm->getManager()->isKey()) m_pForm->back();
}
void MainWindow::onHistogram()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    m_pForm->showHistogram();
}
void MainWindow::onProductsListReady()
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__;
    if (m_pForm->isFacetAvailable()) {
        ui->actionHistogram->setEnabled(true);
    } else {
        ui->actionHistogram->setEnabled(false);

    }
}
