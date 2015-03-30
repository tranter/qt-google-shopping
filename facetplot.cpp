#include <QDateTime>

#include <qpen.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
//#include <qwt_legend_item.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qwt_scale_draw.h>

#include "facetplot.h"
#include "ui_facetplot.h"

#include <QDebug>

class HistogramScaleDraw: public QwtScaleDraw
{
public:
    HistogramScaleDraw(const QVariantList& list) : m_list(list) {}
    virtual QwtText label(double v) const
    {
        bool bBrand =((FacetPlot *)m_pPlot)->getVBrand();

        int index = bBrand ? 0 : 1;
        if(fabs(floor(v + 0.5) - v) > 0.00001)
        {
            //Not integer value, return empty string
            return QwtText("");
        }
        int ind1 = (int)floor(v + 0.5) - 1;
        if(ind1 < m_list[index].toMap()["buckets"].toList().count() && ind1 >= 0)
        {
            return m_list[index].toMap()["buckets"].toList()[ind1].toMap()["value"].toString();
        }
        else
        {
            return QwtText("");
        }
    }
    void setPlot(QwtPlot* p) {m_pPlot = p;}

private:
    const QVariantList& m_list;
    QwtPlot* m_pPlot;
};

class Histogram: public QwtPlotHistogram
{
public:
    Histogram(const QString &, const QColor &);

    void setColor(const QColor &);
    void setValues(uint numValues, const double *);
    void setValues(const QVariant& facet);
};

Histogram::Histogram(const QString &title, const QColor &symbolColor):
    QwtPlotHistogram(title)
{
    setStyle(QwtPlotHistogram::Columns);
    setColor(symbolColor);
}

void Histogram::setColor(const QColor &symbolColor)
{
    QColor color = symbolColor;
    color.setAlpha(180);

    setPen(QPen(Qt::black));
    setBrush(QBrush(color));

    QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
    symbol->setFrameStyle(QwtColumnSymbol::Raised);
    symbol->setLineWidth(2);
    symbol->setPalette(QPalette(color));
    setSymbol(symbol);
}

void Histogram::setValues(uint numValues, const double *values)
{
    QVector<QwtIntervalSample> samples(numValues);
    for ( uint i = 0; i < numValues; i++ )
    {
        QwtInterval interval(double(i), i + 1.0);
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        samples[i] = QwtIntervalSample(values[i], interval);
    }

    setData(new QwtIntervalSeriesData(samples));
}

void Histogram::setValues(const QVariant& facet)
{
    uint len = facet.toMap()["buckets"].toList().size();
    QVector<QwtIntervalSample> samples(len);
    for ( uint i = 0; i < len; i++ )
    {
        QwtInterval interval(double(i), i + 1.0);
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        samples[i] = QwtIntervalSample(facet.toMap()["buckets"]
                                       .toList()[i]
                                       .toMap()["count"]
                                       .toString()
                                       .toDouble()
                                       , interval);
    }

    setData(new QwtIntervalSeriesData(samples));
}

FacetPlot::FacetPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FacetPlot)
{
    ui->setupUi(this);

    m_vBrand = true;

    ui->qwtPlot->setTitle("Histogram of Facets");
    ui->qwtPlot->setCanvasBackground(QColor(Qt::gray));
    ui->qwtPlot->plotLayout()->setAlignCanvasToScales(true);

    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "Amount, pcs");
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "Facet");

    QwtLegend *legend = new QwtLegend;
    //legend->setItemMode(QwtLegend::CheckableItem);
    ui->qwtPlot->insertLegend(legend, QwtPlot::RightLegend);

    connect(ui->qwtPlot, SIGNAL(legendChecked(QwtPlotItem *, bool)),
        SLOT(showItem(QwtPlotItem *, bool)));
}

FacetPlot::~FacetPlot()
{
    delete ui;
}

void FacetPlot::populate(const QVariantList& facets)
{
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(false);
    grid->enableY(true);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(ui->qwtPlot);

    HistogramScaleDraw* pScale = new HistogramScaleDraw(facets);
    pScale->setPlot((QwtPlot*)this);
    ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom, pScale);
    ui->qwtPlot->setAxisLabelRotation(QwtPlot::xBottom, -90.0);
    ui->qwtPlot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QColor aColor[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::cyan};
    int colorInd = 0;
    int len = facets.size();
    for (int i=0; i<len; i++) {
        Histogram *histogram = new Histogram(facets[i].toMap()["displayName"].toString(), aColor[colorInd++]);
        if (colorInd >= sizeof(aColor)) {
            colorInd = 0;
        }
        histogram->setValues(facets[i]);
        if (facets[i].toMap()["property"].toString() != "brand") {
            histogram->setVisible(false);
        }
        histogram->attach(ui->qwtPlot);
    }
}

void FacetPlot::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);

    m_vBrand = item->title().text() == "brand" ? on : !on;
    ui->qwtPlot->replot(); // creating the legend items
}
