/****************************************************************************
** Form interface generated from reading ui file 'choosedialog.ui'
**
** Created: Wed Oct 12 13:53:01 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CHOOSEDIALOG_H
#define CHOOSEDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QComboBox;
class QPushButton;

class ChooseDialog : public QDialog
{
    Q_OBJECT

public:
    ChooseDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ChooseDialog();

    QLabel* chooseLabel;
    QComboBox* chooseCombo;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void ok();

protected:
    QGridLayout* ChooseDialogLayout;
    QVBoxLayout* layout15;
    QHBoxLayout* layout17;
    QSpacerItem* spacer9;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // CHOOSEDIALOG_H
