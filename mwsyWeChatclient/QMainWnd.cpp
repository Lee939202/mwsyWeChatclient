#include "QMainWnd.h"
#include "QSimpleSplit.h"
#include "json/CJsonObject.hpp"
#include "QWSClientMgr.h"
#include <QApplication>
#include "QCommMsgItemWnd.h"
#include "QCommContactItemWnd.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include "QChatMsgWnd.h"
#include "QDataManager.h"

QMainWnd* QMainWnd::m_mainWnd = nullptr;

QMainWnd::QMainWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	m_hLayout = new QHBoxLayout(this);
	m_toolWnd = new QToolWnd(this);
	
	m_commMsgListWnd = new QCommListWnd(this,QCommListWnd::QCommListWndEnum::QCommMsgItemWnd_Tpye);
	m_commContactsListWnd = new QCommListWnd(this, QCommListWnd::QCommListWndEnum::QCommContactItemWnd_Type);

	m_hLayout->setContentsMargins(0, 0, 0, 0);
	m_hLayout->setSpacing(0);

	m_sLayout1 = new QStackedLayout();
	//��Ϣ����
	m_sLayout1->addWidget(m_commMsgListWnd);
	//��ϵ�˴���
	m_sLayout1->addWidget(m_commContactsListWnd);
	m_sLayout1->setContentsMargins(0, 0, 0, 0);

	m_hLayout->setSpacing(0);

	m_sLayout2 = new QStackedLayout();
	m_sLayout2->setContentsMargins(0, 0, 0, 0);

	{
		m_commContactInfo = new QCommContactInfo(this);
		m_sLayout2->addWidget(m_commContactInfo);
		connect(m_commContactInfo->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
		connect(m_commContactInfo->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
		connect(m_commContactInfo->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
	}

	//{
		m_dealNewFriendsApplyWnd = new QDealNewFriendsApplyWnd(this);
		//m_dealNewFriendsApplyWnd->addListItem("./img/head2.png","this is a name "," this is a msg!!!",0 );
		connect(m_dealNewFriendsApplyWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
		connect(m_dealNewFriendsApplyWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
		connect(m_dealNewFriendsApplyWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
		m_sLayout2->addWidget(m_dealNewFriendsApplyWnd);

	//}

	//
	//m_currentSesId = 1;
	//for (int i = 1; i <=10; i++)
	//{
	//	m_sessionWnd = new QSessionWnd();
	//	QString str = m_sessionWnd->m_sesTopWnd->m_titleLabel->text();
	//	str += QString("%1xxxx")).arg(i);
	//	m_sessionWnd->m_sesTopWnd->m_titleLabel->setText(str);
	//	//ģ��һ��sesid;
	//	m_sessionWnd->m_sesId = m_currentSesId++;
	//	connect(m_sessionWnd->m_sesTopWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
	//	connect(m_sessionWnd->m_sesTopWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
	//	connect(m_sessionWnd->m_sesTopWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
	//	m_sLayout2->addWidget(m_sessionWnd);
	//}

	

	//for (int i = 1; i <=10; i++){
	//	
	//	QString str1 = QString("%1����Ϣ����")).arg(i);
	//	QString str2 = QString("[2��]_С����: [С����] %1��...")).arg(i);
	//	m_commMsgListWnd->addMsgItem("./img/head1.png", str1.toLocal8Bit().data(), "[2��]_С����:[С����] 0��...",i);
	//}

	//for (int i = 0; i < 10; i++){
	//	//if (i == 0) {
	//	//	m_commContactsListWnd->addContactsItem("./img/head2.png", "�µ�����0��");
	//	//}
	//	//else {
	//	//	m_commContactsListWnd->addContactsItem("./img/head2.png", "�µ�����1��");
	//	//}

	//	QString str1 = QString("�µ�����%1��")).arg(i);
	//	m_commContactsListWnd->addContactsItem("./img/head2.png", str1.toLocal8Bit().data());
	//}

	m_commContactsListWnd->addContactsItem("./img/head2.png", "�µ�����",true);

	m_hLayout->addWidget(m_toolWnd);

	{
		/*��ӷָ��ߵ�ʾ������*/
		QSimpleSplit* sp = new QSimpleSplit(this,QSimpleSplit::QSimpleSplit_direction_h);
		m_hLayout->addWidget(sp);
	}

	m_hLayout->addLayout(m_sLayout1,1);


	{
		/*��ӷָ��ߵ�ʾ������*/
		QSimpleSplit* sp = new QSimpleSplit(this, QSimpleSplit::QSimpleSplit_direction_h);
		m_hLayout->addWidget(sp);
	}
	m_hLayout->setSpacing(0);
	m_hLayout->addLayout(m_sLayout2,2);

	m_hLayout->addStretch();

	setLayout(m_hLayout);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_StyledBackground);

	//
	connect(m_toolWnd, SIGNAL(signal_toolWndPageChanged(int)), this, SLOT(slot_toolWndPageChanged(int)));
	connect(m_commMsgListWnd, SIGNAL(commListChanged(int)), this, SLOT(slot_sesIdToIndex(int)));

	connect(m_commContactsListWnd, SIGNAL(signal_commListChangedAndSendInfo(QMap<QString, QString>)),
		m_commContactInfo, SLOT(slot_commListChangedAndSendInfo(QMap< QString, QString>)));

	//������Ͱ�ť����
	connect(m_commContactInfo, SIGNAL(signal_sendMsgBtnClick(QMap<QString, QString>)),
		this, SLOT(slot_createSesAndSelect(QMap<QString, QString>)));

	QWSClientMgr::getSingletonInstance()->regMsgCall("cs_msg_sendmsg",std::bind(&QMainWnd::cs_msg_sendmsg,this, std::placeholders::_1) );

	m_networkMgr = new QNetworkAccessManager();
	connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_replyFinished(QNetworkReply*)));

}

void QMainWnd::cs_msg_sendmsg(neb::CJsonObject& msg)
{
	//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
	//���Ȼ�ȡ��Ӧ�ĻỰid�� 
	int sesid = -1;
	if (!msg["data"].Get("sesid", sesid))
	{
		return;
	}
	std::string msgtext = "";
	if (!msg["data"].Get("msgtext", msgtext))
	{
		return;
	}

	int64_t sendid = -1;
	if (!msg["data"].Get("sendid", sendid))
	{
		return;
	}

	int64_t recvid = -1;
	if (!msg["data"].Get("sendid", recvid))
	{
		return;
	}

	//���Ҷ�Ӧ�ĻỰ
	QSessionWnd* ses = nullptr;
	int count = m_sLayout2->count();
	for (int i = 0; i < count; i++) {

		if (i <= 1) {
			continue;
		}

		QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
		QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
		if (sesWnd->m_sesId == sesid) {
			ses = sesWnd;
			break;
		}
	}

	if (ses != nullptr)
	{
		//��Ự��Ƕ��һ�����ݣ�
		QString time = QString::number(QDateTime::currentDateTime().toTime_t());
		QChatMsgWnd* msgWnd = new QChatMsgWnd(ses->m_MsgWndList,sendid,recvid);
		QListWidgetItem* msgItem = new QListWidgetItem(ses->m_MsgWndList);
		msgWnd->setFixedWidth(640);
		QSize msgSize = msgWnd->fontRect(msgtext.c_str());
		msgItem->setSizeHint(msgSize);
		//��������Ϣ��������Ӧ��
		msgWnd->setText(msgtext.c_str(), time, msgSize, QChatMsgWnd::ChatMsg_Other);
		//�������봰��
		ses->m_MsgWndList->setItemWidget(msgItem, msgWnd);
		ses->m_MsgWndList->scrollToBottom();
	}
}

void QMainWnd::requestHeadImg()
{
	QString imgurl = QString("http://49.232.169.205:8080/UploadDemo/img/%1.png").arg(m_userid);
	m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void QMainWnd::requestFriendList()
{
	//
	neb::CJsonObject json;
	json.Add("ownerid", QMainWnd::getSinletonInstance()->m_userid);
	QWSClientMgr::getSingletonInstance()->request("cs_msg_get_friendslist", json, [this](neb::CJsonObject& msg)
		{
			//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
			if (!msg["data"].IsArray())
			{
				return;
			}

			for (int i = 0; i < msg["data"].GetArraySize(); i++)
			{
				neb::CJsonObject json1;
				if (!msg["data"].Get(i, json1))
				{
					continue;
				}

				int friendid = -1;
				if (!json1.Get("friendid", friendid))
				{
					continue;
				}


				std::string friendusername = "";
				if (!json1.Get("friendusername", friendusername))
				{
					continue;
				}

				bool has = false;
				int count = m_commContactsListWnd->m_listWidget->count();
				for (int i = 0; i < count; i++)
				{
					QListWidgetItem* pitem = m_commContactsListWnd->m_listWidget->item(i);
					QCommContactItemWnd* pWnd = dynamic_cast<QCommContactItemWnd*>(m_commContactsListWnd->m_listWidget->itemWidget(pitem));
					if (pWnd->m_friendId == friendid)
					{
						has = true;
					}
				}

				if (!has)
				{
					m_commContactsListWnd->addContactsItem("./img/head2.png", friendusername.c_str(), false, friendid);
				}

			}

		});
}

void QMainWnd::requestSessionList()
{
	//��Զ������Ự�б�
	neb::CJsonObject json;
	json.Add("ownerid", QMainWnd::getSinletonInstance()->getSinletonInstance()->m_userid);
	QWSClientMgr::getSingletonInstance()->request("cs_msg_get_sessionlist", json, [this](neb::CJsonObject& msg)
		{
			//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
			//��Ự�б������һЩ����
			if (!msg["data"].IsArray())
			{
				return;
			}

			for (int i = 0; i < msg["data"].GetArraySize(); i++)
			{
				neb::CJsonObject json1;
				if (!msg["data"].Get(i, json1))
				{
					continue;
				}

				int sesid = -1;
				if (!json1.Get("id", sesid))
				{
					continue;
				}

				std::string recentlymsg;
				if (!json1.Get("recentlymsg", recentlymsg))
				{
					continue;
				}

				std::string recvusername;
				if (!json1.Get("recvusername", recvusername))
				{
					continue;
				}

				int64_t sendid = -1;
				int64_t recvid = -1;

				//
				if (!json1.Get("sendid", sendid))
				{
					continue;
				}


				if (!json1.Get("recvid", recvid))
				{
					continue;
				}

				{
					bool has1 = false;
					int count = m_commMsgListWnd->m_listWidget->count();
					for (int i = 0; i < count; i++)
					{
						QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
						QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
						if (pWnd->m_sesId == sesid)
						{
							has1 = true;
							break;
						}
					}

					if (!has1)
					{
						//����m_commMsgListWnd
						m_commMsgListWnd->addMsgItem("./img/head1.png", recvusername.c_str(), recentlymsg.c_str(), sesid,recvid);
					}
				}

				{
					bool has2 = false;

					int count = m_sLayout2->count();
					for (int i = 0; i < count; i++) {

						if (i <= 1) {
							continue;
						}

						QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
						QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
						if (sesWnd->m_sesId == sesid) {
							has2 = true;
							break;
						}
					}

					if (!has2)
					{
						QSessionWnd* sesWnd = new QSessionWnd();
						sesWnd->m_sesTopWnd->m_titleLabel->setText(recvusername.c_str());
						sesWnd->m_sesId = sesid;
						sesWnd->m_recvId = recvid;
						connect(sesWnd->m_sesTopWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
						connect(sesWnd->m_sesTopWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
						connect(sesWnd->m_sesTopWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
						m_sLayout2->addWidget(sesWnd);
					}

				}
			}

		});
}

void QMainWnd::closeWnd()
{
	qApp->quit();
}

void QMainWnd::minWnd()
{
	showMinimized();
}

void QMainWnd::maxWnd()
{
	showMaximized();
}

void QMainWnd::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bPress)
	{
		move(event->pos() - m_poPress + pos());
	}
}

void QMainWnd::mousePressEvent(QMouseEvent* event)
{
	m_bPress = true;
	m_poPress = event->pos();
}

void QMainWnd::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_bPress = false;
}

void QMainWnd::slot_sesIdToIndex(int sesid)
{
	int layoutId = 0;
	if (sesid != 0) {
		int count = m_sLayout2->count();
		for (int i = 0; i < count; i++) {
			
			if (i <= 1) {
				continue;
			}

			QLayoutItem* item = m_sLayout2->layout()->itemAt(i);
			QSessionWnd* sesWnd = dynamic_cast<QSessionWnd*>(item->widget());
			if (sesWnd->m_sesId == sesid) {
				layoutId = i;
				break;
			}
		}
	}
	if (sesid != 0) {
		m_lastSesId = layoutId;
	}
	//return layoutId;
	m_sLayout2->setCurrentIndex(layoutId);
}

void QMainWnd::slot_createSesAndSelect(QMap<QString, QString> infoMap)
{
	//ĳ�������ĳ����ҷ�����Ϣ��

	//��ߴ���һ���Ự��
	//��ȡinfo�е���Ϣ
	//QString str1 =  infoMap["name"];
	////����һ���Ự��
	//int sesid = m_currentSesId++;
	//QListWidgetItem *pItem = m_commMsgListWnd->addMsgItem("./img/head1.png", str1.toLocal8Bit().data(), "",sesid,-1);
	//m_commMsgListWnd->m_listWidget->setCurrentItem(pItem);
	////����һ���Ự����
	//{
	//	//int i = 12;
	//	m_sessionWnd = new QSessionWnd();
	//	QString str = m_sessionWnd->m_sesTopWnd->m_titleLabel->text();
	//	str += QString("%1xxxx").arg(sesid);
	//	m_sessionWnd->m_sesTopWnd->m_titleLabel->setText(str);
	//	
	//	//ģ��һ��sesid;
	//	m_sessionWnd->m_sesId = sesid;
	//	m_lastSesId = sesid;

	//	connect(m_sessionWnd->m_sesTopWnd->m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
	//	connect(m_sessionWnd->m_sesTopWnd->m_maxBtn, SIGNAL(clicked()), this, SLOT(maxWnd()));
	//	connect(m_sessionWnd->m_sesTopWnd->m_minBtn, SIGNAL(clicked()), this, SLOT(minWnd()));
	//	m_sLayout2->addWidget(m_sessionWnd);
	//}

	//m_toolWnd->m_msgBtn->click();
	//slot_sesIdToIndex(sesid);

	//int count = m_commMsgListWnd->m_listWidget->count();
	bool has = false;
	int sesid = -1;
	int count = m_commMsgListWnd->m_listWidget->count();
	for (int i = 0; i < count; i++)
	{
		//if (i == 0)
		//{
		//	continue;
		//}

		QListWidgetItem* pitem = m_commMsgListWnd->m_listWidget->item(i);
		QCommMsgItemWnd* pWnd = dynamic_cast<QCommMsgItemWnd*>(m_commMsgListWnd->m_listWidget->itemWidget(pitem));
		QString namestr = pWnd->m_name->text();
		if (namestr == infoMap["name"])
		{
			sesid = pWnd->m_sesId;
			has = true;
		}
	}
	
	if (has)
	{
		m_lastSesId = sesid;
		m_toolWnd->m_msgBtn->click();
		slot_sesIdToIndex(sesid);
	}
	//if (!has)
	//{
	//	m_commContactsListWnd->addContactsItem("./img/head2.png", friendusername.c_str(), false, friendid);
	//}

}

void QMainWnd::slot_toolWndPageChanged(int page)
{
	m_sLayout1->setCurrentIndex(page);
	if (page == 0) {
		requestSessionList();
		if (m_lastSesId != -1) {
			slot_sesIdToIndex(m_lastSesId);
		}
		else {
			//���ñ���ͼƬ��
			m_commContactInfo->showBgPng();
		}
	}
	else {

		requestFriendList();

		m_commContactInfo->showBgPng();
		slot_sesIdToIndex(0);
	}
}

void QMainWnd::slot_replyFinished(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QPixmap pixmap;
		pixmap.loadFromData(reply->readAll());
		pixmap = pixmap.scaled(40, 40);
		m_toolWnd->m_headImg = pixmap;
		QDataManager::getInstance()->m_UserId2HeadImgMap[m_userid] = m_toolWnd->m_headImg;
	    m_toolWnd->m_headUrlLabel->setPixmap(m_toolWnd->m_headImg);
	}
	//else
	//{
		//����ͷ��ʧ�ܣ��������ͷ��
		
	//}
}

