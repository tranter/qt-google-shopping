#include <QDebug>
#include "productdetaildlg.h"
#include "ui_productdetaildlg.h"

ProductDetailDlg::ProductDetailDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductDetailDlg)
{
    ui->setupUi(this);

}

ProductDetailDlg::~ProductDetailDlg()
{
    delete ui;
}
