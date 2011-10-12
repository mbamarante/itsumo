#include "choosedialogimpl.h"
#include <qlabel.h>


ChooseDialogImpl::ChooseDialogImpl(QString label, QValueVector<QString>& list)
	: ChooseDialog()
{

	chooseLabel->setText(label);

	QValueVector<QString>::iterator it;

	for (it=list.begin(); it!=list.end(); it++)
	{
		chooseCombo->insertItem(*it);
	}
}


ChooseDialogImpl::~ChooseDialogImpl()
{
}

void ChooseDialogImpl::ok()
{
	QString item = chooseCombo->currentText();

	emit selected(item);

	accept();
}

