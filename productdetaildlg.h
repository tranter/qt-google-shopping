#ifndef PRODUCTDETAILDLG_H
#define PRODUCTDETAILDLG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
    class ProductDetailDlg;
}

class ProductDetailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailDlg(QWidget *parent = 0);
    ~ProductDetailDlg();

    Ui::ProductDetailDlg* data() {return ui;}

private:
    Ui::ProductDetailDlg *ui;
};

#endif // PRODUCTDETAILDLG_H
