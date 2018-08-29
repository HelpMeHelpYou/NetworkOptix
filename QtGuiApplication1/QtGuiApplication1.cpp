#include "QtGuiApplication1.h"

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtGuiApplication1::onRecalcutateButtonClicked);

	ui.spinBoxBookmarkCount->setPrefix("BookMarks count:");
	ui.spinBoxBookmarkCount->setMinimum(0);
	ui.spinBoxBookmarkCount->setMaximum(INT32_MAX);
	ui.spinBoxBookmarkCount->setValue(100);
	
	ui.doubleSpinBoxBookMarkDuratation->setPrefix("BookMarks max duratation:");
	ui.doubleSpinBoxBookMarkDuratation->setSuffix("s");

	ui.doubleSpinBoxBookMarkDuratation->setMinimum(0.0);
	ui.doubleSpinBoxBookMarkDuratation->setMaximum(3600*24);
	ui.doubleSpinBoxBookMarkDuratation->setValue(3600);
	onRecalcutateButtonClicked();
}

void QtGuiApplication1::onRecalcutateButtonClicked(void)
{
	this->ui.widget->renewBookMarks(ui.spinBoxBookmarkCount->value(), ui.doubleSpinBoxBookMarkDuratation->value());
}
