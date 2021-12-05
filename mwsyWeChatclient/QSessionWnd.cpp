#include <QDateTime>

#include <QSplitter>
#include <QListWidget>
#include <QScrollBar>

#include "QSessionWnd.h"
#include "QChatMsgWnd.h"
#include "QSessionTopWnd.h"

#include "QSimpleSplit.h"
#include "QMainWnd.h"
#include "QWSClientMgr.h"
#include "json/CJsonObject.hpp"
#include "QMessageBox.h"


QSessionWnd::QSessionWnd(QWidget* p /*= nullptr*/)
	: QWidget(p)
{

	setMinimumSize(640,600);
	setWindowTitle("�Ự����");


	m_vLayout = new QVBoxLayout(this);
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	m_vLayout->setSpacing(0);

	m_sesTopWnd = new QSessionTopWnd(this);
	
	m_MsgWndList = new QListWidget(this);
	m_MsgWndList->setMinimumWidth(this->width());
	m_sendTextEdit = new QTextEdit(this);
	m_sendTextEdit->setStyleSheet("border:0px");

	m_sendTextBtn = new QPushButton(this);
	m_sendTextBtn->setFixedSize(70, 30);
	m_sendTextBtn->setText("����(S)");

	
	m_vLayout->addWidget(m_sesTopWnd);

	{
		/*��ӷָ��ߵ�ʾ������*/
		QSimpleSplit* sp = new QSimpleSplit(this);
		m_vLayout->addWidget(sp);
	}

	m_vLayout->addWidget(m_MsgWndList,2);

	{
		/*��ӷָ��ߵ�ʾ������*/
		QSimpleSplit* sp = new QSimpleSplit(this);
		m_vLayout->addWidget(sp);
	}

	m_vLayout->addWidget(m_sendTextEdit,1);

	//{
		/*��ӷָ��ߵ�ʾ������*/
	//	QSimpleSplit* sp = new QSimpleSplit(this);
	//	m_vLayout->addWidget(sp);
	//}

	m_hLayout = new QHBoxLayout();
	m_hLayout->addStretch();
	m_hLayout->addWidget(m_sendTextBtn);
	//m_hLayout->addSpacing(30);
	m_hLayout->setContentsMargins(0, 0, 15,10);
	m_vLayout->addLayout(m_hLayout);


	//m_MsgWndList->setStyleSheet("border-left:0px;border-top:1px solid gray;");
	//m_sendTextEdit->setStyleSheet("border-left:0px;border-top:1px solid gray;");

	//���ûỰ���ڴ�С
	//m_sendTextBtn->setGeometry(350, 550, 80, 20); 

	m_MsgWndList->setSelectionMode(QAbstractItemView::NoSelection);
	m_MsgWndList->setStyleSheet("border:opx");
	m_MsgWndList->setFocusPolicy(Qt::NoFocus);
	//��ť���ʱ������Ϣ
	connect(m_sendTextBtn, SIGNAL(clicked()), this, SLOT(onSendTextBtnClick()));
	//connect(m_MsgWndList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onMsgListItemClicked(QListWidgetItem*)));
	m_sendTextBtn->setStyleSheet("background-color:#1aad19;border-style: none;");

	setAttribute(Qt::WA_StyledBackground);
	setStyleSheet("background-color:white;border:0px");


	////���ù���������ʽ
	m_MsgWndList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,0%);"
		"margin:0px,0px,0px,0px;"
		"padding-top:9px;"
		"padding-bottom:9px;"
		"}"
		"QScrollBar::handle:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,25%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::handle:vertical:hover"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,50%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::add-line:vertical"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/3.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/1.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-line:vertical:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/4.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:vertical:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/2.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
		"{"
		"background:rgba(0,0,0,10%);"
		"border-radius:4px;"
		"}"
	);

	m_MsgWndList->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,0%);"
		"margin:0px,0px,0px,0px;"
		"padding-top:9px;"
		"padding-bottom:9px;"
		"}"
		"QScrollBar::handle:horizontal"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,25%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::handle:horizontal:hover"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,50%);"
		" border-radius:4px;"
		"min-height:20;"
		"}"
		"QScrollBar::add-line:horizontal"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/3.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:horizontal"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/1.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-line:horizontal:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/4.png);"
		"subcontrol-position:bottom;"
		"}"
		"QScrollBar::sub-line:horizontal:hover"
		"{"
		"height:9px;width:8px;"
		"border-image:url(:/images/a/2.png);"
		"subcontrol-position:top;"
		"}"
		"QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal"
		"{"
		"background:rgba(0,0,0,10%);"
		"border-radius:4px;"
		"}"
	);
}

void QSessionWnd::onSendTextBtnClick()
{
	QString msgText = m_sendTextEdit->toPlainText();
	
	//��������κ����ݲ�������
	if (msgText == "") {
		return;
	}

	m_sendTextEdit->setText("");

	neb::CJsonObject json;
	json.Add("sendid", QMainWnd::getSinletonInstance()->m_userid);
	json.Add("recvid",m_recvId);
	json.Add("sesid",m_sesId);
	json.Add("msgtext", msgText.toStdString().c_str());
	json.Add("msgtype", 0);

	QWSClientMgr::getSingletonInstance()->request("cs_msg_sendmsg", json, [this,msgText](neb::CJsonObject& msg)
		{
			//QMessageBox::information(nullptr, "info", msg.ToString().c_str());
			//��Զ�˷�����Ϣ
			QString time = QString::number(QDateTime::currentDateTime().toTime_t());
			QChatMsgWnd* msgWnd = new QChatMsgWnd(m_MsgWndList, QMainWnd::getSinletonInstance()->m_userid,m_recvId);
			QListWidgetItem* msgItem = new QListWidgetItem(m_MsgWndList);
			msgWnd->setFixedWidth(this->width());
			QSize msgSize = msgWnd->fontRect(msgText);
			msgItem->setSizeHint(msgSize);
			//��������Ϣ��������Ӧ��
			msgWnd->setText(msgText, time,msgSize, QChatMsgWnd::ChatMsg_Owner);
			//�������봰��
			m_MsgWndList->setItemWidget(msgItem, msgWnd);
		});
}