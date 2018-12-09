#include<QApplication>
#include "gui.hpp"
#include<iostream>
#include<sstream>
#include<QFileDialog>
#include<QFileInfo>
#include<QImage>
#include<QPixmap>
#include<stdio.h>


void view::onbrowse()//load image,save image name,display image on first widget
{
  //file dialog open
  QString name=QFileDialog::getOpenFileName();

  //display selected image
  QPixmap pixmap(name);
  qltl->setPixmap(name);


  imgwd=pixmap.width();
  imght=pixmap.height();

  mean.resize(imgwd*imght);
  mean1.resize(imgwd*imght);
  mean2.resize(imgwd*imght);
  mean3.resize(imgwd*imght);
  mean4.resize(imgwd*imght);
  image.resize(imgwd*imght);
  nolop.resize(imgwd*imght);
  nolop1.resize(imgwd*imght);
  nolop2.resize(imgwd*imght);
  nolop3.resize(imgwd*imght);
  nolop4.resize(imgwd*imght);

  QFileInfo fi(name);
  QString base=fi.baseName();//tmp0001
  QString num="";
  QString nam="";
  basenum="";
  basenam="";
  for(int i=0;i<base.length();i++)
  {
    if(base[i]>=48 && base[i]<=57)
      num+=(base[i]);
    else
      nam+=(base[i]);
  }

  try
  {
    basenum=num.toUtf8().constData();
    basenam=nam.toUtf8().constData();
    path=(fi.path()+"/").toUtf8().constData();///home/erudite/image
    ext=("."+fi.suffix()).toUtf8().constData();//jpg
  }
  catch(...)
  {
    basenum=num.toLocal8Bit().constData();
    basenam=nam.toLocal8Bit().constData();
    path=(fi.path()+"/").toLocal8Bit().constData();///home/erudite/image
    ext=("."+fi.suffix()).toLocal8Bit().constData();//jpg
  }
}



// load wronskian vector size, load threshold value and execute
void view::ontrain()
{
  //getting range of images to be used for background subtraction
  QString text=qletl->text();
  QStringList tx=text.split("-");
  try
  {
    beg=tx[0].toInt();
    end=tx[1].toInt();
  }
  catch(...)
  {
    beg=1;
    end=beg;
  }

  //getting the threshold value
  thval=qsbtl1->value();
  //getting the size of wronskian vector
  wvsize=qsbtl2->value();
  cout<<"threshold value: "<<thval<<endl<<"wronskian vector size: "<<wvsize<<endl;

  //calculating wvsize1
  wvsize1=(wvsize-1)/2;
  cout<<"wvsize1 : "<<wvsize1<<endl;

  train();
  QPixmap pixmap("bg.bmp");
  qltl->setPixmap(pixmap);
  qlbl->setPixmap(pixmap);
}


void view::init_track()
{
  medval=qsbbl1->value();
  medval1=(medval-1)/2;
  thval=qsbtl1->value();
  nummean=qsbbl->value();
  wvsize=qsbtl2->value();
  wvsize1=(wvsize-1)/2;
  QString txt=qlebl->text();
  QStringList txl=txt.split("-");
  try
  {
    beg=txl[0].toInt();
    end=txl[1].toInt();
  }
  catch(...)
  {
    beg=1;
    end=beg;
  }
}


int view::ontrackwMF()
{
  init_track();
  stringstream formimg;
  for(int i=beg;i<=end;i++)
  {
    formimg.str(std::string());
    formimg<<path<<basenam<<zfill(i,basenum.length())<<ext;

    imgread(formimg.str());

    QImage img(imgwd,imght,QImage::Format_RGB888);
    QImage img1(imgwd,imght,QImage::Format_RGB888);
    int tempimg[imght][imgwd];
    for(int j=0;j<imght;j++)
    {
      for(int k=0;k<imgwd;k++)
      {
        if(wronskian(j,k,&mean[0]) || wronskian(j,k,&mean1[0]) || wronskian(j,k,&mean2[0]) || wronskian(j,k,&mean3[0]) || wronskian(j,k,&mean4[0]))
          tempimg[j][k]=0;
        else
          tempimg[j][k]=1;
      }
    }

    int num0=0;
    for(int j=0;j<imght;j++)
    {
      for(int k=0;k<imgwd;k++)
      {
        num0=0;
        if(j>=medval1 && k>=medval1 && j<imght-medval1 && k<imgwd-medval1)
        {
          for(int l=j-medval1;l<j+medval1+1;l++)
          {
            for(int m=k-medval1;m<k+medval1+1;m++)
            {
              if(tempimg[l][m]==0)
                num0++;
            }
          }
          if(num0>(medval*medval-1)/2)
          {
            img.setPixel(k,imght-j-1,qRgb(0,0,0));
            img1.setPixel(k,imght-j-1,qRgb(200,200,200));
          }
          else
          {
            img.setPixel(k,imght-j-1,qRgb(255,255,255));
            img1.setPixel(k,imght-j-1,qRgb(image[j*imgwd+k].R,image[j*imgwd+k].G,image[j*imgwd+k].B));
          }
        }
      }
    }

    QString tn=dir+"/"+"imgf"+QString::number(i)+".bmp";
    qltr->setPixmap(QPixmap::fromImage(img));
    topright->repaint();
    qlbl->setPixmap(QPixmap::fromImage(img1));
    bottomleft->repaint();
    QPixmap pixmap(formimg.str().c_str());
    qlbr->setPixmap(pixmap);
    bottomright->repaint();
    if(save==1)
      img.save(tn,"BMP",-1);
  }

  return 0;
}


int view::ontrack()
{
  init_track();
  stringstream formimg;
  for(int i=beg;i<=end;i++)
  {
    formimg.str(std::string());
    formimg<<path<<basenam<<zfill(i,basenum.length())<<ext;

    imgread(formimg.str());

    QImage img(imgwd,imght,QImage::Format_RGB888);
    QImage img1(imgwd,imght,QImage::Format_RGB888);
    for(int j=0;j<imght;j++)
    {
      for(int k=0;k<imgwd;k++)
      {
        if(nummean==5)
        {
          if(wronskian(j,k,&mean[0])==true||wronskian(j,k,&mean1[0])==true||wronskian(j,k,&mean2[0])==true||wronskian(j,k,&mean3[0])==true||wronskian(j,k,&mean4[0])==true)
          {
            img.setPixel(k,imght-j-1,qRgb(0,0,0));
            img1.setPixel(k,imght-j-1,qRgb(200,200,200));
          }
          else
          {
            img.setPixel(k,imght-j-1,qRgb(255,255,255));
            img1.setPixel(k,imght-j-1,qRgb(image[j*imgwd+k].R,image[j*imgwd+k].G,image[j*imgwd+k].B));
          }
        }
        else if(nummean==4)
        {
          if(wronskian(j,k,&mean[0])==true||wronskian(j,k,&mean1[0])==true||wronskian(j,k,&mean2[0])==true||wronskian(j,k,&mean3[0])==true)
          {
            img.setPixel(k,imght-j-1,qRgb(0,0,0));
            img1.setPixel(k,imght-j-1,qRgb(200,200,200));
          }

          else
          {
            img.setPixel(k,imght-j-1,qRgb(255,255,255));
            img1.setPixel(k,imght-j-1,qRgb(image[j*imgwd+k].R,image[j*imgwd+k].G,image[j*imgwd+k].B));
          }
        }
        else if(nummean==3)
        {
          if(wronskian(j,k,&mean[0])==true||wronskian(j,k,&mean1[0])==true||wronskian(j,k,&mean2[0])==true)
          {
            img.setPixel(k,imght-j-1,qRgb(0,0,0));
            img1.setPixel(k,imght-j-1,qRgb(200,200,200));
          }
          else
          {
            img.setPixel(k,imght-j-1,qRgb(255,255,255));
            img1.setPixel(k,imght-j-1,qRgb(image[j*imgwd+k].R,image[j*imgwd+k].G,image[j*imgwd+k].B));
          }
        }
        else if(nummean==2)
        {
          if(wronskian(j,k,&mean[0])==true||wronskian(j,k,&mean1[0])==true)
          {
            img.setPixel(k,imght-j-1,qRgb(0,0,0));
            img1.setPixel(k,imght-j-1,qRgb(200,200,200));
          }
          else
          {
            img.setPixel(k,imght-j-1,qRgb(255,255,255));
            img1.setPixel(k,imght-j-1,qRgb(image[j*imgwd+k].R,image[j*imgwd+k].G,image[j*imgwd+k].B));
          }
        }
        else
        {
          if(wronskian(j,k,&mean[0])==true)
          {
            img.setPixel(k,imght-j-1,qRgb(0,0,0));
            img1.setPixel(k,imght-j-1,qRgb(200,200,200));
          }
          else
          {
            img.setPixel(k,imght-j-1,qRgb(255,255,255));
            img1.setPixel(k,imght-j-1,qRgb(image[j*imgwd+k].R,image[j*imgwd+k].G,image[j*imgwd+k].B));
          }
        }
      }
    }

    QString tn="imgf"+QString::number(i)+".bmp";
    qltr->setPixmap(QPixmap::fromImage(img));
    topright->repaint();
    qlbl->setPixmap(QPixmap::fromImage(img1));
    bottomleft->repaint();
    QPixmap pixmap(formimg.str().c_str());
    qlbr->setPixmap(pixmap);
    bottomright->repaint();
    if(save==1)
    {
      img.save(dir+"/"+tn,"BMP",-1);
      img1.save(dir+"/"+"j"+tn,"BMP",-1);
    }
  }

  return 0;
}

void view::onsave()
{
  if(save!=1)
  {
    save=1;
    qpbbl3->setText("save-on");
    dir=QFileDialog::getExistingDirectory();
  }
  else
  {
    save=0;
    qpbbl3->setText("save-off");
  }
}

int main(int argc, char *argv[])
{
  QApplication app(argc,argv);
  view window;
  window.show();
  return app.exec();
}
