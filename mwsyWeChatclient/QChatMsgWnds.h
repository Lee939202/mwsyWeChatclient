#pragma once

//设置编码格式为utf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QLabel>
#include <QMovie>


class QChatMsgWnds : public QWidget 
{
	Q_OBJECT
public:
	//消息类型
	enum  ChatMsgTypeEnum {
		ChatMsg_System = 10,
		ChatMsg_Time,
		ChatMsg_Owner,
		ChatMsg_Other,
	};


public:
	//构造函数
	QChatMsgWnds(QWidget* p = nullptr,int64_t sendid = 0, int64_t recvid = 0);
protected:
	//绘制
	void paintEvent(QPaintEvent* event);
public:
	//
};