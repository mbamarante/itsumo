#include "routedialogimpl.h"
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qtable.h>
#include <qcanvas.h>
#include <qlayout.h>
#include <qmessagebox.h>

#include <iostream>
using std::cout;
using std::endl;

using namespace std;


RouteDialogImpl::RouteDialogImpl(int type, NetHandler* nh, RouteList rl)
	: RouteDialog()
{
	net_ = nh;
	routes_ = rl;
	type_ = type;

	xRange_ = net_->xRange();
	yRange_ = net_->yRange();

	show_();

	tableRoute->setColumnReadOnly(0, true);
	tableRoute->setColumnReadOnly(1, true);

	redrawNet_(comboFrom->currentText(), comboTo->currentText());
}

RouteDialogImpl::~RouteDialogImpl()
{
}


void RouteDialogImpl::show_()
{
	QString aux;

	NodeList list = net_->nodes();
	NodeList::iterator it;
	
	//instert nodes with TLs into comboBox from
	for(it = list.begin(); it!=list.end(); it++)
	{
		if (it->hastl)
			comboFrom->insertItem(it->name);
	}

	if (routes_.size() == 0)
	{
		radioRandom->setChecked(true);
		radioClicked_(0);
	}
	else
	{
		radioDefined->setChecked(true);
		radioClicked_(1);

		RouteList::iterator itR;

		for (itR = routes_.begin(); itR!=routes_.end(); itR++)
		{
			QPair<QString, QString> nds = net_->lanesetNodes(itR->laneset);

			tableRoute->insertRows(tableRoute->numRows());
			tableRoute->setText(tableRoute->numRows()-1, 0, nds.first);
			
			if (type_ != 2)
			{
				tableRoute->setText(tableRoute->numRows()-1, 1, nds.second);
			}
			else
				itR->laneset = net_->nodeId_(nds.first);
		}
	}

	fromChanged_(comboFrom->currentText());
}

void RouteDialogImpl::ok_()
{
	if ((radioRandom->isOn()) && (comboTo->isHidden() == false))
		routes_.clear();

	emit done(routes_);
	accept();
}

void RouteDialogImpl::toChanged_(const QString& to)
{
	redrawNet_(comboFrom->currentText(), to);
}

void RouteDialogImpl::fromChanged_(const QString& from)
{
	comboTo->clear();

	QValueVector<QString> next = net_->nextNodes(from);
	QValueVector<QString>::iterator itD;

	for(itD = next.begin(); itD!=next.end(); itD++)
	{
		comboTo->insertItem(*itD);
	}

	redrawNet_(from, comboTo->currentText());
}

void RouteDialogImpl::add_()
{
	QString aux;
	Route r;

	if (!comboTo->currentText().isEmpty())
	{
		
		r.laneset = net_->definedLaneset(comboFrom->currentText(), comboTo->currentText());
		
		//when selecting traffic lights without routes
		if (comboTo->isHidden())
			r.laneset = net_->nodeId_(comboFrom->currentText());
		
		if (!exists_(r))
		{
			tableRoute->insertRows(tableRoute->numRows());
			tableRoute->setText(tableRoute->numRows()-1, 0, comboFrom->currentText());
			tableRoute->setText(tableRoute->numRows()-1, 1, comboTo->currentText());
			
			routes_.push_back(r);
		}
	}

}

void RouteDialogImpl::remove_()
{
	int row = tableRoute->currentRow();

	Route r;
	r.laneset = net_->definedLaneset(tableRoute->text(row, 0), tableRoute->text(row, 1));

	if (type_ == 2)
		r.laneset = net_->nodeId_(tableRoute->text(row, 0));
	
	RouteList::iterator it;

	bool found = false;
	for (it = routes_.begin(); it!=routes_.end(); it++)
	{
		if (it->laneset == r.laneset)
		{
			found = true;
			break;
		}
	}
	
	if (found)
	{
		routes_.erase(it);
		tableRoute->removeRow(row);
	}
}

void RouteDialogImpl::radioClicked_(int pos)
{
  // just to avoid warnings
  pos = 0;

	if (radioRandom->isOn())
		groupBox4->setEnabled(false);
	else
		groupBox4->setEnabled(true);
}

bool RouteDialogImpl::exists_(const Route& r)
{
	RouteList::iterator it;

	for (it = routes_.begin(); it!=routes_.end(); it++)
	{
		if (it->laneset == r.laneset)
			return true;
	}
	return false;
}

QPoint RouteDialogImpl::realPoint_(QPoint p)
{
	QPoint ans;
	ans = p;

	double xFactor = (xRange_.y()-xRange_.x());
	xFactor = (MAXX)/xFactor;

	double yFactor = (yRange_.y()-yRange_.x());
	yFactor = (MAXY)/yFactor;
	
	ans.setX((int) ((ans.x() * xFactor) - (xRange_.x() * xFactor) + 5) );

	double y = (ans.y() * yFactor) - (yRange_.x() * yFactor) - 5;

	// Bruno (25jun2007): subtract from MAXY to vertically flip the map
	ans.setY((int) (MAXY-y) );

	return ans;
}

//needs optimization to use same canvas (memory problem)
void RouteDialogImpl::redrawNet_(const QString& from, const QString& to)
{
	c_ = new QCanvas(525, 205);
	cv_ = new QCanvasView( c_, frameNet );

	cv_->resize(530, 210);

	int actual = net_->definedLaneset(from, to);

	LanesetList lanesets;
	LanesetList::iterator itL;

	lanesets = net_->lanesets();

	for (itL=lanesets.begin(); itL!=lanesets.end(); itL++)
	{
		QCanvasLine *line = new QCanvasLine( c_ );
		QPoint begin = realPoint_(net_->nodeLocation(itL->begin));
		QPoint end = realPoint_(net_->nodeLocation(itL->end));

		if ( (itL->id == actual) && (type_ == 0) )
			line->setPen( QPen( Qt::green, 2 ) );
		else
			line->setPen( QPen( Qt::black, 1 ) );

		line->setPoints( begin.x(), begin.y(), end.x(), end.y() );

		line->show();
	}

	NodeList nodes;
	NodeList::iterator it;

	nodes = net_->nodes();

	for (it=nodes.begin(); it!=nodes.end(); it++)
	{
		QPoint p = realPoint_(net_->nodeLocation(it->id));

		QCanvasPolygonalItem* i;

		i = new QCanvasEllipse(8,8,c_);

		if (it->hastl)
			i = new QCanvasRectangle(p.x(), p.y(), 8, 8, c_);

		if (it->type == SOURCE)
			i->setBrush( Qt::blue );
		else if (it->type == SINK)
			i->setBrush( Qt::red );
		else
			i->setBrush( Qt::black );

		if ( (it->name == from) && (type_ != 1 ) )
			i->setBrush( Qt::green );

		i->move(p.x(), p.y());
		i->show();
	}

	c_->update();
	
	cv_->show();
}
