#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMap>

class QCreateGroupListwnd2Item : public QWidget {
	Q_OBJECT
public:
	QCreateGroupListwnd2Item(QWidget* p = nullptr);
	QCreateGroupListwnd2Item(QWidget* p, const char* headimg, const char* nickname, const char* rolename);
//signals:
//	void signal_selRBtnClick(QMap<QString,QString> map);
//public slots:
//	void slot_clickedSelBtn(bool isSel = false);
public:
	QHBoxLayout* m_hLayout;
	QLabel* m_headImage;
	QLabel* m_nickName;
	QLabel* m_roleName;
	QString m_headImgStr;
	
	//QRadioButton* m_selRBtn;
};