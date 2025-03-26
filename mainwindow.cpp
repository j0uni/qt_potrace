#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , potraceParams(nullptr)
    , traceView(new TraceView(this))
    , settings("QtPotrace", "QtPotrace")
    , brightness(0)
    , contrast(0)
{
    ui->setupUi(this);

    state=0;
    
    // Initialize Potrace parameters
    potraceParams = potrace_param_default();
    if (!potraceParams) {
        QMessageBox::critical(this, "Error", "Failed to initialize Potrace parameters");
        return;
    }
    
    // Replace the image label with our trace view
    QWidget *parentWidget = qobject_cast<QWidget*>(ui->imageLabel->parent());
    if (parentWidget) {
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
        if (layout) {
            layout->replaceWidget(ui->imageLabel, traceView);
            delete ui->imageLabel;
        }
    }
    
    // Connect signals
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadImage);
    connect(ui->processButton, &QPushButton::clicked, this, &MainWindow::onProcessImage);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveSVG);
    connect(ui->brightnessSlider, &QSlider::valueChanged, this, &MainWindow::onBrightnessChanged);
    connect(ui->contrastSlider, &QSlider::valueChanged, this, &MainWindow::onContrastChanged);
    
    updateUI();

    // Load the last used file
    loadLastFile();
}

MainWindow::~MainWindow()
{
    if (potraceParams) {
        potrace_param_free(potraceParams);
    }
    delete ui;
}

void MainWindow::loadLastFile()
{
    lastUsedFile = settings.value("lastFile").toString();
    if (!lastUsedFile.isEmpty() && QFile::exists(lastUsedFile)) {
        sourceImage.load(lastUsedFile);
        if (!sourceImage.isNull()) {
            sourceImage = sourceImage.convertToFormat(QImage::Format_Grayscale8);
            onProcessImage(); // Automatically process the image
        }
    }
}

void MainWindow::saveLastFile()
{
    settings.setValue("lastFile", lastUsedFile);
}

void MainWindow::onLoadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), lastUsedFile.isEmpty() ? QDir::homePath() : QFileInfo(lastUsedFile).path(), 
        tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }

    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Could not load image."));
        return;
    }

    // Downscale if either dimension exceeds 1024 pixels
    if (image.width() > 1024 || image.height() > 1024) {
        image = image.scaled(1024, 1024, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // Store the original image and apply adjustments
    originalImage = image;
    sourceImage = imageProcessor.applyImageAdjustments(originalImage, brightness, contrast);
    
    // Store the filename
    lastUsedFile = fileName;
    saveLastFile();
    
    // Display the image
    traceView->setTraceData(nullptr);
    traceView->update();
    
    updateUI();
}

void MainWindow::onBrightnessChanged(int value)
{
    brightness = value;
    ui->brightnessValue->setText(QString::number(value));
    if (!originalImage.isNull()) {
        sourceImage = imageProcessor.applyImageAdjustments(originalImage, brightness, contrast);
        traceView->update();
        onProcessImage();
    }
}

void MainWindow::onContrastChanged(int value)
{
    contrast = value;
    ui->contrastValue->setText(QString::number(value));
    if (!originalImage.isNull()) {
        sourceImage = imageProcessor.applyImageAdjustments(originalImage, brightness, contrast);
        traceView->update();
        onProcessImage();
    }
}

void MainWindow::onProcessImage()
{
    if (sourceImage.isNull()) {
        return;
    }
    
    // Process with Potrace
    state = imageProcessor.processImage(sourceImage, potraceParams);
    if (!state) {
        QMessageBox::warning(this, "Error", "Failed to process image with Potrace");
        return;
    }
    
    // Update the trace view
    traceView->setTraceData(state);
    
    updateUI();
}

void MainWindow::onSaveSVG()
{

    if (!state) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save SVG"), lastUsedFile.isEmpty() ? QDir::homePath() : QFileInfo(lastUsedFile).path(),
        tr("SVG Files (*.svg)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Could not save file."));
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Write SVG header
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
        << "<svg width=\"" << sourceImage.width() << "\" height=\"" << sourceImage.height() << "\" "
        << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
        << "<g transform=\"translate(0," << sourceImage.height() << ") scale(1,-1)\">\n";

    // Write paths
    potrace_path_t *path = state->plist;
    while (path) {
        out << "<path d=\"";
        
        // Start with the first point
        if (path->curve.n > 0) {
            out << "M " << path->curve.c[0][2].x << "," << path->curve.c[0][2].y << " ";
        }

        // Process each segment
        for (int i = 0; i < path->curve.n; i++) {
            if (path->curve.tag[i] == POTRACE_CURVETO) {
                // Bezier curve
                out << "C " 
                    << path->curve.c[i][0].x << "," << path->curve.c[i][0].y << " "
                    << path->curve.c[i][1].x << "," << path->curve.c[i][1].y << " "
                    << path->curve.c[i][2].x << "," << path->curve.c[i][2].y << " ";
            } else {
                // Line segment
                out << "L " << path->curve.c[i][2].x << "," << path->curve.c[i][2].y << " ";
            }
        }

        // Close the path
        out << "Z\" fill=\"" << (path->sign == '+' ? "black" : "white") << "\"/>\n";
        path = path->next;
    }

    out << "</g>\n</svg>\n";
    file.close();
    lastUsedFile = fileName;
    saveLastFile();
    statusBar()->showMessage(tr("SVG saved."));
}

void MainWindow::updateUI()
{
    ui->processButton->setEnabled(!sourceImage.isNull());
    ui->saveButton->setEnabled(!sourceImage.isNull());
    
    if (sourceImage.isNull()) {
        ui->statusLabel->setText("Ready - Load an image to begin");
    } else {
        ui->statusLabel->setText("Image loaded - Click Process to trace");
    }
}
