#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget> //�����б�

#include "QSessionTopWnd.h"
#include "QSessionToolBar.h"

/*
* �Ự�Ĵ��� sesId����,�ұ���Ϣ����С����ѡ��Ҳ��һ��id��ͨ��id�����˴���Ϣ
* ���޸Ķ�Ӧ�Ĵ�����Ϣ
*/

class QSessionWnd : public QWidget 
{
	//�����Ϣӳ��֧��
	Q_OBJECT
public:
	//�Ự����
	QSessionWnd(QWidget* p = nullptr);
public slots:
	void onSendTextBtnClick();
	void slot_emoijClicked(QString str);
public:
	//���ڲ��ֹ���
	QVBoxLayout *m_vLayout;
	//�Ự���ϷŴ���
	QSessionTopWnd* m_sesTopWnd;
	//�����б���Ϣ
	QListWidget* m_MsgWndList;
	//
	QSessionToolBar* m_sesToolBar;
	//��Ҫ���͵�����
	QTextEdit* m_sendTextEdit;
	//���ڷ������ֵİ�ť
	QPushButton* m_sendTextBtn;
	//���ڿ���
	QHBoxLayout* m_hLayout;
	
	//�Ựid
	qint64 m_sesId;
	//�Ự����
	QString m_name;
	//����˷���
	int64_t m_recvId;
	//�Ƿ���Ⱥ�Ự
	bool m_isGroupSes = false;
};