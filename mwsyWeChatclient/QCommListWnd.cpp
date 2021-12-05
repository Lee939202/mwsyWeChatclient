#include "QCommListWnd.h"

#include "QCommMsgItemWnd.h"
#include "QCommContactItemWnd.h"
#include "QCustomListWidgetItem.h"
#include "QSimpleSplit.h"
#include "QSelectWnd1.h"
#include "QMainWnd.h"

#include <QDebug>

#include <QScrollBar>

QCommListWnd::QCommListWnd(QWidget* p /*= nullptr*/, QCommListWndEnum wndType/*QCommMsgItemWnd_Tpye*/)
	: QWidget(p),m_WndType(wndType)
{
	m_vLayout = new QVBoxLayout();
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	m_vLayout->setSpacing(0);
	setLayout(m_vLayout);
	setFixedWidth(250);
	
	m_hLayout = new QHBoxLayout(this);
	m_hLayout->setContentsMargins(0, 0, 0, 0);
	m_hLayout->setSpacing(0);
	m_searchEdit = new QLineEdit(this);
	m_startGroupBtn = new QPushButton(this);
	m_searchEdit->setFixedHeight(25);
	m_startGroupBtn->setFixedSize(25,25);
	m_startGroupBtn->setIcon(QPixmap("./img/startGroupBtn.png"));
	m_startGroupBtn->setIconSize(QSize(25, 25));

	m_hLayout->addSpacing(10);
	m_hLayout->addWidget(m_searchEdit);
	m_hLayout->addSpacing(10);
	m_hLayout->addWidget(m_startGroupBtn);
	m_hLayout->addSpacing(10);
	m_vLayout->addSpacing(25);
	m_vLayout->addLayout(m_hLayout);
	m_vLayout->addSpacing(15);

	{
		/*��ӷָ��ߵ�ʾ������*/
		QSimpleSplit* sp = new QSimpleSplit(this);
		m_vLayout->addWidget(sp);
	}
	
	m_listWidget = new QListWidget();
	m_listWidget->setFixedWidth(250);
	m_listWidget->setAttribute(Qt::WA_StyledBackground);
	m_listWidget->setWindowFlags(Qt::FramelessWindowHint);
	//m_listWidget->setStyleSheet("border-left:0px ;");
	m_vLayout->addWidget(m_listWidget);

	m_selectWnd1 = new QSelectWnd1(nullptr);
	m_selectWnd1->hide();

	setFixedWidth(250);
	//setObjectName("QCommListWnd");
	setStyleSheet("#QCommListWnd{border:0px}");
	setAttribute(Qt::WA_StyledBackground);
	setWindowFlags(Qt::FramelessWindowHint);


	m_listWidget->setStyleSheet("border:0px");
	connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onCurrentItemClicked(QListWidgetItem*)));
	connect(m_startGroupBtn, SIGNAL(clicked()), this, SLOT(onStartGroupBtnClicked()));

	////���ù���������ʽ
	m_listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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


	m_listWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
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

void QCommListWnd::onCurrentItemClicked(QListWidgetItem* item)
{
	QCustomListWidgetItem* pCustItem = dynamic_cast<QCustomListWidgetItem*>(item);
	qDebug() << "sesid == " << pCustItem->sesId();
	if (m_WndType == QCommContactItemWnd_Type) {

		QCommContactItemWnd* wnd = dynamic_cast<QCommContactItemWnd*>(m_listWidget->itemWidget(pCustItem));
		//�޸�ջ����Ϣ
		QMap<QString, QString> infoMap;
		infoMap["name"] = wnd->m_name->text();
		
		if (wnd->m_bNewFriend)
		{
			//
			//commListChanged(0);
			QMainWnd::getSinletonInstance()->m_dealNewFriendsApplyWnd->setFriendApplyList();
			//�л����ڵ��µ����Ѵ���
			QMainWnd::getSinletonInstance()->m_sLayout2->setCurrentIndex(1);

		}
		else
		{
			//commListChanged(2);
			QMainWnd::getSinletonInstance()->m_sLayout2->setCurrentIndex(0);
			infoMap["friendid"] = QString::number(wnd->m_friendId,10);
			emit signal_commListChangedAndSendInfo(infoMap);
		}

		//emit signal_commListChangedAndSendInfo(infoMap);
		//emit infoMap;
		//emit signal_commListChangedAndSendInfo(infoMap);
		//emit signal_test();
	}
	else {
		QCommMsgItemWnd* wnd = dynamic_cast<QCommMsgItemWnd*>(m_listWidget->itemWidget(pCustItem));
		qint64 sesid = wnd->m_sesId;

		//��ȡstacklayout�е�sesid��

		//����stackLayout�ж϶�Ӧ�Ĵ���sesid
		commListChanged(sesid);
	}
}

void QCommListWnd::onStartGroupBtnClicked()
{
	//qDebug() << "onStartGroupBtnClicked()";
	QRect rect = m_startGroupBtn->geometry();
	qDebug() << "rect" << rect << endl;
	qDebug() << " the pos of startGroupBtn: " << m_startGroupBtn->pos();
	QPoint gPoint = m_startGroupBtn->mapToGlobal(QPoint(0,0));
	
	/*ѡ��Ĵ���ֻ���������һ��*/
	//QWidget *qw = new QSelectWnd1(nullptr);
	QRect swRect = m_selectWnd1->geometry();
	swRect.setX(gPoint.x() - m_selectWnd1->width()+m_startGroupBtn->width());
	swRect.setY(gPoint.y() + m_startGroupBtn->height() + 5);
	m_selectWnd1->setGeometry(swRect);
	m_selectWnd1->show();
}

QListWidgetItem* QCommListWnd::addMsgItem(const char* headUrl, const char* name, const char*  msg,qint64 sesid,int64_t userid)
{

	QCommMsgItemWnd* pMsgItem = new QCommMsgItemWnd(m_listWidget,headUrl,name,msg,sesid,userid);
	QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);

	pMsgItem->setFixedWidth(this->width()-5);
	pListItem->setSizeHint(QSize(this->width()-5, 65));
	m_listWidget->setItemWidget(pListItem, pMsgItem);

	/*void (QListWidget:: * pfnItemClicked)(QListWidgetItem*) = &QListWidget::itemClicked;
	QObject::connect(m_listWidget, pfnItemClicked, [&](QListWidgetItem* item) {
		QCommMsgItemWnd* pw = dynamic_cast<QCommMsgItemWnd*> (m_listWidget->itemWidget(item));
		qDebug() << pw->m_msg->text();
		});*/

	return pListItem;
}

void QCommListWnd::addContactsItem(const char* headUrl, const char*  name, bool isNewFriend /*= false*/, int friendid/* = -1*/)
{
	QCommContactItemWnd* pMsgItem = new QCommContactItemWnd(m_listWidget,headUrl,name,isNewFriend,friendid);
	QListWidgetItem* pListItem = new QCustomListWidgetItem(m_listWidget);
	pMsgItem->setFixedWidth(this->width()-5);
	pListItem->setSizeHint(QSize(this->width()-5, 65));
	m_listWidget->setItemWidget(pListItem, pMsgItem);
}

