#include "QEmoijWnd.h"
#include <QApplication>
#include <QPainter>
#include <QTabWidget>
#include <QHeaderView>
#include <QFile>
#include <QDebug>

QEmoijWnd::QEmoijWnd(QWidget* p /*= nullptr*/) 
	: QWidget(p)
{
	setFixedSize(335, 225);
	//设置无边框属性
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	m_vLayout = new QVBoxLayout();
	m_centerWnd = new QTableWidget();
	m_centerWnd->setRowCount(6);
	m_centerWnd->setColumnCount(10);
	m_centerWnd->setContentsMargins(0, 0, 0, 0);
	m_vLayout->addWidget(m_centerWnd);

	//m_centerWnd->setItem(0, 0, new QTableWidgetItem("1"));
	//m_centerWnd->setItem(1, 0, new QTableWidgetItem("Feb"));
	//m_centerWnd->setItem(2, 0, new QTableWidgetItem("Mar"));

	m_centerWnd->verticalHeader()->setVisible(false); //隐藏列表头 
	m_centerWnd->horizontalHeader()->setVisible(false); //隐藏行表头

		// 设置无焦点
	m_centerWnd->setFocusPolicy(Qt::NoFocus);
	// 设置不可编辑
	m_centerWnd->setEditTriggers(QAbstractItemView::NoEditTriggers);


	//m_centerWnd->verticalHeader()->setFixedWidth(30);
	//m_centerWnd->horizontalHeader()->setFixedHeight(30);
	for (int i=0;i<m_centerWnd->rowCount(); i++)
	{
		m_centerWnd->setRowHeight(i, 30);
	}

	for (int i=0;i<m_centerWnd->columnCount(); i++)
	{
		m_centerWnd->setColumnWidth(i, 30);
	}

	setLayout(m_vLayout);
	setContentsMargins(5,5,5,15);
	setAttribute(Qt::WA_StyledBackground);
	///读取文件
	QFile emoijFile("./emoij/emoij.txt");
	if (emoijFile.exists() && emoijFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str = emoijFile.readAll();
		m_emoijStrList =  str.split('|', QString::SkipEmptyParts);
		for (int i = 0; i < m_emoijStrList.length(); i++)
		{
			int col = i / 10;
			int row = i % 10;
			m_centerWnd->setItem(col,row, new QTableWidgetItem(m_emoijStrList[i]));
		}
	}

	connect(m_centerWnd, SIGNAL(cellClicked(int, int)), this, SLOT(slot_cellClicked(int, int)));
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

void QEmoijWnd::slot_cellClicked(int x, int y)
{
	qDebug() << "slot_cellClicked:" << x << "," << y;
	//点击完成后隐藏窗口
	if (x * 10 + y >= 0 && x * 10 + y < m_emoijStrList.size())
	{
		qDebug() << m_emoijStrList[x*10+y] << endl;
		emit signal_emoijClicked(m_emoijStrList[x*10+y]);
	}
	hide();
}
