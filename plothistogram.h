#ifndef PLOTHISTOGRAM_H
#define PLOTHISTOGRAM_H

#include <qwt_plot_histogram.h>

class PlotHistogram : public QwtPlotHistogram
{
    Q_OBJECT
public:
    PlotHistogram(const QString &, const QColor &);
//    explicit PlotHistogram(QObject *parent = 0);

    void setColor(const QColor &);
    void setValues(uint numValues, const double *);
};

#endif // PLOTHISTOGRAM_H
