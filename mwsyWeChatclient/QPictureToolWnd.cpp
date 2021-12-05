#include "QPictureToolWnd.h"
#include <QFileDialog>
#include "./json/CJsonObject.hpp"
#include "QMainWnd.h"
#include <QBuffer>
#include <QByteArray>
#include "QWSClientMgr.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QNetworkReply>
#include "QDataManager.h"

QPictureToolWnd::QPictureToolWnd(QWidget* p /*= nullptr*/) : QWidget(p)
{
	setWindowFlags(Qt::FramelessWindowHint);

	m_vLayout = new QVBoxLayout();
	setLayout(m_vLayout);
	m_picLable = new QLabel();
	m_picLable->setFixedSize(350, 350);

	m_determineBtn = new QPushButton();
	m_cancelBtn = new QPushButton();
	m_uploadBtn = new QPushButton();

	m_determineBtn->setText("确定");
	m_cancelBtn->setText("取消");
	m_uploadBtn->setText("上传");

	m_hLayout = new QHBoxLayout();
	m_hLayout->addWidget(m_uploadBtn);
	m_hLayout->addWidget(m_determineBtn);
	m_hLayout->addWidget(m_cancelBtn);

	m_vLayout->addWidget(m_picLable);
	m_vLayout->addLayout(m_hLayout);

	connect(m_uploadBtn, SIGNAL(clicked()), this, SLOT(slot_uploadBtnClicked()));
	connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(slot_cancelBtnClicked()));
	connect(m_determineBtn, SIGNAL(clicked()), this, SLOT(slot_determineBtnClicked()));
}

void QPictureToolWnd::slot_uploadBtnClicked()
{
	QString filepath = QFileDialog::getOpenFileName(nullptr, "选择图片", ".", "*.png");
	if (filepath.trimmed().isEmpty() == false)
	{
		QPixmap picture;
		picture.load(filepath);
		m_filePath = filepath;
		picture = picture.scaled(m_picLable->width(), m_picLable->height());
		//picture = picture.scaled(40, 40);
		//m_HeadImg = picture;
		m_picLable->setPixmap(picture);
		picture = picture.scaled(40, 40);
		m_HeadImg = picture;
	}
}

void QPictureToolWnd::slot_cancelBtnClicked()
{
	hide();
}

void QPictureToolWnd::slot_determineBtnClicked()
{
	hide();
	QByteArray byteArr;
	QBuffer buffer(&byteArr);
	buffer.open(QIODevice::WriteOnly);
	m_HeadImg.save(&buffer,"png");
	QByteArray byteArr2 = byteArr.toBase64();
	QString headimgdata(byteArr2);


	QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
	QNetworkRequest request;
	request.setUrl(QUrl("http://127.0.0.1:8080/UploadDemo/UploadServlet"));
	QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);
	QHttpPart part;
	part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"headimg\";filename=\"%1.png\"").arg(QMainWnd::getSinletonInstance()->m_userid));
	part.setHeader(QNetworkRequest::ContentTypeHeader,"image/png");
	QFile* file = new QFile(m_filePath);
	file->open(QFile::ReadOnly);
	part.setBodyDevice(file);
	file->setParent(multiPart);
	multiPart->append(part);
	QNetworkReply* reply = pManager->post(request, multiPart);
	//
	connect(reply, &QNetworkReply::readyRead, this, [=]() {
			QByteArray all = reply->readAll();
			
			file->close();
			file->deleteLater();
			reply->deleteLater();
			
			QString str = QString(all);
			qDebug() << str << endl;

			neb::CJsonObject json0;
			json0.Add("headimg", "/xx/xx/x.png");
			std::string str0 = json0.ToString();

			neb::CJsonObject json1(str.toStdString());
			std::string headimgstr = "";
			if (!json1.Get("headimg", headimgstr))
			{
				return;
			}

			neb::CJsonObject json2;
			json2.Add("headimg", headimgstr);
			json2.Add("userid", QMainWnd::getSinletonInstance()->m_userid);
			//告诉远端服务器该玩家的头像数据
			QWSClientMgr::getSingletonInstance()->request("cs_msg_updateheadimg", json2, [this](neb::CJsonObject &msg) {
					qDebug() << msg.ToString().c_str() << endl;
					QMainWnd::getSinletonInstance()->m_toolWnd->m_headUrlLabel->setPixmap(QMainWnd::getSinletonInstance()->m_toolWnd->m_pictureToolWnd->m_HeadImg);
					QDataManager::getInstance()->m_UserId2HeadImgMap[QMainWnd::getSinletonInstance()->m_userid] = QMainWnd::getSinletonInstance()->m_toolWnd->m_pictureToolWnd->m_HeadImg;
				});
		});
}
