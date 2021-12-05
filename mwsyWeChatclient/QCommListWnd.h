#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMap>
#include <QString>

#include "QSelectWnd1.h"

class QCommListWnd : public QWidget
{
	//�����Ϣӳ��֧��
	Q_OBJECT

public:
	enum QCommListWndEnum
	{
		QCommMsgItemWnd_Tpye = 0,
		QCommContactItemWnd_Type
	};

	QCommListWnd(QWidget* p = nullptr, QCommListWndEnum wndType = QCommMsgItemWnd_Tpye);

signals:
	void commListChanged(int num);
	void signal_commListChangedAndSendInfo(QMap<QString,QString> infoMap);
	//void signal_test();
public slots:
	void onCurrentItemClicked(QListWidgetItem* item);
	//�������Ǹ���ť�Ļ�
	void onStartGroupBtnClicked();
public:
	QListWidgetItem* addMsgItem(const char* headUrl, const char* name, const char* msg,qint64 sesid, int64_t userid);
	void addContactsItem(const char* headUrl, const char* name,bool isNewFriend = false,int friendid = -1);
	

	QVBoxLayout* m_vLayout;
	QHBoxLayout* m_hLayout;

	//������
	QLineEdit* m_searchEdit;
	//��ʼ���Ǹ���ť
	QPushButton* m_startGroupBtn;

	QSelectWnd1* m_selectWnd1;

	QListWidget* m_listWidget;
	//��������
	QCommListWndEnum m_WndType;
};