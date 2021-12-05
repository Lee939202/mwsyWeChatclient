
#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "QLoginAndRegWnd.h"
#include "QWSClientMgr.h"
#include "QDealNewFriendsApplyWnd.h"

int main(int argc, char** argv)
{

	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);

	QApplication app(argc, argv);
	QWSClientMgr::getSingletonInstance();
	app.setWindowIcon(QIcon("./img/wechat.ico"));

	//µÇÂ¼´°¿Ú
	QLoginAndRegWnd lgw;
	lgw.show();

	app.exec();
}