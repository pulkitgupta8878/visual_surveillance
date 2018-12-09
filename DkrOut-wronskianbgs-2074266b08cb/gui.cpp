#include<QWidget>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QSplitter>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QImage>
#include "gui.hpp"

using namespace std;

view::view(QWidget *parent)
  :QWidget(parent)
  {
  
  
    //4 main qwidgets  
    topleft=new QWidget(this);
    topright=new QWidget(this);
    bottomleft=new QWidget(this);
    bottomright=new QWidget(this);
  
    //grid layouts for the 4 main qwidgets  
    qgltl=new QGridLayout();
    qgltr=new QGridLayout();
    qglbl=new QGridLayout();
    qglbr=new QGridLayout();
  
    //subwidgets of the topleft qwidget
    qltl=new QLabel();
    qletl=new QLineEdit();
    qpbtl1=new QPushButton("Browse");
    qpbtl2=new QPushButton("Train");
    qsbtl1=new QDoubleSpinBox();
    qsbtl2=new QSpinBox();
  
    //subwidgets of the bottomleft qwidget
    qlbl=new QLabel();
    qlebl=new QLineEdit();
    qsbbl=new QSpinBox();
    qsbbl1=new QSpinBox();
    qpbbl1=new QPushButton("WMF");
    qpbbl2=new QPushButton("WOMF");
    qpbbl3=new QPushButton("save-off");
  
    //subwidgets of topright qwidget
    qltr=new QLabel();
  
    //subwidgets of bottomright qwidget
    qlbr=new QLabel();
    
    this->resize(1360,740);
    this->setWindowTitle("Object Tracker");
    
    QHBoxLayout *maingrid=new QHBoxLayout(this);
    maingrid->setSpacing(1);
    
    
    //subwidgets for top-left-qwidget
    qgltl->setSpacing(10);
    qgltl->addWidget(qltl,3,2);
    qgltl->addWidget(qpbtl1,1,1);
    qgltl->addWidget(qpbtl2,1,2);
    qgltl->addWidget(qletl,1,3);
    qgltl->addWidget(qsbtl1,1,4);
    qgltl->addWidget(qsbtl2,1,5);
    topleft->setLayout(qgltl);
    QLabel *qltl1=new QLabel();
    QLabel *qltl2=new QLabel();
    QLabel *qltl3=new QLabel();
    qltl1->setText("range");
    qltl2->setText("threshold");
    qltl3->setText("wvsize");
    qsbtl1->setDecimals(10);
    qsbtl1->setValue(0.01);
    qsbtl2->setValue(5);
    qgltl->addWidget(qltl1,2,3);
    qgltl->addWidget(qltl2,2,4);
    qgltl->addWidget(qltl3,2,5);
    
    qletl->setFixedSize(75,20);
    qpbtl1->setFixedSize(75,20);
    qpbtl2->setFixedSize(75,20);
    qsbtl1->setFixedSize(85,20);
    qsbtl2->setFixedSize(75,20);
    qltl->setFixedSize(500,320);
    topleft->setFixedSize(670,330);
    
    
    
    //subwidgets for bottom-left-qwidget
    qglbl->setSpacing(10);
    QLabel *qlbl1=new QLabel();
    QLabel *qlbl2=new QLabel();
    QLabel *qlbl3=new QLabel();
    qlbl1->setText("range");
    qlbl2->setText("means");
    qlbl3->setText("medfil");
    qglbl->addWidget(qlbl,3,2);
    qglbl->addWidget(qpbbl1,1,5);
    qglbl->addWidget(qpbbl2,1,6);
    qglbl->addWidget(qpbbl3,1,7);
    qglbl->addWidget(qlebl,1,1);
    qglbl->addWidget(qsbbl,1,2);
    qglbl->addWidget(qsbbl1,1,3);
    qglbl->addWidget(qlbl1,2,1);
    qglbl->addWidget(qlbl2,2,2);
    qglbl->addWidget(qlbl3,2,3);
    bottomleft->setLayout(qglbl);
    
    qpbbl1->setFixedSize(75,20);
    qpbbl2->setFixedSize(75,20);
    qpbbl3->setFixedSize(75,20);
    qlebl->setFixedSize(75,20);
    qsbbl->setFixedSize(75,20);
    qsbbl1->setFixedSize(75,20);
    qlbl->setFixedSize(500,320);
    bottomleft->setFixedSize(670,330);
    qsbbl->setValue(5);
    qsbbl1->setValue(3);
    //subwidgets for top-right-qwidget
    qgltr->setSpacing(0);
    qgltr->addWidget(qltr,3,2);
    topright->setLayout(qgltr);
    
    qltr->setFixedSize(500,320);
    topright->setFixedSize(670,330);
    
    //subwidgets for bottom-right-qwidget
    qglbr->setSpacing(0);
    qglbr->addWidget(qlbr,3,2);
    bottomright->setLayout(qglbr);
    
    qlbr->setFixedSize(500,320);
    bottomright->setFixedSize(670,330);
    
    //splitter for 4 main sub-widgets
    QSplitter *splitter1=new QSplitter(Qt::Horizontal);
    splitter1->addWidget(topleft);
    splitter1->addWidget(topright);
    
    QSplitter *splitter2=new QSplitter(Qt::Horizontal);
    splitter2->addWidget(bottomleft);
    splitter2->addWidget(bottomright);
    
    QSplitter *splitterv=new QSplitter(Qt::Vertical);
    splitterv->addWidget(splitter1);
    splitterv->addWidget(splitter2);
    
    //maingrid layout
    maingrid->addWidget(splitterv);
    setLayout(maingrid);
    
    //on pushbuttons
    connect(qpbtl1,SIGNAL(clicked()),this,SLOT(onbrowse()));
    connect(qpbtl2,SIGNAL(clicked()),this,SLOT(ontrain()));
    connect(qpbbl1,SIGNAL(clicked()),this,SLOT(ontrackwMF()));
    connect(qpbbl2,SIGNAL(clicked()),this,SLOT(ontrack()));
    connect(qpbbl3,SIGNAL(clicked()),this,SLOT(onsave()));
     
  }
