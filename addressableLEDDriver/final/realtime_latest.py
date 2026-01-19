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

config = { "teensyport" : "COM3"}
print("Connecting to Teensy at " + config["teensyport"])

try:
    teensy = serial.Serial(config["teensyport"], write_timeout=0)
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
    return [real_width * scale, t]


def horizontal_top(t):
    return [t, top_to_diffuser * scale]


def horizontal_bottom(t):
    return [t, (top_to_diffuser + diffuser_to_diffuser) * scale]


def horizontal_pil(t):
    return [t, (top_to_diffuser + diffuser_to_diffuser / 2) * scale]


real_width = 630 - 2 * 30
real_height = 1545
top_to_diffuser = 590
diffuser_to_diffuser = 160
scale = 0.6

print("start")
r = 280
h = 650
width = 870
h_bottom = 200
h_top = 400
h_pil = 300
t_height = np.linspace(0, real_height * scale, 358)
t_top_width = np.linspace((real_width - 45) * scale, 45 * scale, 125)
t_bottom_width = np.linspace((real_width - 30) * scale, 30 * scale, 135)
t_pil = np.linspace(real_width * scale, 0, 10)
box_x1 = 850
box_y1 = 210
box_x2 = 1850
box_y2 = 1210
box_width = box_x2 - box_x1
box_height = box_y2 - box_y1

sct = mss.mss()
monitor = {"top": box_y1, "left": box_x1, "width": box_width, "height": box_height}

v1 = [vertical_left(n) for n in t_height]
v2 = [vertical_right(n) for n in t_height]
v3 = [horizontal_top(n) for n in t_top_width]
v4 = [horizontal_bottom(n) for n in t_bottom_width]
v5 = [horizontal_pil(n) for n in t_pil]
v = v2 + v1 + v3 + v4
v = [[p[0] + 302, p[1] + 50] for p in v]
v5 = list(np.repeat(v5[1:-1], 8, axis=0))
v5 = [[p[0] + 302, p[1] + 50] for p in v5]
screen = np.array(np.array(sct.grab(monitor)))
# print(screen[:, :, :-1].shape)
plt.imshow(screen)


plt.scatter(*zip(*v), s=4, color="red")
plt.scatter(*zip(*v5), s=4, color="red")
plt.show()

start = time.time_ns()

while True:
    # print("Loop")
    screen = cv2.cvtColor(np.array(sct.grab(monitor)), cv2.COLOR_RGBA2RGB)
    try:
        colors = [get_subpixel(screen, p[0], p[1]) for p in v]
        pill_colors = [get_subpixel(screen, p[0], p[1]) for p in v5]

        data = list(chain.from_iterable((int(color[2]), int(color[1]), int(color[0])) for color in colors))
        data += list(chain.from_iterable((int(color[2]), int(color[1]), int(color[0]), 0) for color in pill_colors))
        time.sleep(0.05)
        # print(len(data))
        # break
        teensy.write(bytearray(data))
    except Exception as e:
        print("Error: " + repr(e))