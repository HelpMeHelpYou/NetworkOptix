#include "TimeScale.h"

#include <QSizepolicy>
#include <QFrame>
#include <QVboxLayout>
#include <QResizeEvent>
#include <QApplication>
#include <QtGlobal>

TimeScale::TimeScale(QWidget * parent) : QWidget(parent)
{
	QSizePolicy policy;

	policy.setHorizontalPolicy(QSizePolicy::Expanding);
	policy.setVerticalPolicy(QSizePolicy::Expanding);

	this->setSizePolicy(policy);

	this->setMinimumSize(QSize(400, 100));

	Tooltip = new QFrame(parent);
	auto layout = new QVBoxLayout(Tooltip);

	Tooltip->setHidden(true);
	Tooltip->layout()->setSizeConstraint(QLayout::SetMinimumSize);
	Tooltip->setWindowOpacity(1.0);
	Tooltip->setStyleSheet("opacity: 1.0");

	Tooltip->setAttribute(Qt::WA_TranslucentBackground, false);
	Tooltip->setStyleSheet("background-color: yellow;");
	Tooltip->setFrameShape(QFrame::Box);
	Tooltip->setLineWidth(1);


	for (int i = 0; i <= 15; i++)
	{
		QLabel * lb = new QLabel(this);
		Tooltip->layout()->addWidget(lb);
		lb->setHidden(true);
		_tooltipBookmarks.append(lb);
		lb->setWindowOpacity(1.0);
	}

	for (int i = 0; i <= 24; i++)
	{
		QLabel * lb = new QLabel(this);
		lb->setText(QObject::tr("%1").arg(i));
		_hoursCounter.append(lb);
		QFrame * fm = new QFrame(this);
		fm->setFixedSize(2, 10);
		fm->setFrameShape(QFrame::Box);
		fm->setLineWidth(1);
		_scale.append(fm);
		
	}



	_startIndex.resize(_maxLabelsCount);
	_endIndex.resize(_maxLabelsCount);

	for (int i = 0; i < _maxLabelsCount; i++)
	{
		BookMarkView * lb = new BookMarkView(this);
		_bookmarksLables.append(lb);
		lb->setHidden(true);
		lb->setFrameShape(QFrame::Box);
		lb->setLineWidth(1);
		lb->setMouseTracking(true);
	}
}


void TimeScale::requstTooltip(QLabel * lb, int index)
{
	Tooltip->setHidden(false);

	Tooltip->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	Tooltip->setFocus();

	Tooltip->show();
	int k = 0;
	for (auto it = bookmarks.find(_startIndex[index]); (it-1 != bookmarks.find(_endIndex[index])) && (k < _tooltipBookmarks.size()); it++, k++)
	{
		_tooltipBookmarks[k]->setHidden(false);
		if ( (k == _tooltipBookmarks.size()-1) && (_endIndex[index] - _startIndex[index])  >  _tooltipBookmarks.size())
		{
			_tooltipBookmarks[k]->setText(QObject::tr("and other %1 bookmarks").arg(bookmarks.find(_endIndex[index])->count - it->count -14));
		}
		else
		{
			_tooltipBookmarks[k]->setText(QObject::tr("bookmark %1 start %2 end %3").arg(it->count).arg(it.key() / SECS_IN_HOUR).arg(it->end / SECS_IN_HOUR));
		}
		Tooltip->layout()->addWidget(_tooltipBookmarks[k]);
	}

	for (; k < _tooltipBookmarks.size(); k++)
	{
		_tooltipBookmarks[k]->setHidden(true);
		Tooltip->layout()->removeWidget(_tooltipBookmarks[k]);
	}

	Tooltip->layout()->setSizeConstraint(QLayout::SetMinimumSize);
	Tooltip->resize(Tooltip->sizeHint());


	int x = this->pos().x() + lb->pos().x();
	int y = this->pos().y() + lb->pos().y();


	Tooltip->move( qMin(x,this->size().width()- Tooltip->size().width()), y + lb->size().height() + 5);
}

void TimeScale::renewBookMarks(int bookMarksCount, double maxDuratation)
{
	discardAndGenerateBookmarks(0, 24 * 3600, maxDuratation, bookMarksCount);
}

TimeScale::~TimeScale()
{
	int w = width();
	int h = height();
}

void TimeScale::mouseMoveEvent(QMouseEvent * event)
{
	int w = width();
	int h = height();
}

void TimeScale::mousePressEvent(QMouseEvent * event)
{
	int w = width();
	int h = height();
}

void TimeScale::paintEvent(QPaintEvent * event)
{
	int w = width();
	int h = height();
}

void TimeScale::resizeEvent(QResizeEvent * event)
{
	int w = width();
	int h = height();

	size_t sz = _hoursCounter.size();

	double  increment = (w - (_hoursCounterWidthOffset * 2)) / (sz - 1);

	for (int i = 0; i < sz; i++)
	{

		_hoursCounter.at(i)->move(_hoursCounterWidthOffset + increment * i- _hoursCounter.at(i)->size().width()/2,
			_hoursCounterHeightOffset);
		_scale.at(i)->move(_hoursCounterWidthOffset + increment * i - 1, 5);
	}

	size_t bookMarksCount = bookmarks.size();

	increment = increment / SECS_IN_HOUR;


	int current_offset = _hoursCounterWidthOffset;
	int start = 0;
	int end = 0;
	int k = 0;

	bool isOpened = false;

	for (auto it = bookmarks.begin(); it != bookmarks.end() ; it++)
	{
		start = _hoursCounterWidthOffset + int(increment*(it.key()));

		_startIndex[k] = it.key();
		size_t startCount = it->count;

		auto itNext = bookmarks.upperBound(it.key() + _collapseThereholdPX /increment);

		it = itNext - 1;
		_endIndex[k] = it.key();
		end = _hoursCounterWidthOffset + it->end*increment;
		size_t endCount = it->count;
	
			_bookmarksLables[k]->setHidden(false);
			_bookmarksLables[k]->move(start, _hoursCounterHeightOffset + _bookmarkHeight+5 + (_bookmarkHeight+5) * _snakeMode *(k % 2));
			_bookmarksLables[k]->setFixedSize(end - start, _bookmarkHeight);
			if (_startIndex[k] != _endIndex[k])
			{
				_bookmarksLables[k]->setText(QObject::tr("%1 .. %2").arg(startCount).arg(endCount));
				_bookmarksLables[k]->setStyleSheet("background-color: rgba(0, 255, 0, 50);");
			}
			else
			{
				_bookmarksLables[k]->setText(QObject::tr("%1").arg(startCount));
				_bookmarksLables[k]->setStyleSheet("background-color: rgba(0, 0, 255, 50);");
			}
			_bookmarksLables[k]->index = k;
			k++;
			if (itNext == bookmarks.end())
				break;
	}




	for (int i = k; i < _bookmarksLables.size(); i++)
	{
		_bookmarksLables[i]->setHidden(true);
	}

}

void TimeScale::discardAndGenerateBookmarks(double start, double end, double maxDuratation, size_t count)
{
	bookmarks.clear();
	QVector<int> vecStart;

	long long sum = 0;
	for (size_t i = 0; i < count+1; i++)
	{
		int a = qrand();
		vecStart.push_back(a);
		sum += a;
	}
	double current = 0;

	for (size_t i = 0; i < count; i++)
	{
		current += start + vecStart[i] * ((end - start)) / sum;
		bookmarks[current] = { qMin(current + (maxDuratation * qrand() / RAND_MAX) ,end), i};
	}
	QSize oldSize(this->size());
	QResizeEvent *  myResizeEvent = new QResizeEvent(oldSize, oldSize);
	QCoreApplication::postEvent(this, myResizeEvent);
}
