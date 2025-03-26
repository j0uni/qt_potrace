#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QSettings>
#include "traceview.h"
#include "potracelib.h"
#include "imageprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadImage();
    void onProcessImage();
    void onSaveSVG();
    void onBrightnessChanged(int value);
    void onContrastChanged(int value);

private:
    void updateUI();
    void loadLastFile();
    void saveLastFile();

    potrace_state_t *state;

    Ui::MainWindow *ui;
    QImage sourceImage;
    QImage originalImage;  // Store the original image before adjustments
    potrace_param_t *potraceParams;
    TraceView *traceView;
    ImageProcessor imageProcessor;
    QSettings settings;
    QString lastUsedFile;
    int brightness;
    int contrast;
};
#endif // MAINWINDOW_H
