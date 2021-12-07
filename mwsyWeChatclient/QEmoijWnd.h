#pragma once

#include <QWidget>
#include <QEvent>
#include <QVBoxLayout>

class QEmoijWnd : public QWidget
{
	Q_OBJECT
public:
	QEmoijWnd(QWidget* p = nullptr);
protected:
	bool event(QEvent* event);

	void paintEvent(QPaintEvent* paintEvent);
public:
	QWidget* m_centerWnd;
	QVBoxLayout* m_vLayout;
};

