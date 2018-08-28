/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *ImportButton;
    QSlider *scaleSlider;
    QGroupBox *NavigationBox;
    QPushButton *start_select_bt;
    QPushButton *end_select_bt;
    QLineEdit *startpos;
    QLineEdit *endpos;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(400, 300);
        Widget->setAutoFillBackground(true);
        ImportButton = new QPushButton(Widget);
        ImportButton->setObjectName(QStringLiteral("ImportButton"));
        ImportButton->setEnabled(true);
        ImportButton->setGeometry(QRect(10, 10, 80, 80));
        ImportButton->setCursor(QCursor(Qt::PointingHandCursor));
        ImportButton->setMouseTracking(false);
        ImportButton->setAcceptDrops(false);
        ImportButton->setAutoFillBackground(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/download00.png"), QSize(), QIcon::Normal, QIcon::Off);
        ImportButton->setIcon(icon);
        ImportButton->setIconSize(QSize(80, 80));
        ImportButton->setCheckable(false);
        ImportButton->setAutoRepeat(false);
        ImportButton->setAutoExclusive(false);
        ImportButton->setAutoRepeatDelay(300);
        ImportButton->setAutoRepeatInterval(100);
        ImportButton->setAutoDefault(false);
        ImportButton->setFlat(true);
        scaleSlider = new QSlider(Widget);
        scaleSlider->setObjectName(QStringLiteral("scaleSlider"));
        scaleSlider->setGeometry(QRect(20, 110, 16, 160));
        scaleSlider->setOrientation(Qt::Vertical);
        NavigationBox = new QGroupBox(Widget);
        NavigationBox->setObjectName(QStringLiteral("NavigationBox"));
        NavigationBox->setGeometry(QRect(110, 10, 241, 81));
        NavigationBox->setAutoFillBackground(false);
        NavigationBox->setFlat(true);
        NavigationBox->setCheckable(true);
        NavigationBox->setChecked(false);
        start_select_bt = new QPushButton(NavigationBox);
        start_select_bt->setObjectName(QStringLiteral("start_select_bt"));
        start_select_bt->setEnabled(false);
        start_select_bt->setGeometry(QRect(20, 20, 40, 30));
        start_select_bt->setAutoFillBackground(true);
        start_select_bt->setCheckable(false);
        start_select_bt->setChecked(false);
        start_select_bt->setFlat(true);
        end_select_bt = new QPushButton(NavigationBox);
        end_select_bt->setObjectName(QStringLiteral("end_select_bt"));
        end_select_bt->setEnabled(false);
        end_select_bt->setGeometry(QRect(20, 50, 40, 30));
        end_select_bt->setAutoFillBackground(true);
        end_select_bt->setCheckable(false);
        end_select_bt->setFlat(true);
        startpos = new QLineEdit(NavigationBox);
        startpos->setObjectName(QStringLiteral("startpos"));
        startpos->setGeometry(QRect(60, 20, 180, 30));
        startpos->setAutoFillBackground(true);
        startpos->setDragEnabled(true);
        startpos->setReadOnly(true);
        endpos = new QLineEdit(NavigationBox);
        endpos->setObjectName(QStringLiteral("endpos"));
        endpos->setGeometry(QRect(60, 50, 180, 30));
        endpos->setAutoFillBackground(true);
        endpos->setDragEnabled(true);
        endpos->setReadOnly(true);

        retranslateUi(Widget);

        ImportButton->setDefault(false);
        start_select_bt->setDefault(false);
        end_select_bt->setDefault(false);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        ImportButton->setText(QString());
        NavigationBox->setTitle(QString());
        start_select_bt->setText(QApplication::translate("Widget", "Start", 0));
        end_select_bt->setText(QApplication::translate("Widget", "End", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
