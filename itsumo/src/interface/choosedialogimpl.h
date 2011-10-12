#ifndef CHOOSEDIALOGIMPL_H
#define CHOOSEDIALOGIMPL_H


#include "choosedialog.h"
#include <qvaluevector.h>
#include <qcombobox.h>

class QString;


class ChooseDialogImpl : public ChooseDialog
{
    Q_OBJECT

public:
    ChooseDialogImpl(QString label, QValueVector<QString>& list);
    ~ChooseDialogImpl();

signals:

	void selected(QString item);

private slots:

	void ok();

};

#endif // CHOOSEDIALOGIMPL_H
