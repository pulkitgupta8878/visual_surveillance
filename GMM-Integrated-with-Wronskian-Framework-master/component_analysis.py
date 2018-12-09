# connected component analysis - maximum 6 components with sizes greater than 650 pixels
from PIL import Image
import numpy as np
import cv2 as cv

text_bw  = np.genfromtxt('G:\SUMMER INTERN\@multi-camera colour tracking\GMM-Integrated-with-Wronskian-Framework-master\imgf1081.txt', delimiter = ' ')

text_bw = text_bw[:, 0:15]

data_3 = text_bw.reshape(288,360, 3)


#initializing label
k=int(256)

new_img=data_3

#component analysis algorithm
par_chi=dict()
for i in range (0,288):
    for j in range(0,360):
        if(i==0 and j==0):
            continue
        elif(i==0):
            if(data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,0]==255) and (data_3[i,j-1,0]>255):
                new_img[i,j,0]=data_3[i,j-1,0]
                new_img[i,j,1]=data_3[i,j-1,1]
                new_img[i,j,2]=data_3[i,j-1,2]
        elif(j==0):
            if(data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,0]==255) and (data_3[i-1,j,0]>255):
                new_img[i,j,0]=data_3[i-1,j,0]
                new_img[i,j,1]=data_3[i-1,j,1]
                new_img[i,j,2]=data_3[i-1,j,2]
        elif((data_3[i,j,0]==255 and data_3[i,j,1]==255 and data_3[i,j,0]==255)):
            if(data_3[i-1,j,0]>255 and data_3[i-1,j,0]<=data_3[i,j-1,0]):
                new_img[i,j,0]=data_3[i-1,j,0]
                new_img[i,j,1]=data_3[i-1,j,1]
                new_img[i,j,2]=data_3[i-1,j,2]
                #parent child
                if data_3[i-1,j,0]!=data_3[i,j-1,0]:
                    par_chi[data_3[i,j-1,0]]=data_3[i-1,j,0]
            elif(data_3[i,j-1,0]>255 and data_3[i,j-1,0]<data_3[i-1,j,0]):
                new_img[i,j,0]=data_3[i,j-1,0]
                new_img[i,j,1]=data_3[i,j-1,1]
                new_img[i,j,2]=data_3[i,j-1,2]
                par_chi[data_3[i-1,j,0]]=data_3[i,j-1,0]
                #parent child
            else:
                new_img[i,j,0]=k
                new_img[i,j,1]=k
                new_img[i,j,2]=k
                k=k+1

        else:
            continue



print(par_chi)
print(k)

for i in range (0,288):
    for j in range(0,360):
        if new_img[i,j,0]>255:
            while(new_img[i,j,0] in par_chi.keys()):
                new_img[i,j,0]=par_chi[new_img[i,j,0]]
                new_img[i,j,1]=par_chi[new_img[i,j,1]]
                new_img[i,j,2]=par_chi[new_img[i,j,2]]
    


#################
#counting no of components and their pixel frequency
def frequency(x, no):
    count = 0
     
    for i in range(0,288):
        for j in range(0,360):
            if x[i,j,0] == no: 
                count += 1
    return count
##################
#counting only those regions who have no of pixels greater than 650 and store the label no. in a list
counter=[]
for a in range(256,k):
    freq=frequency(new_img,a)
    if freq>650:
        counter.append([a,freq])
    
print(counter)
####################
#assigning components a colour
#
new_region_clr=[[255,0,0],[0,255,0],[0,0,255],[255,255,0],[255,0,255],[0,255,255]]
y=0

for z in counter:
    for i in range(0,288):
        for j in range(0,360):
            if new_img[i,j,0] == z[0]: 
                new_img[i,j,0]=new_region_clr[y][0]
                new_img[i,j,1]=new_region_clr[y][1]
                new_img[i,j,2]=new_region_clr[y][2]
    y+=1

for i in range(0,288):
    for j in range(0,360):
        if new_img[i,j,0]>255:
            new_img[i,j,0]=0
            new_img[i,j,1]=0
            new_img[i,j,2]=0

#####################
            
#converting array to image and saving it
arr_orig=new_img.astype(np.uint8)
arr_orig=arr_orig.reshape(288,360,3)

im1 = Image.fromarray(arr_orig)
im1.save("original.jpeg")

#reading image and displaying
image=cv.imread('original.jpeg')
cv.imshow('',image)
cv.waitKey(0)
cv.destroyAllWindows()