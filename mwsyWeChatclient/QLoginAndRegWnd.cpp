#include "QLoginAndRegWnd.h"
#include "QSimpleSplit.h"
#include <QMouseEvent>

#include <QDebug>
#include "QWSClientMgr.h"
#include <QMessageBox>
#include <QBuffer>
#include <QHttpMultiPart>
#include "QMainWnd.h"
#include "QDataManager.h"


#include "./json/CJsonObject.hpp"

QLoginAndRegWnd::QLoginAndRegWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{
	setFixedSize(430, 330);
	setContentsMargins(0, 0, 0, 0);
	m_vLayout = new QVBoxLayout();
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	m_vLayout->setSpacing(0);
	setLayout(m_vLayout);
	setWindowFlags(Qt::FramelessWindowHint);

	m_topWnd = new QWidget();
	m_topWnd->setFixedSize(430, 130);
	m_topWnd->setStyleSheet("background-color:#1aad19;border-style: none;");
	m_topWnd->setContentsMargins(0, 0, 0, 0);

	//顶部的窗口的布局
	m_vTopLayout = new QVBoxLayout();
	m_vTopLayout->setContentsMargins(0, 0, 0, 0);
	m_hTopLayout = new QHBoxLayout();

	m_topWnd->setLayout(m_vTopLayout);

	m_titleLabel = new QLabel();
	m_minBtn = new QPushButton();
	m_closeBtn = new QPushButton();

	m_titleLabel->setText("莫忘输赢/制作");
	m_minBtn->setIcon(QPixmap("./img/minWnd1.png"));
	m_minBtn->setIconSize(QSize(20, 20));
	m_minBtn->setFixedSize(20, 20);

	m_closeBtn->setIcon(QPixmap("./img/closeWnd1.png"));
	m_closeBtn->setIconSize(QSize(20, 20));
	m_closeBtn->setFixedSize(20, 20);

	m_hTopLayout->addWidget(m_titleLabel);
	m_hTopLayout->addWidget(m_minBtn);
	m_hTopLayout->addWidget(m_closeBtn);

	m_vTopLayout->addLayout(m_hTopLayout);
	
	{
		QSimpleSplit* sp = new QSimpleSplit();
		m_vTopLayout->addWidget(sp);
	}

	m_vTopLayout->addStretch();

	m_vLayout->addWidget(m_topWnd);

	{
		QSimpleSplit* sp = new QSimpleSplit();
		m_vLayout->addWidget(sp);
	}


	m_bottomWnd = new QWidget();
	m_bottomWnd->setFixedHeight(200);

	m_hBottomLayout1 = new QHBoxLayout();
	m_bottomWnd->setLayout(m_hBottomLayout1);

	m_vBottomLayout = new QVBoxLayout();
	m_accuntEdit = new QLineEdit();
	m_accuntEdit->setFixedSize(240, 40);
	m_pwdEdit = new QLineEdit();
	m_pwdEdit->setFixedSize(240, 40);

	m_accuntEdit->setPlaceholderText("请输入账号");
	m_pwdEdit->setPlaceholderText("请输入密码");

	m_accuntEdit->setText("myfirstAccount");
	m_pwdEdit->setText("mypassword");

	m_regOrLoginBtn = new QPushButton();
	m_regOrLoginBtn->setFixedSize(240, 40);
	m_regOrLoginBtn->setStyleSheet("background-color:#1aad19;border-style: none;");
	m_regOrLoginBtn->setText("登录");

	m_hBottomLayout2 = new QHBoxLayout();
	m_regOrLoginChx = new QCheckBox();
	m_remmerPwdChx = new QCheckBox();

	m_regOrLoginChx->setText("注册");
	m_remmerPwdChx->setText("找回密码");

	m_hBottomLayout2->addWidget(m_regOrLoginChx);
	m_hBottomLayout2->addWidget(m_remmerPwdChx);


	m_vBottomLayout->addWidget(m_accuntEdit);
	m_vBottomLayout->addWidget(m_pwdEdit);
	m_vBottomLayout->addLayout(m_hBottomLayout2);
	m_vBottomLayout->addWidget(m_regOrLoginBtn);

	m_hBottomLayout1->addStretch();
	m_hBottomLayout1->addLayout(m_vBottomLayout);
	m_hBottomLayout1->addStretch();


	m_vLayout->addWidget(m_bottomWnd);


	connect(m_minBtn, SIGNAL(clicked()), this, SLOT(slot_minWnd()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(slot_closeWnd()));
	connect(m_regOrLoginChx, SIGNAL(clicked(bool)), this, SLOT(slot_regOrLoginChx(bool)));
	connect(m_regOrLoginBtn, SIGNAL(clicked()), this, SLOT(slot_regOrLoginBtn()));
	connect(m_pwdEdit, SIGNAL(returnPressed()), this, SLOT(slot_regOrLoginBtn()));
}

void QLoginAndRegWnd::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bPress)
	{
		move(event->pos() - m_poPress + pos());
	}
}

void QLoginAndRegWnd::mousePressEvent(QMouseEvent* event)
{
	m_bPress = true;
	m_poPress = event->pos();
}

void QLoginAndRegWnd::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_bPress = false;
}

void QLoginAndRegWnd::slot_closeWnd()
{
	close();
}

void QLoginAndRegWnd::slot_minWnd()
{
	showMinimized();
}

void QLoginAndRegWnd::slot_regOrLoginChx(bool isSel /*= false*/)
{
	if (isSel) {
		m_bReg = true;
		m_regOrLoginBtn->setText("注册");
	}
	else {
		m_bReg = false;
		m_regOrLoginBtn->setText("登录");
	}
}

void QLoginAndRegWnd::slot_regOrLoginBtn()
{
	if (m_bReg) {
		//提供注册功能，点击注册按钮向远端服务发送消息
		neb::CJsonObject json;

		std::string username = m_accuntEdit->text().toStdString().c_str();//用户名
		std::string password = m_pwdEdit->text().toStdString().c_str();//密码
		std::string nickname = m_accuntEdit->text().toStdString().c_str();//角色名

		//性别标识
		int sex = 0;
		json.Add("username", username);
		json.Add("password", password);
		json.Add("nickname", nickname);
		json.Add("sex", sex);

		QWSClientMgr::getSingletonInstance()->request("cs_msg_register", json, [this](neb::CJsonObject& msg) {
				int state = 0;
				if (!msg.Get("state",state)) {
					return;
				}

				int userId = -1;
				if (!msg["data"].Get("userId", userId))
				{
					return;
				}

				std::string infoStr = "注册失败";
				if (state == 0) {
					infoStr = "注册成功";
				}

				//QByteArray byteArr;
				//QBuffer buffer(&byteArr);
				//buffer.open(QIODevice::WriteOnly);
				//m_HeadImg.save(&buffer, "png");
				//QByteArray byteArr2 = byteArr.toBase64();
				//QString headimgdata(byteArr2);


				QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
				QNetworkRequest request;
				request.setUrl(QUrl("http://49.232.169.205:8080/UploadDemo/UploadServlet"));
				QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
				QHttpPart part;
				part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"headimg\";filename=\"%1.png\"").arg(userId));
				part.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
				QFile* file = new QFile("./img/default.png");
				file->open(QFile::ReadOnly);
				part.setBodyDevice(file);
				file->setParent(multiPart);
				multiPart->append(part);
				QNetworkReply* reply = pManager->post(request, multiPart);


				QMessageBox::information(nullptr, "info", infoStr.c_str());
			});
		return;
	}
	 
	if(!m_bReg)
	{
		std::string username = m_accuntEdit->text().toStdString().c_str();
		std::string password = m_pwdEdit->text().toStdString().c_str();
		
		neb::CJsonObject json;
		json.Add("username", username);
		json.Add("password", password);

		QWSClientMgr::getSingletonInstance()->request("cs_msg_login", json, [this](neb::CJsonObject& msg){
			//
			int state = 0;
			if (!msg.Get("state", state)) 
			{
				//QMessageBox::information(nullptr, "login error", msg["data"].ToString().c_str());
				return;
			}

			int64 userid = 0;
			if (!msg["data"].Get("userId", userid)) 
			{
				//QMessageBox::information(nullptr,"login error", msg["data"].ToString().c_str());
				return;
			}

			std::string token;
			if (!msg["data"].Get("token", token)) 
			{
				//QMessageBox::information(nullptr, "login error", msg["data"].ToString().c_str());
				return;
			}

			//登录成功执行如下逻辑
			//if (m_mainWnd == nullptr) {
				//m_mainWnd = QMainWnd::getSinletonInstance();
			//}

			std::string username;
			if (!msg["data"].Get("username", username))
			{
				return;
			}

			qDebug() << msg.ToString().c_str();
			m_mainWnd = QMainWnd::getSinletonInstance();
			if (m_mainWnd != nullptr) {
				m_mainWnd->m_userid = userid;
				m_mainWnd->m_username = username.c_str();
				m_mainWnd->requestHeadImg();
				m_mainWnd->requestFriendList();
				m_mainWnd->requestSessionList();
				m_mainWnd->show();
				QDataManager::getInstance()->m_userid = userid;
				QDataManager::getInstance()->m_username = username.c_str();
				this->hide();
			}
		});
	}
}

