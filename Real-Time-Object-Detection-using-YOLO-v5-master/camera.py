import numpy as np
import pandas as pd
import time
#import torch
#import torch.nn as nn
import cv2
from utility import *
from yolo import Darknet
import random
import argparse
import pickle as pkl
import collections
import socket
import requests


import datetime as dt
t = dt.datetime.now()

# khai báo api
url = 'https://6paggkxn3f.execute-api.ap-southeast-1.amazonaws.com/test/post'

def arg_parse():
    """Parsing arguments"""
    parser = argparse.ArgumentParser(description='YOLO v3 Real Time Detection')
    parser.add_argument("--confidence", dest="confidence", help="Object Confidence to filter predictions", default=0.25)
    parser.add_argument("--nms_thresh", dest="nms_thresh", help="NMS Threshhold", default=0.4)
    parser.add_argument("--reso", dest='reso', help=
    "Input resolution of the network. Increase to increase accuracy. Decrease to increase speed",
                        default="160", type=str)
    return parser.parse_args()


def prep_image(img, inp_dim):
    """Converting a numpy array of frame into PyTorch tensor"""
    orig_im = img
    dim = orig_im.shape[1], orig_im.shape[0]
    img = cv2.resize(orig_im, (inp_dim, inp_dim))
    img_ = img[:, :, ::-1].transpose((2, 0, 1)).copy()
    img_ = torch.from_numpy(img_).float().div(255.0).unsqueeze(0)
    return img_, orig_im, dim


def write1(x, img):
    c1 = tuple(x[1:3].int())
    c2 = tuple(x[3:5].int())
    cls = int(x[-1])
    label1 = "{0}".format(classes1[cls])
    color = random.choice(colors1)
    cv2.rectangle(img, (int(c1[0]), int(c1[1])), (int(c2[0]), int(c2[1])), color, 1)
    
    t_size = cv2.getTextSize(label1, cv2.FONT_HERSHEY_PLAIN, 1, 1)[0]
    c2 = c1[0] + t_size[0] + 3, c1[1] + t_size[1] + 4
    cv2.rectangle(img, (int(c1[0]), int(c1[1])), (int(c2[0]), int(c2[1])), color, -1)
    cv2.putText(img, label1, (int(c1[0]), int(c1[1] + t_size[1] + 4)),
                cv2.FONT_HERSHEY_PLAIN, 1, [225, 255, 255], 1)
    
    #count vehicles
    global count1
    if label1 == 'motorbike':
        count1 += 2
    elif label1 =='car' :
        count1 += 6
    elif label1 == 'truck' and 'bus' :
        count1 += 10
    return img

def write2(x, img):
    c1 = tuple(x[1:3].int())
    c2 = tuple(x[3:5].int())
    cls = int(x[-1])
    label2 = "{0}".format(classes2[cls])
    color = random.choice(colors2)
    cv2.rectangle(img, (int(c1[0]), int(c1[1])), (int(c2[0]), int(c2[1])), color, 1)
    
    t_size = cv2.getTextSize(label2, cv2.FONT_HERSHEY_PLAIN, 1, 1)[0]
    c2 = c1[0] + t_size[0] + 3, c1[1] + t_size[1] + 4
    cv2.rectangle(img, (int(c1[0]), int(c1[1])), (int(c2[0]), int(c2[1])), color, -1)
    cv2.putText(img, label2, (int(c1[0]), int(c1[1] + t_size[1] + 4)),
                cv2.FONT_HERSHEY_PLAIN, 1, [225, 255, 255], 1)
    
    #count vehicles
    global count2
    if label2 == 'motorbike':
        count2 += 2
    elif label2 =='car' :
        count2 += 6
    elif label2 == 'truck' and 'bus' :
        count2 += 10
    return img


if __name__ == "__main__":
    cfgfile = "D:/IEC/Real-Time-Object-Detection-using-YOLO-v3-master/Real-Time-Object-Detection-using-YOLO-v3-master/cfg/yolov3.cfg"
    weightsfile = "D:/IEC/Real-Time-Object-Detection-using-YOLO-v3-master/Real-Time-Object-Detection-using-YOLO-v3-master/weights/yolov3.weights"
    num_classesp = 80

    args = arg_parse()
    confidence = float(args.confidence)
    nms_thesh = float(args.nms_thresh)


    num_classes = 80
    bbox_attrs = 5 + num_classes

    model1 = Darknet(cfgfile).to(device)
    model1.load_weights(weightsfile)
    model2 = Darknet(cfgfile).to(device)
    model2.load_weights(weightsfile)


    model1.network_info["height"] = args.reso
    inp_dim1 = int(model1.network_info["height"])
    model2.network_info["height"] = args.reso
    inp_dim2 = int(model1.network_info["height"])



    assert inp_dim1 % 32 == 0
    assert inp_dim1 > 32
    assert inp_dim2 % 32 == 0
    assert inp_dim2 > 32



    model1.eval()
    model2.eval()

    cap1 = cv2.VideoCapture('D:\IEC\Real-Time-Object-Detection-using-YOLO-v3-master\Real-Time-Object-Detection-using-YOLO-v3-master/traffic1.mp4')
    cap1.set(3,160)
    cap1.set(4,120)
    cap2 = cv2.VideoCapture('D:\IEC\Real-Time-Object-Detection-using-YOLO-v3-master\Real-Time-Object-Detection-using-YOLO-v3-master/traffic2.mp4')
    cap2.set(3,160)
    cap2.set(4,120)       
    #cap = cv2.VideoCapture(0)
    assert cap1.isOpened(), 'Cannot capture source'
    assert cap2.isOpened(), 'Cannot capture source'
    count1 = 0
    frames1 = 0
    count2 = 0
    frames2 = 0

    count101 = 0
    count102 = 0
    max1 = 0
    max2 = 0
    m1=0
    m2=0
    i = 0
    dem = 0

    maxx1 = 0
    maxx2 = 0
    classes1 = load_classes('D:/IEC/Real-Time-Object-Detection-using-YOLO-v3-master/Real-Time-Object-Detection-using-YOLO-v3-master/data/coco.names')
    colors1 = pkl.load(open("D:/IEC/Real-Time-Object-Detection-using-YOLO-v3-master/Real-Time-Object-Detection-using-YOLO-v3-master/color/pallete", "rb"))
    while True:
        i=i+1
        
        ret1, frame1 = cap1.read()
        if ret1:
            
            img1, orig_im1, dim1 = prep_image(frame1, inp_dim1)
            img1.to(device)

            output1 = model1(img1)
            output1 = write_results(output1, confidence, num_classes, nms_conf=nms_thesh)


            output1[:, 1:5] = torch.clamp(output1[:, 1:5], 0.0, float(inp_dim1)) / inp_dim1

            output1[:, [1, 3]] *= frame1.shape[1]
            output1[:, [2, 4]] *= frame1.shape[0]

        

            count1 = 0
        list(map(lambda x: write1(x, orig_im1), output1))
        cv2.imshow("frame1", orig_im1)
 
        ret2, frame2 = cap2.read()
        if ret2:

            img2, orig_im2, dim2 = prep_image(frame2, inp_dim2)
            img2.to(device)

            output2 = model2(img2)
            output2 = write_results(output2, confidence, num_classes, nms_conf=nms_thesh)


            output2[:, 1:5] = torch.clamp(output2[:, 1:5], 0.0, float(inp_dim2)) / inp_dim2

            output2[:, [1, 3]] *= frame2.shape[1]
            output2[:, [2, 4]] *= frame2.shape[0]

            classes2 = load_classes('D:/IEC/Real-Time-Object-Detection-using-YOLO-v3-master/Real-Time-Object-Detection-using-YOLO-v3-master/data/coco.names')
            colors2 = pkl.load(open("D:/IEC/Real-Time-Object-Detection-using-YOLO-v3-master/Real-Time-Object-Detection-using-YOLO-v3-master/color/pallete", "rb"))


            count2 = 0
            list(map(lambda x: write2(x, orig_im2), output2))
            #time.sleep(2);  
            if count101 == 1 :
                count101 = 0
                 
                m1=max1
                max1 = 0
            else :
                count101 += 1
                if max1 < count1 :
                    max1 = count1

            if count102 == 1 :
                count102 = 0

                m2=max2
                max2 = 0
            else :
                count102 += 1
                if max2 < count2 :
                    max2 = count2
            if m1 != maxx1 and m1 != 0 and m2 != maxx2 and m2 != 0 :
                s1 = str(maxx1)
                s2 = str(maxx2)
                # truyền dữ liệu 
                myobj = {'traffic 1':s1,'traffic 2':s2}
                if i % 4 == 0 :
                    x = requests.post(url, json = myobj)
                    print(x.text)
                    if (maxx1 > maxx2):
                        dem = dem+1
                        if (dem > 4):
                            print(dem*5,"+ 5s")      
                    if (maxx1<maxx2):
                        dem = 0     
                maxx2 = m2
                maxx1 = m1    
            cv2.imshow("frame2", orig_im2)
        key = cv2.waitKey(1)
        if key & 0xFF == ord('q'):
            break
