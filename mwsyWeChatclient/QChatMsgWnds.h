#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QLabel>
#include <QMovie>


class QChatMsgWnds : public QWidget 
{
	Q_OBJECT
public:
	//��Ϣ����
	enum  ChatMsgTypeEnum {
		ChatMsg_System = 10,
		ChatMsg_Time,
		ChatMsg_Owner,
		ChatMsg_Other,
	};


public:
	//���캯��
	QChatMsgWnds(QWidget* p = nullptr,int64_t sendid = 0, int64_t recvid = 0);
protected:
	//����
	void paintEvent(QPaintEvent* event);
public:
	//
};