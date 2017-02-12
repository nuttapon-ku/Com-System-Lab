import sys
import os
import numpy as np
import cv2

def split_into_rgb_channels(image):
  '''Split the target image into its red, green and blue channels.
  image - a numpy array of shape (rows, columns, 3).
  output - three numpy arrays of shape (rows, columns) and dtype same as
  image, containing the corresponding channels. 
  '''
  red = image[:,:,2]
  green = image[:,:,1]
  blue = image[:,:,0]
  return red, green, blue

def main():
  ''' This function searches for a folder images/knowpapa subfolder, and splits
  all images found in that folder into its equivalent rgb channel. 
  It saves each image appending the terms 'red', 'green' and
  'blue' to the orginal filename.
  '''
  
  imagesdir = os.path.abspath(os.path.join(os.curdir, 'images'))
  print "Searching for images in {} Directory)".format(imagesdir)
  exts = ['.bmp', '.pbm', '.pgm', '.ppm', '.sr', '.ras', '.jpeg', '.jpg', 
          '.jpe', '.jp2', '.tiff', '.tif', '.png']
  for dirname, dirnames, filenames in os.walk(imagesdir):
    for filename in filenames:
      count_grey = 0
      name, ext = os.path.splitext(filename)
      img = cv2.imread(os.path.join(dirname, filename))
      red, green, blue = split_into_rgb_channels(img)

      row, col = red.shape

      for i in range(row):
        for j in range(col):
          if(red[i,j]>128 and green[i,j]>128 and blue[i,j]>128):
            count_grey += 1

      print "File name : " + filename
      print "Grey Color: " + str(count_grey)
      print "================================"

if __name__ == "__main__":
  main()