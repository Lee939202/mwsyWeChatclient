#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class QPictureToolWnd : public QWidget
{
	Q_OBJECT
public:
	QPictureToolWnd(QWidget* p = nullptr);
public slots:
	void slot_uploadBtnClicked();
	void slot_cancelBtnClicked();
	void slot_determineBtnClicked();
public:
	QLabel* m_picLable;
	QPixmap m_HeadImg;
	QString m_filePath;

	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;

	QPushButton* m_determineBtn;
	QPushButton* m_cancelBtn;
	QPushButton* m_uploadBtn;
};

