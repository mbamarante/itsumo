/****************************************************************************
** Form implementation generated from reading ui file 'routedialog.ui'
**
** Created: Wed Oct 12 20:19:07 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "routedialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const unsigned char image0_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7a, 0x7a, 0xf4, 0x00, 0x00, 0x03,
    0xf1, 0x49, 0x44, 0x41, 0x54, 0x58, 0x85, 0xbd, 0x97, 0xc1, 0x8e, 0xe3,
    0x44, 0x10, 0x86, 0x3f, 0xb7, 0xdb, 0x76, 0x26, 0x99, 0x11, 0xa3, 0x65,
    0x0f, 0x70, 0x60, 0xb4, 0x12, 0x5c, 0xf6, 0xc8, 0x89, 0x1b, 0x57, 0x2e,
    0xdc, 0x39, 0x21, 0xf1, 0x16, 0x7b, 0x9c, 0xcd, 0xc3, 0x20, 0x24, 0xb8,
    0xf1, 0x10, 0x9c, 0x38, 0xf2, 0x0a, 0x2c, 0x2c, 0xa3, 0x19, 0xc8, 0x4c,
    0x62, 0xc7, 0x76, 0x77, 0x35, 0x87, 0x6a, 0x27, 0x76, 0x12, 0x27, 0x83,
    0x14, 0x51, 0x52, 0x4f, 0x7b, 0x1c, 0x77, 0xd7, 0xdf, 0x55, 0x7f, 0x55,
    0x57, 0x25, 0x40, 0x0a, 0x58, 0x20, 0x03, 0xf2, 0x38, 0xa7, 0x71, 0x24,
    0x9c, 0x4f, 0x02, 0xe0, 0xe3, 0x68, 0x81, 0x06, 0x68, 0x6d, 0x54, 0x5e,
    0x00, 0x53, 0xe0, 0x32, 0xce, 0x45, 0x0f, 0xc8, 0xb9, 0xa4, 0x53, 0x5c,
    0x03, 0x25, 0xb0, 0x04, 0xca, 0xee, 0xe4, 0x53, 0xe0, 0x8f, 0x33, 0x2a,
    0x3b, 0x25, 0x5f, 0x02, 0x77, 0xa0, 0xa7, 0xcf, 0xd1, 0x93, 0xf3, 0xc3,
    0x2f, 0xe0, 0x3d, 0x78, 0x07, 0x6d, 0x0b, 0xae, 0x05, 0xf1, 0x20, 0x02,
    0x21, 0x1c, 0xde, 0xc9, 0xa4, 0x90, 0xc6, 0x61, 0x73, 0xb0, 0x16, 0x52,
    0x0b, 0xc6, 0x40, 0x72, 0xc0, 0x81, 0xdf, 0x7c, 0x01, 0xc0, 0x4d, 0xfc,
    0xd7, 0xf5, 0x2d, 0x80, 0x73, 0xd0, 0x36, 0xb0, 0xae, 0xa0, 0x5a, 0x41,
    0xbd, 0x86, 0xb6, 0xd6, 0xf7, 0x22, 0xa8, 0x17, 0x7b, 0x92, 0x18, 0x55,
    0x96, 0xe7, 0x90, 0x17, 0x70, 0x31, 0x83, 0xe2, 0x42, 0x9f, 0xb3, 0x5c,
    0x41, 0x8c, 0xc8, 0xc7, 0xc0, 0x23, 0xb0, 0xb0, 0xa8, 0x9f, 0x0b, 0xd0,
    0x13, 0xaf, 0x4b, 0xf8, 0xed, 0x57, 0xf8, 0xeb, 0xdd, 0x49, 0x33, 0xee,
    0xc9, 0xcb, 0x8f, 0xe0, 0xf5, 0xe7, 0x5b, 0x70, 0x99, 0x65, 0x8c, 0xc6,
    0x2f, 0x50, 0xab, 0x17, 0x1d, 0x80, 0x0c, 0xa0, 0x69, 0xa0, 0x5c, 0xc1,
    0xfb, 0xdf, 0xe1, 0xf6, 0xf6, 0xf6, 0x3f, 0x03, 0x98, 0xcf, 0xe7, 0x7c,
    0xf2, 0x69, 0x74, 0x87, 0xd5, 0x79, 0x24, 0x8c, 0x2e, 0x81, 0x09, 0x3d,
    0x8c, 0x29, 0x80, 0x6b, 0x60, 0xbd, 0x42, 0x4d, 0x1d, 0xcd, 0x1d, 0x82,
    0xfe, 0x91, 0x10, 0x10, 0x09, 0x84, 0x00, 0x21, 0x84, 0xcd, 0xfb, 0xbe,
    0xf8, 0x16, 0x1e, 0x1f, 0xc0, 0x46, 0x4e, 0x90, 0xe8, 0x73, 0x62, 0x22,
    0x90, 0x2d, 0x9a, 0x4d, 0x94, 0xd9, 0xfe, 0x06, 0xce, 0x41, 0x5d, 0x6f,
    0xf5, 0x87, 0x00, 0x22, 0x01, 0xef, 0x05, 0xd7, 0x7a, 0x9a, 0xc6, 0xd1,
    0xb6, 0x82, 0x77, 0x82, 0x88, 0xec, 0x81, 0xa8, 0x2a, 0xb8, 0xbf, 0x53,
    0x45, 0x01, 0xe5, 0x4d, 0x5e, 0xa8, 0x16, 0x6b, 0x15, 0xc8, 0xae, 0x0c,
    0x00, 0x88, 0x53, 0x1e, 0x6c, 0x10, 0x48, 0xc0, 0x3b, 0xa1, 0x5e, 0x3b,
    0xaa, 0xaa, 0x61, 0xf5, 0x54, 0x53, 0x55, 0x0d, 0xf5, 0xda, 0xe1, 0x9c,
    0xe0, 0x45, 0x06, 0xc4, 0x5c, 0x3d, 0xc2, 0xfd, 0x7b, 0x40, 0x20, 0x88,
    0x5a, 0x64, 0x7a, 0xa9, 0xc4, 0x2c, 0x26, 0x60, 0xb3, 0x13, 0x00, 0xbc,
    0x68, 0x18, 0xaa, 0x99, 0xc1, 0x4b, 0xa0, 0x6d, 0x3c, 0x65, 0xd9, 0xf0,
    0xe7, 0xbb, 0x05, 0x4d, 0xed, 0x06, 0x8b, 0xd3, 0x1d, 0x9a, 0x27, 0x89,
    0xae, 0x73, 0x5e, 0x23, 0x29, 0x31, 0x6a, 0x95, 0xeb, 0x0f, 0xbb, 0x0f,
    0x4e, 0x00, 0x08, 0x11, 0xf9, 0x06, 0x80, 0x0f, 0xd4, 0xb5, 0x63, 0xf9,
    0x54, 0x53, 0xd7, 0x8e, 0xaf, 0xbf, 0xfa, 0x6e, 0x7f, 0x87, 0x9e, 0xcc,
    0x26, 0xaf, 0xb8, 0xfa, 0x00, 0x66, 0x57, 0x1a, 0x92, 0x79, 0x01, 0x3f,
    0xfe, 0x3c, 0x67, 0xb9, 0x80, 0xd4, 0x44, 0x5e, 0x1c, 0x03, 0x20, 0x3d,
    0xe5, 0xea, 0x7f, 0x68, 0x1b, 0xa1, 0x5c, 0x35, 0x10, 0xf6, 0xd2, 0xc0,
    0xbe, 0x84, 0xe1, 0xfa, 0x10, 0xf4, 0x40, 0xe5, 0x52, 0x5d, 0x90, 0x4d,
    0x4e, 0x59, 0x20, 0x2e, 0xe8, 0x38, 0x10, 0x24, 0xe0, 0x9c, 0xd0, 0xac,
    0xdd, 0x20, 0x32, 0x4e, 0xe2, 0xe8, 0xd6, 0xc7, 0x51, 0x57, 0x9a, 0xd0,
    0x7c, 0xbb, 0xff, 0xed, 0xc0, 0x89, 0x41, 0xf6, 0xd1, 0x8b, 0x17, 0xda,
    0x56, 0x46, 0x53, 0xf1, 0xde, 0x01, 0x76, 0x94, 0x8b, 0x68, 0x7e, 0x69,
    0x9d, 0xf2, 0xeb, 0x38, 0x80, 0xde, 0x1c, 0x00, 0x21, 0x20, 0x02, 0x5e,
    0x64, 0x1b, 0x9a, 0x47, 0x46, 0x7f, 0x9f, 0xd0, 0xfb, 0xc1, 0xbb, 0x78,
    0xa7, 0x1c, 0x00, 0x60, 0x77, 0x5f, 0x24, 0x66, 0x87, 0x07, 0x31, 0xe9,
    0x84, 0x3e, 0xc2, 0x31, 0x0b, 0xa0, 0x91, 0xd0, 0x91, 0x5d, 0xd7, 0xab,
    0x15, 0xba, 0x71, 0xd4, 0x02, 0x26, 0xd1, 0x84, 0x71, 0xf2, 0x88, 0x47,
    0x10, 0x0c, 0x6e, 0xc1, 0x1e, 0xa7, 0xc2, 0xc8, 0x8d, 0x3a, 0xb0, 0x40,
    0x62, 0x34, 0x59, 0x0c, 0x78, 0xc0, 0xd0, 0xaf, 0xa7, 0xc4, 0xa4, 0xdb,
    0x8c, 0xb7, 0xc9, 0xa6, 0x61, 0xdc, 0x02, 0x03, 0x00, 0xc6, 0x40, 0x3e,
    0x81, 0xeb, 0x97, 0xf0, 0xf6, 0xed, 0x1c, 0xef, 0xf5, 0x4a, 0x2e, 0x97,
    0x6a, 0x9d, 0xcb, 0x8b, 0x57, 0x47, 0x95, 0x17, 0x13, 0xb5, 0xa0, 0x31,
    0x68, 0x3a, 0x8e, 0x8a, 0x37, 0xe4, 0x7e, 0x16, 0x80, 0x1c, 0x3e, 0x7b,
    0x0d, 0xe5, 0x0d, 0x2c, 0x17, 0xb0, 0x78, 0x80, 0xbf, 0xef, 0xf5, 0x38,
    0xb3, 0xab, 0xe1, 0xe2, 0x24, 0xd1, 0xd1, 0x15, 0x25, 0x36, 0xd3, 0x61,
    0xba, 0x5b, 0x70, 0xe7, 0x52, 0x3b, 0xe9, 0x02, 0x13, 0x5d, 0x30, 0x99,
    0x6e, 0x37, 0xed, 0x48, 0x29, 0x02, 0xd3, 0xd9, 0xce, 0xea, 0x64, 0x1f,
    0x44, 0x9a, 0x0e, 0x79, 0x10, 0xfa, 0xbc, 0x38, 0x05, 0x20, 0xa0, 0x1b,
    0xd9, 0x08, 0x26, 0x49, 0x63, 0x29, 0xeb, 0x35, 0x91, 0x14, 0x17, 0x07,
    0x00, 0xec, 0x02, 0x89, 0xca, 0xba, 0xa8, 0x91, 0x00, 0x26, 0xde, 0x84,
    0x87, 0x28, 0xb4, 0x1f, 0x86, 0x49, 0xac, 0xe9, 0xd2, 0xb8, 0xd1, 0x4c,
    0xe3, 0x78, 0xf5, 0xa4, 0x65, 0xd6, 0x41, 0x49, 0x06, 0xd3, 0xe6, 0xa1,
    0x03, 0x91, 0xe7, 0xd1, 0x9a, 0xcf, 0xb9, 0x8c, 0x06, 0xfb, 0x76, 0x2e,
    0xb9, 0xd0, 0x52, 0xed, 0xfb, 0x9f, 0xe6, 0x23, 0x08, 0xc6, 0x45, 0x44,
    0xaf, 0xe4, 0x2c, 0x7b, 0x06, 0x00, 0xd9, 0xb1, 0x51, 0xe8, 0x40, 0xe4,
    0x70, 0x75, 0xad, 0x85, 0xea, 0xba, 0x82, 0xa6, 0x8e, 0x15, 0xf3, 0x01,
    0x56, 0xc3, 0x96, 0x90, 0x59, 0x2c, 0x54, 0x27, 0x53, 0x48, 0xb3, 0xf1,
    0x82, 0xa4, 0xeb, 0x58, 0x0e, 0x86, 0x89, 0xe9, 0x8a, 0xcb, 0x49, 0x8c,
    0x92, 0x42, 0x95, 0xf7, 0xeb, 0x86, 0xbe, 0x74, 0x3c, 0x30, 0xb1, 0x2e,
    0xcc, 0x72, 0x05, 0x92, 0xda, 0x41, 0x39, 0x50, 0xa3, 0x4d, 0x8a, 0xb7,
    0x6c, 0x3b, 0x96, 0xf1, 0xda, 0xdf, 0x42, 0xd6, 0x95, 0xe0, 0xc5, 0x30,
    0xb6, 0x0f, 0x4a, 0xa2, 0x79, 0x23, 0x31, 0xdb, 0xe8, 0x30, 0xc3, 0x46,
    0x6f, 0x09, 0xac, 0x89, 0xad, 0x99, 0x8f, 0x88, 0x78, 0xf3, 0xed, 0xc8,
    0x86, 0xe7, 0x97, 0x87, 0x08, 0xa2, 0xb6, 0xe8, 0xe9, 0x4b, 0xb4, 0x5d,
    0xba, 0x41, 0x9b, 0x86, 0x4d, 0xdd, 0x7e, 0x46, 0xa5, 0x75, 0x54, 0xfa,
    0x80, 0xb6, 0x81, 0xff, 0x10, 0x7b, 0xc3, 0x26, 0xfe, 0x70, 0x17, 0x3f,
    0x7c, 0xa4, 0x57, 0xb7, 0x9f, 0x11, 0x40, 0x8b, 0x9a, 0x7d, 0x19, 0x95,
    0xdf, 0x01, 0xcb, 0xbe, 0x05, 0x00, 0x1c, 0xb0, 0xe0, 0x7f, 0xec, 0x8e,
    0xff, 0x05, 0xec, 0xb1, 0x2e, 0x55, 0x4a, 0xe4, 0x73, 0x3a, 0x00, 0x00,
    0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};


/*
 *  Constructs a RouteDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
RouteDialog::RouteDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QImage img;
    img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
    image0 = img;
    if ( !name )
	setName( "RouteDialog" );
    setIcon( image0 );
    RouteDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "RouteDialogLayout"); 

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setMaximumSize( QSize( 32767, 50 ) );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QGridLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    radioRandom = new QRadioButton( buttonGroup1, "radioRandom" );

    buttonGroup1Layout->addWidget( radioRandom, 0, 0 );

    radioDefined = new QRadioButton( buttonGroup1, "radioDefined" );

    buttonGroup1Layout->addWidget( radioDefined, 0, 1 );

    RouteDialogLayout->addWidget( buttonGroup1, 0, 0 );

    layout16 = new QHBoxLayout( 0, 0, 6, "layout16"); 
    spacer10 = new QSpacerItem( 341, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout16->addItem( spacer10 );

    buttonOk = new QPushButton( this, "buttonOk" );
    layout16->addWidget( buttonOk );

    RouteDialogLayout->addLayout( layout16, 3, 0 );

    frameNet = new QFrame( this, "frameNet" );
    frameNet->setMinimumSize( QSize( 0, 210 ) );
    frameNet->setMaximumSize( QSize( 550, 32767 ) );
    frameNet->setFrameShape( QFrame::StyledPanel );
    frameNet->setFrameShadow( QFrame::Raised );

    RouteDialogLayout->addWidget( frameNet, 1, 0 );

    groupBox4 = new QGroupBox( this, "groupBox4" );
    groupBox4->setColumnLayout(0, Qt::Vertical );
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QGridLayout( groupBox4->layout() );
    groupBox4Layout->setAlignment( Qt::AlignTop );

    layout14 = new QVBoxLayout( 0, 0, 6, "layout14"); 

    textLabel1 = new QLabel( groupBox4, "textLabel1" );
    textLabel1->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1->setFont( textLabel1_font ); 
    layout14->addWidget( textLabel1 );

    comboFrom = new QComboBox( FALSE, groupBox4, "comboFrom" );
    layout14->addWidget( comboFrom );

    groupBox4Layout->addLayout( layout14, 0, 0 );

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 

    textLabel2 = new QLabel( groupBox4, "textLabel2" );
    textLabel2->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont textLabel2_font(  textLabel2->font() );
    textLabel2->setFont( textLabel2_font ); 
    layout15->addWidget( textLabel2 );

    comboTo = new QComboBox( FALSE, groupBox4, "comboTo" );
    layout15->addWidget( comboTo );

    groupBox4Layout->addLayout( layout15, 0, 1 );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    addButton = new QPushButton( groupBox4, "addButton" );
    layout12->addWidget( addButton );
    spacer8 = new QSpacerItem( 281, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout12->addItem( spacer8 );

    groupBox4Layout->addMultiCellLayout( layout12, 1, 1, 0, 1 );

    tableRoute = new QTable( groupBox4, "tableRoute" );
    tableRoute->setNumCols( tableRoute->numCols() + 1 );
    tableRoute->horizontalHeader()->setLabel( tableRoute->numCols() - 1, tr( "From" ) );
    tableRoute->setNumCols( tableRoute->numCols() + 1 );
    tableRoute->horizontalHeader()->setLabel( tableRoute->numCols() - 1, tr( "To" ) );
    tableRoute->setNumRows( 0 );
    tableRoute->setNumCols( 2 );
    tableRoute->setSelectionMode( QTable::SingleRow );

    groupBox4Layout->addMultiCellWidget( tableRoute, 2, 2, 0, 1 );

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    removeButton = new QPushButton( groupBox4, "removeButton" );
    layout11->addWidget( removeButton );
    spacer9 = new QSpacerItem( 291, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout11->addItem( spacer9 );

    groupBox4Layout->addMultiCellLayout( layout11, 3, 3, 0, 1 );

    RouteDialogLayout->addWidget( groupBox4, 2, 0 );
    languageChange();
    resize( QSize(566, 644).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( ok_() ) );
    connect( comboFrom, SIGNAL( activated(const QString&) ), this, SLOT( fromChanged_(const QString&) ) );
    connect( addButton, SIGNAL( clicked() ), this, SLOT( add_() ) );
    connect( removeButton, SIGNAL( clicked() ), this, SLOT( remove_() ) );
    connect( buttonGroup1, SIGNAL( clicked(int) ), this, SLOT( radioClicked_(int) ) );
    connect( comboTo, SIGNAL( activated(const QString&) ), this, SLOT( toChanged_(const QString&) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
RouteDialog::~RouteDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void RouteDialog::languageChange()
{
    setCaption( tr( "Network" ) );
    buttonGroup1->setTitle( tr( "Type" ) );
    radioRandom->setText( tr( "Regular" ) );
    radioDefined->setText( tr( "User defined" ) );
    buttonOk->setText( tr( "Ok" ) );
    groupBox4->setTitle( tr( "Route" ) );
    textLabel1->setText( tr( "From:" ) );
    textLabel2->setText( tr( "To:" ) );
    addButton->setText( tr( "Add" ) );
    tableRoute->horizontalHeader()->setLabel( 0, tr( "From" ) );
    tableRoute->horizontalHeader()->setLabel( 1, tr( "To" ) );
    removeButton->setText( tr( "Remove" ) );
}

void RouteDialog::ok_()
{
    qWarning( "RouteDialog::ok_(): Not implemented yet" );
}

void RouteDialog::fromChanged_(const QString&)
{
    qWarning( "RouteDialog::fromChanged_(const QString&): Not implemented yet" );
}

void RouteDialog::remove_()
{
    qWarning( "RouteDialog::remove_(): Not implemented yet" );
}

void RouteDialog::add_()
{
    qWarning( "RouteDialog::add_(): Not implemented yet" );
}

void RouteDialog::radioClicked_(int)
{
    qWarning( "RouteDialog::radioClicked_(int): Not implemented yet" );
}

void RouteDialog::toChanged_(const QString&)
{
    qWarning( "RouteDialog::toChanged_(const QString&): Not implemented yet" );
}
