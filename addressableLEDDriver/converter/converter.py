import cv2
import matplotlib.pyplot as plt
import numpy as np
import json
import time
import sys
from PIL import ImageGrab


# gsudo python -m http.server

# MAX FRAME = 4890 (~2min45s)

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
    return [width, t]


def horizontal_top(t):
    return [t, h_top]


def horizontal_bottom(t):
    return [t, h_bottom]


def horizontal_pil(t):
    return [t, h_pil]


def write_file_hex():
    i = 0

    with open('teensyD/lightbar21.py', 'w') as f:
        print("\nwritting to file 'teensy/source.h'");
        f.write(
            f"hex =["
        )
        for arr in carray:
            arr = arr[+1:] + arr[:+1]

            # print(i)
            f.write("\n\"5559d0")
            for arrTime in range(136):
                hex = "," + '{:02x}{:02x}{:02x}'.format(int(arr[arrTime][0]), int(arr[arrTime][1]),
                                                        int(arr[arrTime][2]))
                f.write(hex)
            f.write(",5559d0\",")
        f.write("]")


def write_file():
    print("writing")
    with open('led_data3.dat', 'wb') as f:
        print("\nwritting to file 'led_data.dat'")
        for frame in carray:
            for led in frame:
                f.write(int(led[0]).to_bytes(1, "big") + int(led[1]).to_bytes(1, "big") + int(led[2]).to_bytes(1, "big"))
            # break

try:
    videoFile = sys.argv[1]
except:
    videoFile = "movinganim.mp4"
try:
    frames_to_process = int(sys.argv[2])
except:
    frames_to_process = 5 * 30

if frames_to_process > 4890:
    frames_to_process = 4890

print("\nfile:", videoFile)
print("frames to process", frames_to_process, "\n")

printProgressBar(0, 100, prefix='Progress:', suffix='Complete', length=50)
r = 280
# h = 650
# width = 870
h = 2050
width = 2070
h_bottom = 1000
h_top = 1200
h_pil = 1100
# h_bottom = 200
# h_top = 400
# h_pil = 300p
t_height = np.linspace(0, h, 225 + 135)
t_width = np.linspace(0, width, 92 + 20 + 8)
t_pil = np.linspace(0, width, 10)
box_x = 500
box_y = 180
box_width = 2050
box_height = 1000

t = np.linspace(0, (2 * r * np.pi + 2 * h), 380)

v1 = [vertical_left(n) for n in t_height]
v2 = [vertical_right(n) for n in t_height]
v3 = [horizontal_top(n) for n in t_width]
v4 = [horizontal_bottom(n) for n in t_width]
v5 = [horizontal_pil(n) for n in t_pil]
v = v1 + v2 + v3 + v4 + v5
v = [[p[0] + 60, p[1] + 160] for p in v]

vidcap = cv2.VideoCapture(videoFile)
success, image = vidcap.read()
plt.imshow(image)
plt.scatter(*zip(*v), s=4, color="red")
plt.show()
carray = []
prog_interval = round(frames_to_process / 100)

for j in range(frames_to_process):
    success, image = vidcap.read()
    try:
        im = cv2.blur(src=cv2.cvtColor(image, cv2.COLOR_BGR2RGB), ksize=(5, 5))
        colors = [get_subpixel(im, p[0], p[1]) for p in v]
        carray.append(colors)
        printProgressBar(j + 1, frames_to_process, prefix='Processing:', suffix='Complete', length=50)
    except:
        pass

print(carray[0])

write_file()

plt.imshow(np.array(carray) / 255)
plt.show()


