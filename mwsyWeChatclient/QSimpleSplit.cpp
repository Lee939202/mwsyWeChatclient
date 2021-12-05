#include "QSimpleSplit.h"

QSimpleSplit::QSimpleSplit(QWidget* p /*= nullptr*/, QSimpleSplit_direction d/* = QSimpleSplit_direction_v*/)
{
	setAttribute(Qt::WA_StyledBackground);
	//����һЩ��ʽ
	//�޽���
	setFocusPolicy(Qt::NoFocus);
	if (d == QSimpleSplit_direction_v) {
		//���ø߶�Ϊ2
		setFixedHeight(1);
	}
	else {
		setFixedWidth(1);
	}
	//���÷ָ��ߵ���ɫ
	setStyleSheet("background-color:#909090;border:none");
}

