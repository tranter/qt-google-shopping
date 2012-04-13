#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "shopping_data_manager.h"

namespace Ui {
    class MainWindow;
}

class Form;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onLoadList();
    void onShowDetails();
    void onRefresh();
    void onNext();
    void onBack();
    void onHistogram();
    void onProductsListReady();

private:
    Ui::MainWindow *ui;
    Form* m_pForm;
};

#endif // MAINWINDOW_H
