#ifndef INCLUDE_GUI_HPP
#define INCLUDE_GUI_HPP


#include<QWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QGridLayout>
#include<QSpinBox>
#include<QLabel>
#include<QPainter>
#include<vector>
#include<string>
#include<stdint.h>
#include<QDoubleSpinBox>

using namespace std;

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/**
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309(PROT.10).aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */

typedef struct 
{ 
    WORD   bfType; 
    DWORD  bfSize; 
    WORD   bfReserved1; 
    WORD   bfReserved2; 
    DWORD  bfOffBits; 
} __attribute__((__packed__)) 
BITMAPFILEHEADER; 

typedef struct
{
    DWORD  biSize; 
    LONG   biWidth; 
    LONG   biHeight; 
    WORD   biPlanes; 
    WORD   biBitCount; 
    DWORD  biCompression; 
    DWORD  biSizeImage; 
    LONG   biXPelsPerMeter; 
    LONG   biYPelsPerMeter; 
    DWORD  biClrUsed; 
    DWORD  biClrImportant; 
} __attribute__((__packed__))
BITMAPINFOHEADER; 

typedef struct
  {
    BYTE  B;
    BYTE  G;
    BYTE  R;
  } __attribute__((__packed__))
  rgbs;

class Alg
{
  protected: int wvsize,wvsize1,imgwd,imght,count,beg,end,medval,medval1,nummean;
  protected: float thval;
  public: string path,basenam,basenum,ext;
  protected: vector<rgbs> mean,image,mean1,mean2,mean3,mean4;
  protected: vector<int> nolop,nolop1,nolop2,nolop3,nolop4;//no. of already occured linearly dependent pixel
  public: Alg();
  void imgread(string);
  bool wronskian(int,int,rgbs *);
  void calcmean(rgbs, rgbs,rgbs*, int);
  int train();
  void update(rgbs*,rgbs*);
  int track();
  string zfill(int,int);
};

class view: public QWidget, public Alg
{
  Q_OBJECT

  public:
    view(QWidget *parent=0);
  
  //4 main qwidgets  
  QWidget *topleft;
  QWidget *topright;
  QWidget *bottomleft;
  QWidget *bottomright;
  
  //grid layouts for the 4 main qwidgets  
  QGridLayout *qgltl;
  QGridLayout *qgltr;
  QGridLayout *qglbl;
  QGridLayout *qglbr;
  
  //subwidgets of the topleft qwidget
  QLabel *qltl;
  QLineEdit *qletl;
  QPushButton *qpbtl1;
  QPushButton *qpbtl2;
  QDoubleSpinBox *qsbtl1;
  QSpinBox *qsbtl2;
  
  //subwidgets of the bottomleft qwidget
  QLabel *qlbl;
  QLineEdit *qlebl;
  QSpinBox *qsbbl;
  QSpinBox *qsbbl1;
  QPushButton *qpbbl1;
  QPushButton *qpbbl2;
  QPushButton *qpbbl3;
  
  //subwidgets of topright qwidget
  QLabel *qltr;
  
  //subwidgets of bottomright qwidget
  QLabel *qlbr;
  
  void init_track();
  int save;
  QString dir;
  public slots: 
    void onbrowse();
    void ontrain();
    int ontrack();
    int ontrackwMF();
    void onsave();
};


#endif
