#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QDateTime>
#include <QModelIndex>

namespace Ui {
    class Form;
}

class ShoppingDataManager;
class QSettings;

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    void startSearch();
    void refresh();
    void next();
    void back();
    bool isFacetAvailable();
    ShoppingDataManager* getManager() {return m_pManager;}

public slots:
    void showDetails();
    void showHistogram();

protected:
    void keyPressEvent(QKeyEvent* ev);

private slots:
    void onErrorOccured(const QString& error);
    void onProductsListReady();
    void on_pbSearch_clicked();
    void showProductDetails(int row, int column);

private:
    Ui::Form *ui;
    ShoppingDataManager* m_pManager;
    int m_combo_count;
    QString m_savePattern;
    QString m_saveCountry;
    bool m_bKey;    ///< flag to prevent double handling the stream of same events (only for search combobox).
    QSettings* m_pSettings;

    QDateTime convertToTime(const QString& str);
    void checkComboBoxes();
    void saveSettings();
};

#endif // FORM_H
