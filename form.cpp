#include <QDebug>
#include <QDateTime>
#include <QComboBox>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSettings>

#include "form.h"
#include "ui_form.h"
#include "ui_productdetaildlg.h"
#include "productdetaildlg.h"
#include "shopping_data_manager.h"
#include "facetplot.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    m_combo_count = 10;
    ui->setupUi(this);
    m_pManager = new ShoppingDataManager();
    m_bKey = false;

    ui->twProducts->setRowCount(0);
    ui->twProducts->setColumnWidth(0,370);
    ui->twProducts->setColumnWidth(2,100);
    ui->twProducts->setColumnWidth(3,100);

    connect(m_pManager, SIGNAL(sigErrorOccured(QString)),this,SLOT(onErrorOccured(QString)));
    connect(m_pManager, SIGNAL(sigProductsListReady()),this,SLOT(onProductsListReady()));

    connect(ui->detailsButton, SIGNAL(clicked()), this, SLOT(showDetails()));
    connect(ui->twProducts,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(showProductDetails(int,int)));

    m_pSettings = new QSettings("ICS", "Google API Shopping Client");
    QVariant patterns = m_pSettings->value("list_patterns");
    ui->comboBoxSearch->addItems(patterns.toStringList());
    QVariant countries = m_pSettings->value("list_countries");
    ui->cbCountry->addItems(countries.toStringList());

    ui->comboBoxSearch->clearEditText();
}

Form::~Form()
{
    saveSettings();
    delete m_pSettings;
    delete m_pManager;
    delete ui;
}

void Form::startSearch()
{
    QString s = ui->comboBoxSearch->currentText();
    if (s.isEmpty()) {
        if (m_bKey) {  //Already done, nothing to do now.
            m_bKey = false;
        } else {
            QMessageBox(QMessageBox::Information,"Info","No valid search string available. Please define it.",QMessageBox::Ok,this).exec();
            m_bKey = true;
        }
        return;
    }
    m_pManager->startSearch(s,ui->cbCountry->currentText());
}
/*! \brief Handler for signal sigErrorOccured
 *
 */
void Form::onErrorOccured(const QString& error)
{
    QMessageBox::warning(this, tr("Error occured"), error);
}
/*! \brief Handler for signal sigProductsListReady
 *
 */
void Form::onProductsListReady()
{
    m_bKey = false; //reset flag
    QVariantList& products = m_pManager->getProducts();
    int len = products.size();
    QStringList listTitles;
    QStringList listBrands;
    QStringList listPrices;
    QStringList listCurrencies;
    QString sTitle, sBrand, sPrice, sCurrency;
    QTableWidgetItem* itemTitle;
    QTableWidgetItem* itemBrand;
    QTableWidgetItem* itemPrice;
    QTableWidgetItem* itemCurrency;

    ui->twProducts->setRowCount(len);
    for (int i=0; i<len; i++) {
        sTitle = products[i].toMap()["product"].toMap()["title"].toString();
        sBrand = products[i].toMap()["product"].toMap()["brand"].toString();
        sPrice = products[i].toMap()["product"].toMap()["inventories"].toList()[0].toMap()["price"].toString();
        sCurrency = products[i].toMap()["product"].toMap()["inventories"].toList()[0].toMap()["currency"].toString();
        listTitles << sTitle;
        listBrands << sBrand;
        listPrices << sPrice;
        listCurrencies << sCurrency;
    }
    if (listTitles.size() > 0) {
        checkComboBoxes();
        ui->twProducts->setRowCount(len);

        for (int i=0; i<len; i++) {
            itemTitle = new QTableWidgetItem(listTitles[i]);
            ui->twProducts->setItem(i,0,itemTitle);
            itemBrand = new QTableWidgetItem(listBrands[i]);
            ui->twProducts->setItem(i,1,itemBrand);
            itemPrice = new QTableWidgetItem(listPrices[i]);
            ui->twProducts->setItem(i,2,itemPrice);
            itemCurrency = new QTableWidgetItem(listCurrencies[i]);
            ui->twProducts->setItem(i,3,itemCurrency);
        }
        ui->twProducts->setCurrentCell(0,0);
    }
    ui->twProducts->setFocus();
}
/*! \brief Handler for Enter key event
 *
 */
void Form::keyPressEvent(QKeyEvent* ev)
{
    if (ev->key() == Qt::Key_Return)
    {
        QWidget* active = QApplication::focusWidget();
        if (active == (QWidget*)ui->comboBoxSearch || active == (QWidget*)ui->cbCountry)
        {
            startSearch();
        }
        else if (active == (QWidget*)ui->twProducts)
        {
            showDetails();
        }
    }
    QWidget::keyPressEvent(ev);
}
/*! \brief Show lookup data for particular product
 *
 */
void Form::showDetails()
{
    int pos = ui->twProducts->currentRow();
    QVariantList products = m_pManager->getProducts();

    if ((pos < 0) || (pos > products.size())) {
        return;
    }

    ProductDetailDlg* pDlgDetails = new ProductDetailDlg(this);

    pDlgDetails->data()->labelTitle->setText(products[pos].toMap()["product"].toMap()["title"].toString());
    pDlgDetails->data()->teDescription->setText(products[pos].toMap()["product"].toMap()["description"].toString());

    QTableWidgetItem* itemAuthor;
    QTableWidgetItem* itemTimeCreate;
    QTableWidgetItem* itemTimeModify;
    QTableWidgetItem* itemCountryLanguage;
    QTableWidgetItem* itemBrand;
    QTableWidgetItem* itemCondition;

    pDlgDetails->data()->twDetails->setColumnCount(1);
    pDlgDetails->data()->twDetails->setColumnWidth(0,150);

    QString str = products[pos].toMap()["product"].toMap()["author"].toMap()["name"].toString();
    itemAuthor = new QTableWidgetItem(str);
    pDlgDetails->data()->twDetails->setItem(0,0,itemAuthor);
    str = products[pos].toMap()["product"].toMap()["creationTime"].toString();
    itemTimeCreate = new QTableWidgetItem(convertToTime(str).toString("dd.MM.yyyy hh:mm:ss"));
    pDlgDetails->data()->twDetails->setItem(1,0,itemTimeCreate);
    str = products[pos].toMap()["product"].toMap()["modificationTime"].toString();
    itemTimeModify = new QTableWidgetItem(convertToTime(str).toString("dd.MM.yyyy hh:mm:ss"));
    pDlgDetails->data()->twDetails->setItem(2,0,itemTimeModify);
    str = products[pos].toMap()["product"].toMap()["country"].toString()+"/";
    str += products[pos].toMap()["product"].toMap()["language"].toString();
    itemCountryLanguage = new QTableWidgetItem(str);
    pDlgDetails->data()->twDetails->setItem(3,0,itemCountryLanguage);
    str = products[pos].toMap()["product"].toMap()["brand"].toString();
    itemBrand = new QTableWidgetItem(str);
    pDlgDetails->data()->twDetails->setItem(4,0,itemBrand);
    str = products[pos].toMap()["product"].toMap()["condition"].toString();
    itemCondition = new QTableWidgetItem(str);
    pDlgDetails->data()->twDetails->setItem(5,0,itemCondition);

    str = products[pos].toMap()["product"].toMap()["images"].toList()[0].toMap()["link"].toString();
    pDlgDetails->data()->wvLink->setUrl(str);

    QVariantList list = products[pos].toMap()["product"].toMap()["inventories"].toList();
    QTableWidgetItem* channel;
    QTableWidgetItem* availability;
    QTableWidgetItem* price;
    QTableWidgetItem* currency;
    int len = list.size();
    pDlgDetails->data()->twInventories->setRowCount(len);
    for (int i=0; i<len; i++) {
        channel = new QTableWidgetItem(list[i].toMap()["channel"].toString());
        pDlgDetails->data()->twInventories->setItem(i,0,channel);

        availability = new QTableWidgetItem(list[i].toMap()["availability"].toString());
        pDlgDetails->data()->twInventories->setItem(i,1,availability);

        price = new QTableWidgetItem(list[i].toMap()["price"].toString());
        pDlgDetails->data()->twInventories->setItem(i,2,price);

        currency = new QTableWidgetItem(list[i].toMap()["currency"].toString());
        pDlgDetails->data()->twInventories->setItem(i,3,currency);
    }
    pDlgDetails->exec();
    delete pDlgDetails;
}
QDateTime Form::convertToTime(const QString& str)
{
    QDateTime ret;

    int pos = str.indexOf("T");
    QString sYear = str.mid(0,4);
    QString sMonth = str.mid(5,2);
    QString sDay = str.mid(8,2);

    QString sHour = str.mid(pos+1,2);
    QString sMin = str.mid(pos+4,2);
    QString sSec = str.mid(pos+7,2);

    ret.setDate(QDate(sYear.toInt(),sMonth.toInt(),sDay.toInt()));
    ret.setTime(QTime(sHour.toInt(),sMin.toInt(),sSec.toInt()));

    return ret;
}

void Form::on_pbSearch_clicked()
{
    m_bKey = false;
    startSearch();
}

void Form::showProductDetails(int row, int column)
{
    showDetails();
}

void Form::checkComboBoxes()
{
    m_savePattern = ui->comboBoxSearch->currentText();
    m_saveCountry = ui->cbCountry->currentText();
    if (!m_savePattern.isEmpty()&&(ui->comboBoxSearch->findText(m_savePattern) == -1)) {
        ui->comboBoxSearch->insertItem(0,m_savePattern);
    }
    if ((!m_saveCountry.isEmpty())&&(ui->cbCountry->findText(m_saveCountry) == -1)) {
        ui->cbCountry->insertItem(0,m_saveCountry);
    }
    if (ui->comboBoxSearch->count() > m_combo_count) {
        ui->comboBoxSearch->removeItem(m_combo_count-1);
    }
    if (ui->cbCountry->count() > m_combo_count) {
        ui->cbCountry->removeItem(m_combo_count-1);
    }
}

void Form::saveSettings()
{
    int len = ui->comboBoxSearch->count();
    QStringList list;
    for (int i=0; i<len; i++) {
        list << ui->comboBoxSearch->itemText(i);
    }
    m_pSettings->setValue("list_patterns",list);

    len = ui->cbCountry->count();
    list.clear();
    for (int i=0; i<len; i++) {
        list << ui->cbCountry->itemText(i);
    }
    m_pSettings->setValue("list_countries",list);
}

void Form::refresh()
{
    m_pManager->refresh();
}

void Form::next()
{
    m_pManager->next();
}

void Form::back()
{
    m_pManager->back();
}
bool Form::isFacetAvailable()
{
    QVariantList& facets = m_pManager->getFacets();
    if (facets.size() == 0) {
        return false;
    }
    return true;
}
void Form::showHistogram()
{
    QVariantList facets = m_pManager->getFacets();
    FacetPlot* pPlot = new FacetPlot(this);
    pPlot->populate(facets);
    pPlot->exec();
}
