#ifndef TRACEVIEW_H
#define TRACEVIEW_H

#include <QWidget>
#include "potracelib.h"

class TraceView : public QWidget
{
    Q_OBJECT
public:
    explicit TraceView(QWidget *parent = nullptr);
    void setTraceData(potrace_state_t *state);
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    potrace_state_t *traceState;
    QRectF bounds;
    void calculateBounds();
    void drawPath(QPainter &painter, potrace_path_t *path);
};

#endif // TRACEVIEW_H 