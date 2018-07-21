#import the necessary modules
import freenect
import cv2
import numpy as np
from math import tan

#function to get RGB image from kinect
def get_video():
    array,_ = freenect.sync_get_video()
    array = cv2.cvtColor(array,cv2.COLOR_RGB2BGR)
    return array
 
#function to get depth image from kinect
def get_depth():
    array,_ = freenect.sync_get_depth()
    '''distance = []
    for i in array:
    	row = []
    	for j in i:
    	    row.append(0.1236 * tan(j / 2842.5 + 1.1863))
    	distance.append(row)
    print(str(distance))'''
    array = array.astype(np.uint8)
    return array
 
if __name__ == "__main__":
    while 1:
        #get a frame from RGB camera
        frame = get_video()
        #get a frame from depth sensor
        depth = get_depth()
        #display RGB image
        cv2.imshow('RGB image',frame)
        #display depth image
        cv2.imshow('Depth image',depth)
 
        # quit program when 'esc' key is pressed
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break
    cv2.destroyAllWindows()
