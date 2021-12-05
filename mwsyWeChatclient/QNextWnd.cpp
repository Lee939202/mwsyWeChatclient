#include "QNextWnd.h"
#include "QSimpleSplit.h"
#include "QApplyFriendWnd1.h"
#include "QApplyFriendWnd2.h"

#include <QMouseEvent>
#include <QMessageBox>

#include "QWSClientMgr.h"
#include "QMainWnd.h"


QNextWnd::QNextWnd(QWidget* p /*= nullptr*/, int64_t friendid /*= -1*/, QString username /*= ""*/)
	: QWidget(p),m_friendid(friendid),m_username(username)
{
	m_state = Ps_Next;

	m_vLayout = new QVBoxLayout(this);
	m_vLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_vLayout);
	setFixedSize(460, 360);
	setWindowFlags(Qt::FramelessWindowHint);
	
	m_hLayout1 = new QHBoxLayout();

	m_titleLabel = new QLabel();
	m_minBtn = new QPushButton();
	m_closeBtn = new QPushButton();

	m_titleLabel->setText("don't forget win or lost - ���Ӻ���");

	m_minBtn->setIcon(QPixmap("./img/minWnd.png"));
	m_minBtn->setIconSize(QSize(20, 20));
	m_minBtn->setFixedSize(20, 20);

	m_closeBtn->setIcon(QPixmap("./img/closeWnd.png"));
	m_closeBtn->setIconSize(QSize(20, 20));
	m_closeBtn->setFixedSize(20, 20);

	m_hLayout1->addWidget(m_titleLabel);
	m_hLayout1->addWidget(m_minBtn);
	m_hLayout1->addWidget(m_closeBtn);

	m_vLayout->addLayout(m_hLayout1);

	QSimpleSplit* sp1 = new QSimpleSplit();
	m_vLayout->addWidget(sp1);

	m_sLayout = new QStackedLayout(this);
	m_sLayout->setContentsMargins(0, 0, 0, 0);
	///*m_wnd1 = new QWidget();
	//m_wnd1->setFixedSize(460, 300);
	//m_wnd1->setStyleSheet("background-color:yellow;border:none");
	//m_wnd1->setContentsMargins(0, 0, 0, 0);
	//m_sLayout->addWidget(m_wnd1);
	/////��m_wnd1����һЩ����
	//QVBoxLayout* vLayout1 = new QVBoxLayout();
	//m_wnd1->setLayout(vLayout1);
	//QLabel* label1 = new QLabel();
	//label1->setText(" ------- wnd1 --------");
	//vLayout1->addWidget(label1);*/

	m_wnd1 = new QApplyFriendWnd1();
	m_sLayout->addWidget(m_wnd1);

	////m_wnd2 = new QWidget();
	////m_wnd2->setFixedSize(460,300);
	//////m_wnd2->setStyleSheet("border:red 2px");
	////m_wnd2->setStyleSheet("background-color:yellow;border:none");
	////m_wnd2->setContentsMargins(0, 0, 0, 0);
	////m_sLayout->addWidget(m_wnd2);

	///////��m_wnd1����һЩ����
	////QVBoxLayout* vLayout2 = new QVBoxLayout();
	////m_wnd2->setLayout(vLayout2);
	////QLabel* label2 = new QLabel();
	////label2->setText(" ------- wnd2 --------");
	////vLayout2->addWidget(label2);

	m_wnd2 = new QApplyFriendWnd2();
	m_sLayout->addWidget(m_wnd2);


	m_vLayout->addLayout(m_sLayout);

	QSimpleSplit* sp2 = new QSimpleSplit();
	m_vLayout->addWidget(sp2);

	m_hLayout2 = new QHBoxLayout();
	m_pushBtn = new QPushButton(this);
	m_pushBtn->setText("��һ��");
	//m_pushBtn->raise();
	
	m_hLayout2->addStretch();
	m_hLayout2->addWidget(m_pushBtn);
	m_vLayout->addLayout(m_hLayout2);


	connect(m_minBtn,   SIGNAL(clicked()), this, SLOT(minWnd()));
	connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(closeWnd()));
	connect(m_pushBtn,  SIGNAL(clicked()), this, SLOT(slot_pushBtnClick()));
	
}

void QNextWnd::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bPress)
	{
		move(event->pos() - m_poPress + pos());
	}
}

void QNextWnd::mousePressEvent(QMouseEvent* event)
{
	m_bPress = true;
	m_poPress = event->pos();
}

void QNextWnd::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	m_bPress = false;
}

void QNextWnd::slot_pushBtnClick()
{
	if (m_state == Ps_Next) {
		//����m_sLayoutΪ��2������
		m_sLayout->setCurrentIndex(1);
		//���ð�ť����Ϊ�ر�
		m_pushBtn->setText("�ر�");
		m_state = Ps_Close;

		//��Զ�˷������������Ӻ�������
		neb::CJsonObject json;
		json.Add("ownerid", QMainWnd::getSinletonInstance()->m_userid);
		json.Add("friendid",m_friendid);

		//��ȡ��֤����Ϣ
		auto pApplyWnd1 = dynamic_cast<QApplyFriendWnd1*>(m_wnd1);
		json.Add("applymsg",pApplyWnd1->m_inMsgEdit->toPlainText().toStdString().c_str());

		QWSClientMgr::getSingletonInstance()->request("cs_msg_apply_add_user", json, [](neb::CJsonObject& msg) 
			{
				int state = 0;
				if (!msg.Get("state", state)) 
				{
					return;
				}

				//���Ӻ�������
				//QMessageBox::information(nullptr, "cs_msg_apply_add_friend", msg.ToString().c_str());
			});


		return;
	}

	if (m_state == Ps_Close) {
		close();
	}
}

void QNextWnd::closeWnd()
{
	close();
}

void QNextWnd::minWnd()
{
	showMinimized();
}