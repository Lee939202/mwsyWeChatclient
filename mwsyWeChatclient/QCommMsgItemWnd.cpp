#include "QCommMsgItemWnd.h"
#include "QDataManager.h"
#include <string>

QCommMsgItemWnd::QCommMsgItemWnd(QWidget* p) 
	: QWidget(p)
{
	
}

QCommMsgItemWnd::QCommMsgItemWnd(QWidget* p,const char*  headUrl, const char* name, const char* msg, int64_t sesid,int64_t userid)
{
	m_sesId = sesid;
	m_userid = userid;

	m_vLayout = new QVBoxLayout();
	m_hLayout = new QHBoxLayout();
	m_name = new QLabel();
	m_msg = new QLabel();

	m_name->setText(name);
	m_msg->setText(msg);

	m_vLayout->addSpacing(10);
	m_vLayout->addWidget(m_name);
	m_vLayout->addWidget(m_msg);
	m_vLayout->addSpacing(10);

	m_headurl = new QLabel();
	m_headurl->setFixedSize(40, 40);
	m_headurl->setPixmap(QPixmap(headUrl));

	m_hLayout->addSpacing(10);
	m_hLayout->addWidget(m_headurl);
	m_hLayout->addSpacing(15);
	m_hLayout->addLayout(m_vLayout);

	//m_vLayout->setContentsMargins(0, 0, 0, 0);
	//m_vLayout->setSpacing(0);

	m_hLayout->setContentsMargins(0, 0, 0, 0);
	m_hLayout->setSpacing(0);

	setLayout(m_hLayout);
	setFixedHeight(65);

	//
	QString imgurl = QString("http://49.232.169.205:8080/UploadDemo/img/%1.png").arg(userid);
	m_networkMgr = new QNetworkAccessManager();
	connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(slot_replyFinished(QNetworkReply*)));
	m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));
}

void QCommMsgItemWnd::slot_replyFinished(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QPixmap pixmap;
		pixmap.loadFromData(reply->readAll());
		pixmap = pixmap.scaled(40, 40);
		m_headurl->setPixmap(pixmap);
		QDataManager::getInstance()->m_UserId2HeadImgMap[m_userid] = pixmap;
	}
}

