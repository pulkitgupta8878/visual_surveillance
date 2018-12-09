#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sstream>
#include<math.h>
#include "gui.hpp"
#include<unistd.h>
#include<QImage>

using namespace std;


//This function pads zero according to pad value to an int
string Alg::zfill(int num,int pad)
{
  stringstream st;
  st<<num;
  string snum;
  st>>snum;
  int nz=pad-snum.length();
  for(int i=0;i<nz;i++)
    snum="0"+snum;
  return snum;
}
//function done END


Alg::Alg()
{
  cout<<"Welcome to the big bang!"<<endl;
}


void Alg::imgread(string fname)
{
  int pix=0; 
  QImage img(fname.c_str());
  for (int i=0;i<imght;i++)
  {
    for (int j=0;j<imgwd;j++)
    {
      QRgb rgbtriple=img.pixel(j,imght-i-1);
      image[pix].R=qRed(rgbtriple);
      image[pix].G=qGreen(rgbtriple);
      image[pix].B=qBlue(rgbtriple);
      pix++;
    }
  }
}



int Alg::train()
{

  count=0;
  stringstream formimg;
  vector<rgbs> tempm,tempm1,tempm2,tempm3,tempm4;
  tempm.resize(imgwd*imght);
  tempm1.resize(imgwd*imght);
  tempm2.resize(imgwd*imght);
  tempm3.resize(imgwd*imght);
  tempm4.resize(imgwd*imght);
  nolop.resize(imgwd*imght);
  nolop1.resize(imgwd*imght);
  nolop2.resize(imgwd*imght);
  nolop3.resize(imgwd*imght);
  nolop4.resize(imgwd*imght);
  for(int i=beg;i<=end;i++)
  {
    count++;
    //Reading each image
    formimg.str(std::string());//clearing string stream
    formimg<<path<<basenam<<zfill(i,basenum.length())<<ext;
    
    imgread(formimg.str());

    int pixnum=0;
    for(int j=0;j<imght;j++)
    {
      for(int k=0;k<imgwd;k++)
      {  
        if(count==1)
        {
          ++nolop[pixnum];
          tempm[pixnum]=image[pixnum];
          tempm1[pixnum]=image[pixnum];
          tempm2[pixnum]=image[pixnum];
          tempm3[pixnum]=image[pixnum];
          tempm4[pixnum]=image[pixnum];
        }
        else if(wronskian(j,k,&mean[0]))
        {
          ++nolop[pixnum];
	  calcmean(image[pixnum],mean[pixnum],&tempm[pixnum],nolop[pixnum]);
        }
        else if(nolop1[pixnum]<1)
        {
          ++nolop1[pixnum];
          tempm1[pixnum]=image[pixnum];
        }
        else if(wronskian(j,k,&mean1[0]))
        {
          ++nolop1[pixnum];
          calcmean(image[pixnum],mean1[pixnum],&tempm1[pixnum],nolop1[pixnum]);
        }
        else if(nolop2[pixnum]<1)
        {
          ++nolop2[pixnum];
          tempm2[pixnum]=image[pixnum];
        }
        else if(wronskian(j,k,&mean2[0]))
        {
          ++nolop2[pixnum];
          calcmean(image[pixnum],mean2[pixnum],&tempm2[pixnum],nolop2[pixnum]);
        }
        else if(nolop3[pixnum]<1)
        {
          ++nolop3[pixnum];
          tempm3[pixnum]=image[pixnum];
        }
        else if(wronskian(j,k,&mean3[0]))
        {
          ++nolop3[pixnum];
          calcmean(image[pixnum],mean3[pixnum],&tempm3[pixnum],nolop3[pixnum]);
        }
        else if(nolop4[pixnum]<1)
        {
          ++nolop4[pixnum];
          tempm4[pixnum]=image[pixnum];
        }
        else if(wronskian(j,k,&mean4[0]))
        {
          ++nolop4[pixnum];
          calcmean(image[pixnum],mean4[pixnum],&tempm4[pixnum],nolop4[pixnum]);
        }
        
      pixnum++;
      }//end of col loop
    }//end of row loop
    update(&mean[0],&tempm[0]);
    update(&mean1[0],&tempm1[0]);
    update(&mean2[0],&tempm2[0]);
    update(&mean3[0],&tempm3[0]);
    update(&mean4[0],&tempm4[0]);
  }
  
  QImage img(imgwd,imght,QImage::Format_RGB888);
  for(int j=0;j<imght;j++)
    for(int k=0;k<imgwd;k++)
      img.setPixel(k,imght-j-1,qRgb(mean[j*imgwd+k].R,mean[j*imgwd+k].G,mean[j*imgwd+k].B));
   
  img.save("bg.bmp","BMP",-1);
  
  
  
  cout<<"see you at other end of galaxy"<<endl;
   
  return 0;
}



//This function here, calculates the wronskian value
bool Alg::wronskian(int row,int col,rgbs *mean_t)
{
  float wronskr=0;
  float wronskg=0;
  float wronskb=0;
  float tempr=0;
  float tempg=0;
  float tempb=0;
  float diffr=0;
  float diffg=0;
  float diffb=0;
  int pos=0;
  int kryptr=0;
  int kryptg=0;
  int kryptb=0;
  if((row-wvsize1)>=0 && (row+wvsize1)<imght && (col-wvsize1)>=0 && (col+wvsize1)<imgwd)
  {
    for(int i=row-wvsize1;i<row+wvsize1+1;i++)
    {
      for(int j=col-wvsize1;j<col+wvsize1+1;j++)
      {
        pos=i*imgwd+j;
        if(image[pos].R<5 || mean_t[pos].R<5)
        {
          kryptr++;
          wronskr+=fabs(mean_t[pos].R-image[pos].R)/200;
        }
        else
        {
          wronskr+=(mean_t[pos].R*mean_t[pos].R)/(image[pos].R*image[pos].R);
          tempr+=mean_t[pos].R/image[pos].R;
        }
        
        if(image[pos].G<5 || mean_t[pos].G<5)
        {
          kryptg++;
          wronskg+=fabs(mean_t[pos].G-image[pos].G)/200;  
        }
        else
        {
          wronskg+=(mean_t[pos].G*mean_t[pos].G)/(image[pos].G*image[pos].G);
          tempg+=mean_t[pos].G/image[pos].G;
        }
        
        if(image[pos].B<5 || mean_t[pos].B<5)
        {
          kryptb++;
          wronskb+=fabs(mean_t[pos].B-image[pos].B)/200;
        }
        else
        {
          wronskb+=(mean_t[pos].B*mean_t[pos].B)/(image[pos].B*image[pos].B);
          tempb+=mean_t[pos].B/image[pos].B;  
        }
      }
    }
    diffr=(fabs(wronskr-tempr))/(wvsize*wvsize-kryptr);
    diffg=(fabs(wronskg-tempg))/(wvsize*wvsize-kryptg);
    diffb=(fabs(wronskb-tempb))/(wvsize*wvsize-kryptb); 
    
  }
    if(diffr>thval || diffg>thval || diffb>thval)
      return false;
    else 
      return true;
  /*} 
  else
  {
    diffr=fabs(image[row*imgwd+col].R-mean_t[row*imgwd+col].R);
    diffg=fabs(image[row*imgwd+col].G-mean_t[row*imgwd+col].G);
    diffb=fabs(image[row*imgwd+col].B-mean_t[row*imgwd+col].B);
    if(diffr>10 || diffg>10 || diffb>10)
      return false;
    else 
      return true;
  }*/
  
  
}
//wronskian function done END



//This function calculates the mean
void Alg::calcmean(rgbs rgbt,rgbs meanval,rgbs *tempmean,int ct)
{
  tempmean->R=((ct-1)*( meanval.R)+rgbt.R)/ct;
  tempmean->G=((ct-1)*( meanval.G)+rgbt.G)/ct;
  tempmean->B=((ct-1)*( meanval.B)+rgbt.B)/ct;
}
//mean calculated done END


//This function update the mean arrays
void Alg::update(rgbs *main_arr,rgbs *temp_arr)
{
  for(int i=0;i<imgwd*imght;i++)
    main_arr[i]=temp_arr[i];
}


int Alg::track()
{ 
  cout<<"that's the end"<<endl;
  return 0;
}
