"""
The goal of this code is to load in the video feed, periodically sample a frame, check
to see where the line is within that frame, and to output an instruction to either
keep going forward or stop, then to either turn right or left, based on where the line 
is within the frame
"""

import cv2 as cv
import numpy as np
import math

video = cv.VideoCapture("video0")

while(1):

    #read in frame and convert to HSV
    _ , frame = video.read()
    hsvframe = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    #identify blue range
    blue = cv.inRange(hsvframe, (0,100,0), (50,185,255))

    #header will be the value that we send off to determine where to go
    #0 -- keep going, 1 -- stop
    header_stop = 0
    #value will correspond to angle at which we have to turn
    #full left: 180, full right: 0, straight ahead: 90
    #this value will be sent no matter what, but should only be checked
    #if header_stop = 1
    header_turn = 0
    #find the center of the frame
    center_x = blue.shape[0] // 2
    center_y = blue.shape[1] // 2

    #we're gonna check and see if there's a large amount of blue in the
    #middle/front of the frame (don't want to turn if there's a fleck/glitch)
    #define variables for amount of blue in the frame
    blue_in_frame = 0
    blue_on_left = 0
    blue_on_right = 0

    #for each pixel in the middle of the frame
    for i in range(0,200):
        for j in range(0,200):
            #check top left corner
            if blue[center_x-i, center_y-j] == 255:
                blue_in_frame += 1
                blue_on_left += 1
            #check bottom left corner
            if blue[center_x+i+1, center_y-j] == 255:
                blue_in_frame += 1
                blue_on_left += 1
            #check top right corner
            if blue[center_x-i, center_y+j] == 255:
                blue_in_frame += 1
                blue_on_right += 1
            #check bottom right corner
            if blue[center_x+i+1, center_y+j] == 255:
                blue_in_frame += 1
                blue_on_right += 1
    
    #if there's likely tape here, stop
    if blue_in_frame >= 500:
        header_stop = 1

    if blue_on_right >= blue_on_left:
        header_turn = 135
    elif blue_on_right < blue_on_left:
        header_turn = 45
    
    cv.waitKey(10)
    cv.destroyAllWindows()
