#include "imageprocessor.h"
#include <QDebug>

ImageProcessor::ImageProcessor()
    : potraceState(nullptr)
{
}

ImageProcessor::~ImageProcessor()
{
    if (potraceState) {
        potrace_state_free(potraceState);
    }
}

QImage ImageProcessor::applyImageAdjustments(const QImage &image, int brightness, int contrast)
{
    QImage result = image.convertToFormat(QImage::Format_Grayscale8);
    
    // Apply contrast
    if (contrast != 0) {
        double factor = (259.0 * (contrast + 255)) / (255.0 * (259 - contrast));
        for (int y = 0; y < result.height(); y++) {
            for (int x = 0; x < result.width(); x++) {
                int pixel = qGray(result.pixel(x, y));
                pixel = factor * (pixel - 128) + 128;
                pixel = qBound(0, pixel, 255);
                result.setPixel(x, y, qRgb(pixel, pixel, pixel));
            }
        }
    }
    
    // Apply brightness
    if (brightness != 0) {
        for (int y = 0; y < result.height(); y++) {
            for (int x = 0; x < result.width(); x++) {
                int pixel = qGray(result.pixel(x, y));
                pixel = qBound(0, pixel + brightness, 255);
                result.setPixel(x, y, qRgb(pixel, pixel, pixel));
            }
        }
    }
    
    return result;
}

bool ImageProcessor::convertToBitmap(const QImage &image, potrace_bitmap_t *bm)
{
    bm->w = image.width();
    bm->h = image.height();
    bm->dy = (bm->w + sizeof(potrace_word) * 8 - 1) / (sizeof(potrace_word) * 8);
    bm->map = new potrace_word[bm->dy * bm->h];
    
    if (!bm->map) {
        return false;
    }
    
    // Convert image to bitmap
    for (int y = 0; y < bm->h; y++) {
        for (int x = 0; x < bm->w; x++) {
            int wordIndex = x / (sizeof(potrace_word) * 8);
            int bitIndex = x % (sizeof(potrace_word) * 8);
            
            if (qGray(image.pixel(x, y)) < 128) {
                bm->map[y * bm->dy + wordIndex] |= 1UL << (sizeof(potrace_word) * 8 - 1 - bitIndex);
            } else {
                bm->map[y * bm->dy + wordIndex] &= ~(1UL << (sizeof(potrace_word) * 8 - 1 - bitIndex));
            }
        }
    }
    
    return true;
}

potrace_state_t* ImageProcessor::processImage(const QImage &image, potrace_param_t *params)
{
    // Convert image to bitmap
    potrace_bitmap_t bm;
    if (!convertToBitmap(image, &bm)) {
        return nullptr;
    }
    
    // Process with Potrace
    if (potraceState) {
        potrace_state_free(potraceState);
    }
    
    potraceState = potrace_trace(params, &bm);
    delete[] bm.map;
    
    return potraceState;
} 