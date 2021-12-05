#include "QToolWnd.h"
#include "QMainWnd.h"

QToolWnd::QToolWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	setFixedWidth(55);
	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("background-color:#2E2E2E;");
	//脱离父窗口的样式控制
	setAttribute(Qt::WA_StyledBackground);

	m_vBoxLayout = new QVBoxLayout(this);
	m_headUrlLabel = new QSimpleLabel("头像");
	m_headUrlLabel->setFixedSize(40, 40);
	//m_headUrlLabel->setStyleSheet("background-color:white;");
	
	m_msgBtn = new QPushButton(this);
	m_msgBtn->setFixedSize(30, 30);
	m_msgBtn->setIconSize(QSize(30, 30));
	m_msgBtn->setIcon(QPixmap("./img/msgBtnNormal.png"));

	m_contactsBtn = new QPushButton(this);
	m_contactsBtn->setFixedSize(30, 30);
	m_contactsBtn->setIconSize(QSize(30, 30));
	m_contactsBtn->setIcon(QPixmap("./img/contactsBtnNomal.png"));
	
	m_vBoxLayout->setAlignment(Qt::AlignCenter);
	m_vBoxLayout->setContentsMargins(0, 20, 0, 0);
	m_vBoxLayout->addWidget(m_headUrlLabel);
	m_vBoxLayout->addSpacing(15);
	m_vBoxLayout->addWidget(m_msgBtn);
	m_vBoxLayout->addSpacing(15);
	m_vBoxLayout->addWidget(m_contactsBtn);
	m_vBoxLayout->addStretch();

	setLayout(m_vBoxLayout);


	m_btnVct.push_back(m_msgBtn);
	m_btnVct.push_back(m_contactsBtn);
	m_selectIndex = 0;

	m_userInfoWnd = new QUserInfoWnd();
	m_userInfoWnd->hide();

	m_pictureToolWnd = new QPictureToolWnd();
	m_pictureToolWnd->hide();

	//m_networkMgr = new QNetworkAccessManager();
	//QString imgurl = QString("http://127.0.0.1:8080/UploadDemo/img/%1.png").arg(QMainWnd::getSinletonInstance()->m_userid);
	//connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_replyFinished(QNetworkReply*)));
	//m_networkMgr->get(QNetworkRequest(QUrl(imgurl)));

	connect(m_msgBtn, SIGNAL(clicked()), this, SLOT(slot_onClickMsgBtn()));
	connect(m_contactsBtn, SIGNAL(clicked()), this, SLOT(slot_onClickContactsBtn()));
	connect(m_headUrlLabel, SIGNAL(clicked()), this, SLOT(slot_onClickHeadUrlLabel()));
	connect(m_userInfoWnd->m_changeHeadImgBtn, SIGNAL(clicked()), this, SLOT(slot_onClickChangeHeadImgBtn()));
}

void QToolWnd::slot_onClickMsgBtn()
{
	m_selectIndex = 0;
	m_msgBtn->setIcon(QPixmap("./img/msgBtnClicked.png"));
	m_contactsBtn->setIcon(QPixmap("./img/contactsBtnNomal.png"));
	
	//发出信号
	signal_toolWndPageChanged(0);
}

void QToolWnd::slot_onClickContactsBtn()
{
	m_selectIndex = 0;
	m_msgBtn->setIcon(QPixmap("./img/msgBtnNormal.png"));
	m_contactsBtn->setIcon(QPixmap("./img/contactsBtnClicked.png"));
	signal_toolWndPageChanged(1);
}

void QToolWnd::slot_onClickHeadUrlLabel()
{
	if (m_userInfoWnd->isHidden())
	{
		QRect rect = m_headUrlLabel->geometry();
		QPoint gPoint = m_headUrlLabel->mapToGlobal(QPoint(0, 0));
		QRect swRect = m_userInfoWnd->geometry();
		swRect.setX(gPoint.x() + m_headUrlLabel->width() / 2);
		swRect.setY(gPoint.y() + m_headUrlLabel->height() / 2);
		m_userInfoWnd->setGeometry(swRect);
		m_userInfoWnd->show();
	}
}

void QToolWnd::slot_onClickChangeHeadImgBtn()
{
	m_pictureToolWnd->show();
}

//void QToolWnd::slot_replyFinished(QNetworkReply* reply)
//{
//	if (reply->error() == QNetworkReply::NoError)
//	{
//		QPixmap pixmap;
//		pixmap.loadFromData(reply->readAll());
//		pixmap = pixmap.scaled(40, 40);
//		//m_headUrl->setPixmap(pixmap);
//		m_headImg = pixmap;
//		m_headUrlLabel->setPixmap(m_headImg);
//	}
//}

