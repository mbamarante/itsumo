/****************************************************************************
** Form interface generated from reading ui file 'routedialog.ui'
**
** Created: Wed Oct 12 20:18:54 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ROUTEDIALOG_H
#define ROUTEDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class QRadioButton;
class QPushButton;
class QFrame;
class QGroupBox;
class QLabel;
class QComboBox;
class QTable;

class RouteDialog : public QDialog
{
    Q_OBJECT

public:
    RouteDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~RouteDialog();

    QButtonGroup* buttonGroup1;
    QRadioButton* radioRandom;
    QRadioButton* radioDefined;
    QPushButton* buttonOk;
    QFrame* frameNet;
    QGroupBox* groupBox4;
    QLabel* textLabel1;
    QComboBox* comboFrom;
    QLabel* textLabel2;
    QComboBox* comboTo;
    QPushButton* addButton;
    QTable* tableRoute;
    QPushButton* removeButton;

public slots:
    virtual void ok_();
    virtual void fromChanged_(const QString& from);
    virtual void remove_();
    virtual void add_();
    virtual void radioClicked_(int pos);
    virtual void toChanged_(const QString& to);

protected:
    QGridLayout* RouteDialogLayout;
    QGridLayout* buttonGroup1Layout;
    QHBoxLayout* layout16;
    QSpacerItem* spacer10;
    QGridLayout* groupBox4Layout;
    QVBoxLayout* layout14;
    QVBoxLayout* layout15;
    QHBoxLayout* layout12;
    QSpacerItem* spacer8;
    QHBoxLayout* layout11;
    QSpacerItem* spacer9;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // ROUTEDIALOG_H
