#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <map>
#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QNetworkReply>

#include "QToolWnd.h"
#include "QCommListWnd.h"
#include "QSessionWnd.h"
#include "QCommContactInfo.h"
#include "QDealNewFriendsApplyWnd.h"
#include "./json/CJsonObject.hpp"


/*
* �����ڴ���ֻ����һ����������������Ϊ��������
*/
class QMainWnd : public QWidget
{
	//�����Ϣӳ��֧��
	Q_OBJECT
private:
	//�Ự����
	QMainWnd(QWidget* p = nullptr);

public:
	static QMainWnd* getSinletonInstance() {
		if (m_mainWnd == nullptr)
		{
			m_mainWnd = new QMainWnd();
		}
		return m_mainWnd;
	}

	void cs_msg_sendmsg(neb::CJsonObject& json);
	
	void requestHeadImg();
	void requestFriendList();
	void requestSessionList();

public slots:
	void closeWnd();
	void minWnd();
	void maxWnd();

	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void slot_sesIdToIndex(int sesid);

	void slot_createSesAndSelect(QMap<QString, QString> infoMap);
	void slot_toolWndPageChanged(int page);
	void slot_replyFinished(QNetworkReply* reply);
public:
	QHBoxLayout* m_hLayout;
	QToolWnd* m_toolWnd;

	QCommListWnd* m_commMsgListWnd;
	QCommListWnd* m_commContactsListWnd;
	
	QCommContactInfo* m_commContactInfo;

	QDealNewFriendsApplyWnd* m_dealNewFriendsApplyWnd;

	QSessionWnd* m_sessionWnd;
	QStackedLayout *m_sLayout1;
	QStackedLayout* m_sLayout2;

	int m_lastSesId = -1;
	int m_lastContactId = -1;

	int m_currentSesId = 0;

	bool m_bPress = false;
	QPoint m_poPress;

	static QMainWnd* m_mainWnd;

	//�û���
	QString m_username;
	//�û�id
	int64_t m_userid;
	QNetworkAccessManager* m_networkMgr;
};
