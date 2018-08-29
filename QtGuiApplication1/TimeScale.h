#pragma once
#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QMap>

#include "BookMark.h"

class BookMarkView;

class TimeScale :
	public QWidget
{
public:
	TimeScale(QWidget * parent);
	virtual ~TimeScale();
	
	void requstTooltip(QLabel * lb,int index);

public slots:
	void renewBookMarks(int bookMarksCount, double maxDuratation);

protected:
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

	void discardAndGenerateBookmarks(double start, double end, double maxSize, size_t count);
private:
	QFrame * Tooltip;

	QVector <QLabel*> _hoursCounter;
	QVector <BookMarkView*> _bookmarksLables;
	QVector <QLabel*> _tooltipBookmarks;

	unsigned int _hoursCounterHeightOffset = 12;
	unsigned int _hoursCounterWidthOffset = 50;

private:

	QMap <double, BookMarkEntry> bookmarks;
	QVector <double> _startIndex;
	QVector <double> _endIndex;

	static const size_t _maxLabelsCount = 100;
	static const unsigned int SECS_IN_HOUR = 3600;
	static const unsigned int _collapseThereholdPX = 100;
	static const unsigned int _bookmarkHeight = 35;
	static const bool _snakeMode = false;
};

class BookMarkView : public QLabel
{
public:
	int index;
	BookMarkView(TimeScale * parent) : QLabel(parent), pointer(parent)
	{}

	void mouseMoveEvent(QMouseEvent *event) override
	{
		pointer->requstTooltip(this,index);
	}
private:
	TimeScale * pointer;
};