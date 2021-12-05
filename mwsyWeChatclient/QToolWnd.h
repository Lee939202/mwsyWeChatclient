#pragma once

//���ñ����ʽΪutf-8
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QNetworkAccessManager>

#include "QSimpleLabel.h"
#include "QUserInfoWnd.h"
#include "QPictureToolWnd.h"
#include <QNetworkReply>

/*
* ���ұ߹���������
*/
class QToolWnd : public QWidget 
{
	//�����Ϣӳ��֧��
	Q_OBJECT
public:
	//����������
	QToolWnd(QWidget* p = nullptr);
signals:
	void signal_toolWndPageChanged(int num);
public slots:
	void slot_onClickMsgBtn();
	void slot_onClickContactsBtn();
	void slot_onClickHeadUrlLabel();
	void slot_onClickChangeHeadImgBtn();
	//void slot_replyFinished(QNetworkReply* reply);
public:
	//QMainWnd* m_mainWnd;
	QVBoxLayout* m_vBoxLayout;
	QPushButton *m_msgBtn;
	QPushButton* m_contactsBtn;
	QSimpleLabel* m_headUrlLabel;

	QVector<QPushButton*> m_btnVct;

	QUserInfoWnd* m_userInfoWnd;
	QPictureToolWnd* m_pictureToolWnd;

	QPixmap m_headImg;
	int m_selectIndex;

	//QNetworkAccessManager* m_networkMgr;
};