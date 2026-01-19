from datetime import datetime
import cv2
import matplotlib.pyplot as plt
import numpy as np
from itertools import chain


def get_subpixel(img: cv2.Mat, x: float, y: float):
    """Get interpolated pixel value at (@x, @y) with float precision"""
    patch = cv2.getRectSubPix(img, (1, 1), (x, y), np.zeros((1, 1)), cv2.CV_32F)
    if patch is not None:
        return patch[0][0]
    return None


def printProgressBar(iteration, total, prefix='', suffix='', decimals=1, length=100, fill='█', printEnd="\r"):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
        printEnd    - Optional  : end character (e.g. "\r", "\r\n") (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end=printEnd)
    # Print New Line on Complete
    if iteration == total:
        print()


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

v1 = [vertical_left(n) for n in t_height]
v2 = [vertical_right(n) for n in t_height]
v3 = [horizontal_top(n) for n in t_top_width]
v4 = [horizontal_bottom(n) for n in t_bottom_width]
v5 = [horizontal_pil(n) for n in t_pil]
v = v2 + v1 + v3 + v4

x_offset = 334
y_offset = 0

v = [[p[0] + x_offset, p[1] + y_offset] for p in v]
v5 = list(np.repeat(v5[1:-1], 8, axis=0))
v5 = [[p[0] + x_offset, p[1] + y_offset] for p in v5]

videoFile = "20230803_Nebula_Lighting state_LB_Full play through V3.mp4"
vidcap = cv2.VideoCapture(videoFile)

frames_to_process = vidcap.get(cv2.CAP_PROP_FRAME_COUNT)

print("Frames to process: " + str(frames_to_process))

success, screen = vidcap.read()

plt.imshow(screen)
plt.scatter(*zip(*v), s=4, color="red")
plt.scatter(*zip(*v5), s=4, color="red")
plt.show()

# exit()

# printProgressBar()

now = datetime.now()

current_time = now.strftime("%H_%M_%S")

f = open("led_data_" + current_time + ".dat", "wb")

for i in range(int(frames_to_process)):
    printProgressBar(i, frames_to_process, prefix='Processing:', suffix='Complete', length=50)
    success, screen = vidcap.read()
    try:
        colors = [get_subpixel(screen, p[0], p[1]) for p in v]
        pill_colors = [get_subpixel(screen, p[0], p[1]) for p in v5]

        data = list(chain.from_iterable((int(color[2]), int(color[1]), int(color[0])) for color in colors))
        data += list(chain.from_iterable((int(color[2]), int(color[1]), int(color[0]), 0) for color in pill_colors))

        f.write(bytearray(data))
    except Exception as e:
        print("Error: " + repr(e))

print("COMPLETE")
