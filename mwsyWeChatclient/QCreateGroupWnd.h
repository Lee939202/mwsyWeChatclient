#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>

#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QMap>

class QCreateGroupWnd : public QWidget {
	Q_OBJECT
public:
	QCreateGroupWnd(QWidget* p = nullptr);

	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void addListWnd1Item(const char* headUrl,const char* nickname,const char* rolename);
	void addListWnd2Item(const char* headUrl, const char* nickname, const char* rolename);

	//通过rolename判断listwnd2中是否已经添加此窗口
	bool hasThisWndByRolename(QString rolename,QListWidget* listWnd);
	//通过rolename移除listwnd2中中此窗口
	void delThisWndByRolename(QString rolename, QListWidget* listWnd);
public slots:
	void closeWnd();
	void minWnd();

	//接收到来自单选的按钮的选择时候
	void slot_selRBtnClick(QMap<QString, QString> map);
public:
	QVBoxLayout* m_vLayout;

	QHBoxLayout* m_hLayout1;
	
	//提供顶部支持，title,关闭和缩小按钮
	QHBoxLayout* m_hLayout2;
	QLabel* m_titleLabel;
	QPushButton* m_minBtn;
	QPushButton* m_closeBtn;

	QLineEdit* m_serchEdit;
	QListWidget* m_listWnd1;
	QVBoxLayout* m_vLayout1;

	QLabel* m_hasSelLabel;
	QListWidget* m_listWnd2;
	QLineEdit* m_groupNameEdit;
	QHBoxLayout* m_hLayout3;
	QPushButton* m_comfirmBtn;
	QVBoxLayout* m_vLayout2;

	bool m_bPress = false;
	QPoint m_poPress;

};