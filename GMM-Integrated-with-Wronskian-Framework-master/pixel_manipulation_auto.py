#-*- coding: utf-8 -*-
from PIL import Image
import cv2 as cv
import numpy as np

ctr=187
ctrs=str(ctr)

img1  = np.genfromtxt('G:\SUMMER INTERN\@multi-camera colour tracking\GMM-Integrated-with-Wronskian-Framework-master\imgf33.txt', delimiter = ' ')
img2  = np.genfromtxt('G:\SUMMER INTERN\@multi-camera colour tracking\GMM-Integrated-with-Wronskian-Framework-master\jimgf33.txt', delimiter = ' ')
img1 = img1[:, 0:15]
img2 = img2[:, 0:15]
data_1 = img1.reshape(240,320,3)
data_2 = img2.reshape(240,320,3)


while ctr==187:
    img3  = np.genfromtxt('G:\SUMMER INTERN\@multi-camera colour tracking\GMM-Integrated-with-Wronskian-Framework-master\imgf'+ctrs+'.txt', delimiter = ' ')
    img4  = np.genfromtxt('G:\SUMMER INTERN\@multi-camera colour tracking\GMM-Integrated-with-Wronskian-Framework-master\jimgf'+ctrs+'.txt', delimiter = ' ')
    
    
    img3 = img3[:, 0:15]
    img4 = img4[:, 0:15]
    
    
    data_3 = img3.reshape(240,320,3)
    data_4 = img4.reshape(240,320,3)
    
    
#data_1=b&w base img
#data_2=colour base img
#data_3=colour region tracking img
#data_4=colour tracking img
    
    
# creating am image to track
    to_track_img=[]
    
    for i in range (0,240):
        for j in range (0,320):
            if(data_1[i,j,0]==255 and data_1[i,j,1]==255 and data_1[i,j,2]==255):
                to_track_img.append([data_2[i,j,0],data_2[i,j,1],data_2[i,j,2]])
            else:
                to_track_img.append([0,0,0])
     
#creating to-track image from array & saving image       
    arr_orig=np.array(to_track_img)
    arr_orig=arr_orig.astype(np.uint8)
    arr_orig=arr_orig.reshape(240,320,3)
      
    im1 = Image.fromarray(arr_orig)
    im1.save("to_track.jpeg")
    
#creating a target image
    target=[]

    for i in range (0,240):
        for j in range (0,320):
            if(data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,2]==255):
                target.append([data_4[i,j,0],data_4[i,j,1],data_4[i,j,2]])
            else:
                target.append([0,0,0])
     
#creating to-track image from array & saving image       
    arr_target=np.array(target)
    arr_target=arr_target.astype(np.uint8)
    arr_target=arr_target.reshape(240,320,3)
    im1 = Image.fromarray(arr_target)
    im1.save("target.jpeg")
    
#############
    
#initializing label
    k=int(256)
    
    test_img=data_3
    
#component analysis
    par_chi=dict()
    for i in range (0,240):
        for j in range(0,320):
            if(i==0 and j==0):
                continue
            elif(i==0):
                if(data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,0]==255) and (data_3[i,j-1,0]>255):
                    test_img[i,j,0]=data_3[i,j-1,0]
                    test_img[i,j,1]=data_3[i,j-1,1]
                    test_img[i,j,2]=data_3[i,j-1,2]
            elif(j==0):
                if(data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,0]==255) and (data_3[i-1,j,0]>255):
                    test_img[i,j,0]=data_3[i-1,j,0]
                    test_img[i,j,1]=data_3[i-1,j,1]
                    test_img[i,j,2]=data_3[i-1,j,2]
            elif((data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,0]==255)):
                if(data_3[i-1,j,0]>255 and data_3[i-1,j,0]<=data_3[i,j-1,0]):
                    test_img[i,j,0]=data_3[i-1,j,0]
                    test_img[i,j,1]=data_3[i-1,j,1]
                    test_img[i,j,2]=data_3[i-1,j,2]
                    #parent child
                    if data_3[i-1,j,0]!=data_3[i,j-1,0]:
                        par_chi[data_3[i,j-1,0]]=data_3[i-1,j,0]
                elif(data_3[i,j-1,0]>255 and data_3[i,j-1,0]<data_3[i-1,j,0]):
                    test_img[i,j,0]=data_3[i,j-1,0]
                    test_img[i,j,1]=data_3[i,j-1,1]
                    test_img[i,j,2]=data_3[i,j-1,2]
                    par_chi[data_3[i-1,j,0]]=data_3[i,j-1,0]
                    #parent child
                else:
                    test_img[i,j,0]=k
                    test_img[i,j,1]=k
                    test_img[i,j,2]=k
                    k=k+1

            else:
                continue
    
    
    
#print(par_chi)
    print(k)
    
    for i in range (0,240):
        for j in range(0,320):
            if test_img[i,j,0]>255:
                while(test_img[i,j,0] in par_chi.keys()):
                    test_img[i,j,0]=par_chi[test_img[i,j,0]]
                    test_img[i,j,1]=par_chi[test_img[i,j,1]]
                    test_img[i,j,2]=par_chi[test_img[i,j,2]]
    


#################
#counting no of components and their pixel frequency
    def frequency(x, no):
        count = 0
     
        for i in range(0,240):
            for j in range(0,320):
                if x[i,j,0] == no: 
                    count += 1
        return count
##################
#counting only those regions who have no of pixels greater than 650
    counter=[]
    for a in range(256,k):
        freq=frequency(test_img,a)
        if freq>650:
            counter.append([a,freq])
    
    print(counter)
####################

#assigning components a colour and declaring their names
    target_names=["red.jpeg","lime.jpeg","blue.jpeg","yellow.jpeg","magenta.jpeg","cyan.jpeg"]
    new_region_clr=[[255,0,0],[0,255,0],[0,0,255],[255,255,0],[255,0,255],[0,255,255]]

#initializing label for no. of components
    y=0
    for z in counter:
        for i in range(0,240):
            for j in range(0,320):
                if test_img[i,j,0]==z[0]:
                    test_img[i,j,0]=new_region_clr[y][0]
                    test_img[i,j,1]=new_region_clr[y][1]
                    test_img[i,j,2]=new_region_clr[y][2]
        y=y+1

    for i in range(0,240):
        for j in range(0,320):
            if test_img[i,j,0]>255:
                test_img[i,j,0]=0
                test_img[i,j,1]=0
                test_img[i,j,2]=0

#####################
            
#creating image from array & saving image
    arr_test=test_img.astype(np.uint8)

    im1 = Image.fromarray(arr_test)
    im1.save("target_components.jpeg")

#reading to-track and target image and displaying it
    target=cv.imread('target.jpeg')
    target_components=cv.imread('target_components.jpeg')
    cv.imshow('target',target)
    cv.imshow('target_components',target_components)
    cv.waitKey(0)
    cv.destroyAllWindows()
########### end of component analysis

#reading to-track image to create its histogram for chi-square matching
    to_track=cv.imread('to_track.jpeg')

## [Convert to HSV]
    hsv_f1 = cv.cvtColor(to_track, cv.COLOR_BGR2HSV)

#hist = cv.calcHist([hsv_base], [0, 1], None, [180, 256], [0, 180, 0, 256])
#[Using 50 bins for hue and 60 for saturation]
    h_bins = 50
    s_bins = 60
    histSize = [h_bins, s_bins]

# hue varies from 0 to 179, saturation from 0 to 255
    h_ranges = [0, 180]
    s_ranges = [0, 256]
    ranges = h_ranges + s_ranges # concat lists

# Use the 0-th and 1-st channels
    channels = [0, 1]
## [Using 50 bins for hue and 60 for saturation]

## [Calculate the histograms for the HSV images]
    h1 = cv.calcHist([hsv_f1], channels, None, histSize, ranges, accumulate=False)
    cv.normalize(h1, h1, alpha=0, beta=1, norm_type=cv.NORM_MINMAX)

## [Selecting Histogram Comparison Method -Chi SQuare ]
    compare_method =1

#function for chi-square matching
    def chi_square_compare(hist,histo):
        chi_squared_val = cv.compareHist(hist, histo, compare_method)
        print(chi_squared_val)
        return chi_squared_val

    chi_sq_val=[]
##########

#function to find minimun chi_square_value
    def min_chi(lst):
        lst=np.array(lst)
        min=lst[0]
        ind=0
        for i in range(1,y):
            if lst[i]<min:
                min=lst[i]
                ind=i
        print(min)
        return(ind)
##########
    
#checking every component with to-track image using chi square matching
###############################################################
    for k in range(0,y):
        test_img1=[]
        for i in range(0,240):
            for j in range(0,320):
                if test_img[i,j,0]==new_region_clr[k][0] and test_img[i,j,1]==new_region_clr[k][1] and test_img[i,j,2]==new_region_clr[k][2]:
                    test_img1.append([data_4[i,j,0],data_4[i,j,1],data_4[i,j,2]])
                else:
                    test_img1.append([0,0,0])
#converting component into array and saving it
        arr_test1=np.array(test_img1)
        arr_test1=arr_test1.astype(np.uint8)
        arr_test1=arr_test1.reshape(240,320,3)
        im1 = Image.fromarray(arr_test1)
        im1.save(target_names[k])
#reading same image to generate its histogram
        target=cv.imread(target_names[k])
    
#creating histogram for matching
        hsv_f2 = cv.cvtColor(target, cv.COLOR_BGR2HSV)
        h2 = cv.calcHist([hsv_f2], channels, None, histSize, ranges, accumulate=False)
        cv.normalize(h2, h2, alpha=0, beta=1, norm_type=cv.NORM_MINMAX)
        print("Chi Square Values on Comparing between to-track and ",k+1," object")
#chi-square matching
        v=chi_square_compare(h1,h2)
#appending chi-square values to list
        chi_sq_val.append(v)
################################################################

#converting chi-square list to array
    chi_sq_val_arr=np.array(chi_sq_val)

#calculating the index of min ch_sq_val
    index=min_chi(chi_sq_val_arr)

#displaying traced object with to-track object
    traced=cv.imread(target_names[index])
    target=cv.imread('target.jpeg')
    cv.imshow('To Track',to_track)
    cv.imshow('target',target)
    cv.imshow('traced object',traced)
    cv.waitKey(0)
    cv.destroyAllWindows()
    ctr=ctr+1
