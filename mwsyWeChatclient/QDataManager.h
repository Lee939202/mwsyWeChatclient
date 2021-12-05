#pragma once

#include <QPixmap>

typedef std::map<int64_t, QPixmap> QUserId2HeadImgMap;

class QDataManager
{
private:
	QDataManager(){}
public:
	static QDataManager* getInstance();
	static void freeInstance();
private:
	static QDataManager* m_dataManager;
public:
	QUserId2HeadImgMap m_UserId2HeadImgMap;
	int64_t m_userid = -1;
};