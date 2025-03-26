/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *loadButton;
    QPushButton *processButton;
    QPushButton *saveButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSlider *brightnessSlider;
    QLabel *brightnessValue;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QSlider *contrastSlider;
    QLabel *contrastValue;
    QLabel *imageLabel;
    QLabel *statusLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName("loadButton");

        horizontalLayout->addWidget(loadButton);

        processButton = new QPushButton(centralwidget);
        processButton->setObjectName("processButton");
        processButton->setEnabled(false);

        horizontalLayout->addWidget(processButton);

        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName("saveButton");
        saveButton->setEnabled(false);

        horizontalLayout->addWidget(saveButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        brightnessSlider = new QSlider(centralwidget);
        brightnessSlider->setObjectName("brightnessSlider");
        brightnessSlider->setMinimum(-100);
        brightnessSlider->setMaximum(100);
        brightnessSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(brightnessSlider);

        brightnessValue = new QLabel(centralwidget);
        brightnessValue->setObjectName("brightnessValue");

        horizontalLayout_2->addWidget(brightnessValue);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        horizontalLayout_2->addWidget(label_2);

        contrastSlider = new QSlider(centralwidget);
        contrastSlider->setObjectName("contrastSlider");
        contrastSlider->setMinimum(-100);
        contrastSlider->setMaximum(100);
        contrastSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(contrastSlider);

        contrastValue = new QLabel(centralwidget);
        contrastValue->setObjectName("contrastValue");

        horizontalLayout_2->addWidget(contrastValue);


        verticalLayout->addLayout(horizontalLayout_2);

        imageLabel = new QLabel(centralwidget);
        imageLabel->setObjectName("imageLabel");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(imageLabel->sizePolicy().hasHeightForWidth());
        imageLabel->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(imageLabel);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        verticalLayout->addWidget(statusLabel);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Qt Potrace", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Load Image", nullptr));
        processButton->setText(QCoreApplication::translate("MainWindow", "Process", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save SVG", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Brightness:", nullptr));
        brightnessValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Contrast:", nullptr));
        contrastValue->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        imageLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Ready - Load an image to begin", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
