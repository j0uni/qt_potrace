#include "traceview.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

TraceView::TraceView(QWidget *parent)
    : QWidget(parent)
    , traceState(nullptr)
{
    setMinimumSize(400, 400);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
}

void TraceView::setTraceData(potrace_state_t *state)
{
    traceState = state;
    if (traceState) {
        calculateBounds();
    }
    update();
}

void TraceView::clear()
{
    traceState = nullptr;
    bounds = QRectF();
    update();
}

void TraceView::calculateBounds()
{
    if (!traceState || !traceState->plist) {
        bounds = QRectF();
        return;
    }

    double minX = 0, minY = 0, maxX = 0, maxY = 0;
    bool first = true;

    potrace_path_t *path = traceState->plist;
    while (path) {
        for (int i = 0; i < path->curve.n; i++) {
            for (int j = 0; j < 3; j++) {
                if (path->curve.tag[i] == POTRACE_CURVETO || j > 0) {
                    if (first) {
                        minX = maxX = path->curve.c[i][j].x;
                        minY = maxY = path->curve.c[i][j].y;
                        first = false;
                    } else {
                        minX = qMin(minX, path->curve.c[i][j].x);
                        minY = qMin(minY, path->curve.c[i][j].y);
                        maxX = qMax(maxX, path->curve.c[i][j].x);
                        maxY = qMax(maxY, path->curve.c[i][j].y);
                    }
                }
            }
        }
        path = path->next;
    }

    bounds = QRectF(minX, minY, maxX - minX, maxY - minY);
}

void TraceView::drawPath(QPainter &painter, potrace_path_t *path)
{
    if (!path || path->curve.n <= 0) {
        return;
    }

    QPainterPath painterPath;
    
    // Start with the first point - for corners, use vertex, for curves use first control point
    QPointF currentPoint;
    if (path->curve.tag[0] == POTRACE_CORNER) {
        currentPoint = QPointF(path->curve.c[0][1].x, path->curve.c[0][1].y);
    } else {
        currentPoint = QPointF(path->curve.c[0][0].x, path->curve.c[0][0].y);
    }
    painterPath.moveTo(currentPoint);

    // Process each segment
    for (int i = 0; i < path->curve.n; i++) {
        if (path->curve.tag[i] == POTRACE_CURVETO) {
            // Bezier curve - use the control points and endpoint from the current segment
            QPointF control1(path->curve.c[i][0].x, path->curve.c[i][0].y);
            QPointF control2(path->curve.c[i][1].x, path->curve.c[i][1].y);
            QPointF endPoint(path->curve.c[i][2].x, path->curve.c[i][2].y);
            
            // Draw the Bezier curve from current point
            painterPath.cubicTo(control1, control2, endPoint);
            currentPoint = endPoint;
        } else {
            // Corner - use the vertex and next point from the current segment
            QPointF endPoint(path->curve.c[i][2].x, path->curve.c[i][2].y);
            
            // Draw from current point to end point
            painterPath.lineTo(endPoint);
            currentPoint = endPoint;
        }
    }

    // Close the path
    painterPath.closeSubpath();

    // Set the fill color based on the path's sign
    painter.setBrush(path->sign == '+' ? Qt::black : Qt::white);
    painter.drawPath(painterPath);
}

void TraceView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if (!traceState || !traceState->plist) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate scaling to fit the paths in the widget
    QRectF viewRect = rect();
    if (bounds.isEmpty()) {
        return;
    }

    // Calculate initial scale to fit in widget
    double scaleX = viewRect.width() / bounds.width();
    double scaleY = viewRect.height() / bounds.height();
    double scale = qMin(scaleX, scaleY);

    // Downscale if either dimension exceeds 1024 pixels
    double maxDimension = 1024.0;
    double scaledWidth = bounds.width() * scale;
    double scaledHeight = bounds.height() * scale;
    
    if (scaledWidth > maxDimension || scaledHeight > maxDimension) {
        double downscale = maxDimension / qMax(scaledWidth, scaledHeight);
        scale *= downscale;
    }

    // Center the paths in the widget
    double offsetX = (viewRect.width() - bounds.width() * scale) / 2;
    double offsetY = (viewRect.height() - bounds.height() * scale) / 2;

    painter.translate(offsetX, offsetY);
    painter.scale(scale, scale);
    painter.translate(-bounds.left(), -bounds.top());

    // Draw the paths
    potrace_path_t *path = traceState->plist;
    while (path) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(path->sign == '+' ? Qt::black : Qt::white);
        drawPath(painter, path);
        path = path->next;
    }
}

void TraceView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();
} 
