#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include "potracelib.h"

class ImageProcessor
{
public:
    ImageProcessor();
    ~ImageProcessor();

    QImage applyImageAdjustments(const QImage &image, int brightness, int contrast);
    bool convertToBitmap(const QImage &image, potrace_bitmap_t *bm);
    potrace_state_t* processImage(const QImage &image, potrace_param_t *params);

private:
    potrace_state_t *potraceState;
};

#endif // IMAGEPROCESSOR_H 