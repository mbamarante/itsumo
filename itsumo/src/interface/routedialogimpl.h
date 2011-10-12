#ifndef ROUTEDIALOGIMPL_H
#define ROUTEDIALOGIMPL_H


#include "routedialog.h"
#include "filehandler.h"
#include <qvaluevector.h>

#define MAXX 515
#define MAXY 190

class QPoint;
class QCanvas;
class QCanvasView;

class RouteDialogImpl : public RouteDialog
{
    Q_OBJECT

public:
    RouteDialogImpl(int type = 0, NetHandler* nh = NULL, RouteList rl = 0);//type. 0=full, 1=seenet, 2=addtl
    ~RouteDialogImpl();

signals:

	void done(RouteList rl);

private slots:

	void show_();
	void ok_();
	void add_();
	void remove_();
	void toChanged_(const QString& to);
	void fromChanged_(const QString& from);
	void radioClicked_(int pos);

	bool exists_(const Route& r);

	QPoint realPoint_(QPoint p);

	void redrawNet_(const QString& from, const QString& to);

private:

	QPoint xRange_;
	QPoint yRange_;

	NetHandler* net_;
	RouteList routes_;

	int type_;

	QCanvas* c_;
	QCanvasView* cv_;

};

#endif // ROUTEDIALOGIMPL_H
