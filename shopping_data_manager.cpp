#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QMessageBox>
#include <QJson/Parser>

#include "shopping_data_manager.h"

ShoppingDataManager::ShoppingDataManager()
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    m_key = "YOUR_APP_KEY_HERE";
    m_startIndex = 1;
    m_maxResults = 25;
    m_facetsEnabled = true;

    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

ShoppingDataManager::~ShoppingDataManager()
{
    delete m_pNetworkAccessManager;
}
/*! \brief Send constructed request to google server
 *
 *  \param str Text of request
 *  \param country Specified country for selection (default US if string is empty).
 *  Amount, type of facets, count of backets are hard coded at the moment.
 */
void ShoppingDataManager::startSearch(const QString& str,const QString& country)
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << "search pattern=" << str << ", country=" << country;
    QString query;
    QString strCountry;

    m_saveQuery = str;
    m_saveCountry = country;

    if(!isKey()) return;

    if (m_saveCountry.isEmpty()) {
        m_saveCountry = "US";
    } else {
        m_saveCountry = country;
    }

    if (m_facetsEnabled) {
        query = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                                "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                                "&facets.enabled=true&facets.include=brand:5,condition:5"
                                "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    } else {
        query = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                                "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                                "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    }
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(query)));
}

void ShoppingDataManager::searchProducts(const QString auth_token,const QString& str,const QString& country)
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << "auth_token=" << auth_token << ", str=" << str << ", country=" << country;

    QString locCountry = country;
    if (locCountry.isEmpty()) {
        locCountry = "US";
    }
    QString s = QString("https://www.googleapis.com/shopping/search/v1/public/products?auth_token=%1&country=%2&q=%3&alt=json").arg(auth_token).arg(locCountry).arg(str);
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}

void ShoppingDataManager::showDetails(const int pos)
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << pos;
    if ((pos < 0)||(pos >= m_products.size())) {
        return;
    }

//    QMessageBox(QMessageBox::Information,"Info","No implemented yet.",QMessageBox::Ok,QApplication::activeWindow()).exec();

    QString s = QString("https://www.googleapis.com/shopping/search/v1/public/products/%1/gid/%2?key=%3&country=%4&alt=json")
            .arg(m_products[pos].toMap()["product"].toMap()["author"].toMap()["accountId"].toString())
            .arg(m_products[pos].toMap()["product"].toMap()["googleId"].toString())
            .arg(m_key)
            .arg(m_products[pos].toMap()["product"].toMap()["country"].toString());
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}

void ShoppingDataManager::replyFinished(QNetworkReply *reply)
{
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << reply->url();
    QString json = reply->readAll();
//    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << "Reply is: " << json;
    qDebug() << "****************### Reply = " << json.mid(0,25000);
//    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << "Check point 1";
    if (json.length() == 0) {
        QMessageBox(QMessageBox::Information,"Info","No data present.",QMessageBox::Ok,QApplication::activeWindow()).exec();
        return;
    }

    QJson::Parser parser;

    bool ok;

    // json is a QString containing the data to convert
    QVariant result = parser.parse (json.toLatin1(), &ok);
    if (!ok) {
        emit sigErrorOccured(QString("Cannot convert to QJson object: %1").arg(json));
        return;
    }

    if (result.toMap().contains("error")) {
        emit sigErrorOccured("Server error: "+result.toMap()["error"].toMap()["message"].toString());
        return;
    }

    if (result.toMap()["kind"] == "shopping#products") {
//        qDebug() << "Reply = " << json;
        m_products = result.toMap()["items"].toList();
        m_details.clear();
        m_facets.clear();
        if (result.toMap().contains("facets")) {
            m_facets = result.toMap()["facets"].toList();
        }
//        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << "Amount of products is " << m_products.size();
        emit sigProductsListReady();
        return;
    } else if (result.toMap()["kind"] == "shopping#product") {
//        qDebug() << "Reply = " << json;
        m_details = result.toMap()["items"].toList();
//        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") <<  __FUNCTION__ << "Amount of details is " << m_details.size();
        emit sigDetailsListReady();
        return;
    }
}

void ShoppingDataManager::refresh()
{
    if (m_saveQuery.isEmpty()) {
        return;
    }
    QString str;
    if (m_facetsEnabled) {
        str = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                      "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                      "&facets.enabled=true&facets.include=brand:5,condition:5"
                      "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    } else {
        str = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                      "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                      "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    }
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(str)));
}

void ShoppingDataManager::next()
{
    if (m_saveQuery.isEmpty()) {
        return;
    }
    m_startIndex += m_maxResults;
    QString str;
    if (m_facetsEnabled) {
        str = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                      "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                      "&facets.enabled=true&facets.include=brand:5,condition:5"
                      "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    } else {
        str = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                      "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                      "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    }
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(str)));
}

void ShoppingDataManager::back()
{
    if (m_saveQuery.isEmpty()) {
        return;
    }
    if ((m_startIndex-m_maxResults) < 1) {
        m_startIndex = 1;
    } else {
        m_startIndex -= m_maxResults;
    }
    QString str;
    if (m_facetsEnabled) {
        str = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                      "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                      "&facets.enabled=true&facets.include=brand:5,condition:5"
                      "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    } else {
        str = QString("https://www.googleapis.com/shopping/search/v1/public/products"
                      "?key=%1&country=%2&q=%3&startIndex=%4&maxResults=%5"
                      "&alt=json")
                .arg(m_key)
                .arg(m_saveCountry)
                .arg(m_saveQuery)
                .arg(m_startIndex)
                .arg(m_maxResults);
    }
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(str)));
}

bool ShoppingDataManager::isKey()
{
    bool key = false;

    if(m_key == "YOUR_APP_KEY_HERE")
    {
        QMessageBox::warning(0,"Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-shopping/wiki/HowToRegisterYourApplicationInGoogle'>here</a>");

    }
    else
    {
       key = true;
    }

    return key;
}
