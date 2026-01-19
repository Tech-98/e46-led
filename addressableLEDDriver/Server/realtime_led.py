import time

import cv2
import matplotlib.pyplot as plt
import numpy as np
from PIL import ImageGrab
import json
# from mss import mss
from PIL import Image
from itertools import chain
import serial
import mss

config = json.load(open('config.json'))
print("Connecting to Teensy at " + config["teensyport"])

try:
    teensy = serial.Serial(config["teensyport"])
    print("Connected to Teensy")
except Exception as e:
    print("Couldn't connect to Teensy:\n" + repr(e) + "\nExiting...")
    exit()


def get_subpixel(img: cv2.Mat, x: float, y: float):
    """Get interpolated pixel value at (@x, @y) with float precision"""
    patch = cv2.getRectSubPix(img, (1, 1), (x, y), np.zeros((1, 1)), cv2.CV_32F)
    if patch is not None:
        return patch[0][0]
    return None


def capture_screenshot():
    #print("capturing screen")
    # Capture entire screen
    with mss() as sct:
        monitor = sct.monitors[1]
        sct_img = sct.grab(monitor)
        # Convert to PIL/Pillow Image
        #print("captured")
        return Image.frombytes('RGB', sct_img.size, sct_img.bgra, 'raw', 'BGRX')


def vertical_left(t):
    return [0, t]


def vertical_right(t):
    return [width, t]


def horizontal_top(t):
    return [t, h_top]


def horizontal_bottom(t):
    return [t, h_bottom]


def horizontal_pil(t):
    return [t, h_pil]


print("start")
r = 280
h = 650
width = 870
h_bottom = 200
h_top = 400
h_pil = 300
t_height = np.linspace(0, h, 358)
t_top_width = np.linspace(0, width, 125)
t_bottom_width = np.linspace(0, width, 135)
t_pil = np.linspace(0, width, 10)
box_x1 = 500
box_y1 = 10
box_x2 = 1500
box_y2 = 750
box_width = box_x2 - box_x1
box_height = box_y2 - box_y1

sct = mss.mss()
monitor = {"top": box_y1, "left": box_x1, "width": box_width, "height": box_height}

v1 = [vertical_left(n) for n in t_height]
v2 = [vertical_right(n) for n in t_height]
v3 = [horizontal_top(n) for n in t_top_width]
v4 = [horizontal_bottom(n) for n in t_bottom_width]
# v5 = [horizontal_pil(n) for n in t_pil]
v = v1 + v2 + v3 + v4 #+ v5
v = [[p[0] + 50, p[1] + 50] for p in v]
screen = np.array(np.array(sct.grab(monitor)))
# print(screen[:, :, :-1].shape)
plt.imshow(screen)
plt.scatter(*zip(*v), s=4, color="red")
plt.show()

start = time.time_ns()

while True:
    # print("Loop")
    screen = cv2.cvtColor(np.array(sct.grab(monitor)), cv2.COLOR_RGBA2RGB)
    try:
        colors = [get_subpixel(screen, p[0], p[1]) for p in v]

        data = list(chain.from_iterable((int(color[2]), int(color[1]), int(color[0])) for color in colors))

        # print(data)

        teensy.write(bytearray(data))

        time.sleep(0.1)
    except Exception as e:
        print("Error: " + repr(e))
