#ifndef SHOPPING_DATA_MANAGER_H
#define SHOPPING_DATA_MANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QVariantList>

class ShoppingDataManager : public QObject
{
    Q_OBJECT

public:
    ShoppingDataManager();
    ~ShoppingDataManager();

    void startSearch(const QString& str,const QString& country);
    void searchProducts(const QString auth_token,const QString& str,const QString& country);
    void showDetails(const int pos);
    void refresh();
    void next();
    void back();

    int getStartIndex() {return m_startIndex;}
    void setStartIndex(int index) {m_startIndex = index;}
    int getMaxResults() {return m_maxResults;}
    void setMaxResults(int max) {m_maxResults = max;}
    bool isFacetsEnabled() {return m_facetsEnabled;}
    void setFacetsEnabled(bool flag) {m_facetsEnabled = flag;}

    QVariantList& getProducts() {return m_products;}
    QVariantList& getDetails() {return m_details;}
    QVariantList& getFacets() {return m_facets;}

signals:
    void sigErrorOccured(const QString& error);
    void sigProductsListReady();
    void sigDetailsListReady();

private slots:
    void replyFinished(QNetworkReply*);

private:
    QNetworkAccessManager* m_pNetworkAccessManager;
    QString m_key;
    int m_startIndex;           ///< start index for result set
    int m_maxResults;           ///< max rows in result set
    QString m_saveQuery;        ///< save query for refresh
    QString m_saveCountry;      ///< save country for refresh
    bool m_facetsEnabled;       ///< flag for facets

    QVariantList m_products;
    QVariantList m_details;
    QVariantList m_facets;      ///< contain all facets returned from server
};

#endif // SHOPPING_DATA_MANAGER_H
