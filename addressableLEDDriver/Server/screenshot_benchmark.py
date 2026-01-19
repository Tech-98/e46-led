import time
import mss
from PIL import ImageGrab
import numpy as np
import dxcam

box_x1 = 500
box_y1 = 10
box_x2 = 1500
box_y2 = 750
width = box_x2 - box_x1
height = box_y2 - box_y1


def timeit(func):
    start = time.time_ns()

    n = 100

    for i in range(n):
        func()

    end = time.time_ns()

    print(f"{func} took {(end - start) / (n * 1000 * 1000)}ms on average")


screen = np.array(ImageGrab.grab(bbox=(box_x1, box_y1, box_x2, box_y2)))


def imgrab():
    screen = np.array(ImageGrab.grab(bbox=(box_x1, box_y1, box_x2, box_y2)))


sct = mss.mss()
monitor = {"top": box_y1, "left": box_x1, "width": width, "height": height}


def mssimgrab():
    img_array = np.array(sct.grab(monitor))


camera = dxcam.create()  # returns a DXCamera instance on primary monitor
region = (box_x1, box_y1, box_x2, box_y2)


camera.start(region=region)

start = time.time_ns()

for i in range(100):
    test = camera.get_latest_frame()

end = time.time_ns()

print(f"dxcam took {(end - start) / (100 * 1000 * 1000)}ms on average")

timeit(imgrab)
timeit(mssimgrab)
