#include "QEmoijWnd.h"
#include <QApplication>
#include <QPainter>

QEmoijWnd::QEmoijWnd(QWidget* p /*= nullptr*/) 
	: QWidget(p)
{
	setFixedSize(400, 350);
	//ÉèÖÃÎÞ±ß¿òÊôÐÔ
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	m_vLayout = new QVBoxLayout();
	m_centerWnd = new QWidget();
	m_centerWnd->setContentsMargins(0, 0, 0, 0);
	m_vLayout->addWidget(m_centerWnd);
	setContentsMargins(5,5,5,15);
}


bool QEmoijWnd::event(QEvent* event) {
	if (event->type() == QEvent::ActivationChange)
	{
		if (QApplication::activeWindow() != this)
		{
			this->hide();
		}
	}
	return QWidget::event(event);
}

void QEmoijWnd::paintEvent(QPaintEvent* paintEvent)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(128, 128, 128));

	QPainterPath drawPath;
	drawPath.addRoundedRect(QRect(5, 5,width() - 10, height() - 10 - 15),5,5);

	QPolygon triPolygon;
	triPolygon << QPoint(width()/2,height());
	triPolygon << QPoint(width()/2+15, height()-20);
	triPolygon << QPoint(width()/2-15 , height()-20);
	drawPath.addPolygon(triPolygon);

	painter.drawPath(drawPath);


}
