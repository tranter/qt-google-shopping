#ifndef FACETPLOT_H
#define FACETPLOT_H

#include <QDialog>
#include <qwt_plot_item.h>
#include <QVariantList>

namespace Ui {
    class FacetPlot;
}

class FacetPlot : public QDialog
{
    Q_OBJECT

public:
    explicit FacetPlot(QWidget *parent = 0);
    ~FacetPlot();

    void populate(const QVariantList& facets);
    bool getVBrand() {return m_vBrand;}

private Q_SLOTS:
    void showItem(QwtPlotItem *item, bool on);

private:
    Ui::FacetPlot *ui;
    bool m_vBrand;          ///< flag for visual Brand item. Not good solution, only for test.
};

#endif // FACETPLOT_H
