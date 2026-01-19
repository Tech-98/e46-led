import sys
sys.path.insert(0, 'teensydata')
import json
import numpy as np
import flask_socketio
from flask import Flask, render_template, send_from_directory
from flask_socketio import SocketIO
from os.path import exists
from simple_rpc import Interface
import logging
import time
import win32gui
import win32con
import subprocess
import threading
from time import sleep
import vlc
from PIL import ImageColor
import traceback
from threading import Timer
from full_error import hex_full_error
from seq_cal_2 import hex_seq_cal_2
from seq_cal_3 import hex_seq_cal_3
from prerun_seq import hex_prerun_seq
from playthrough import hex_playthrough

# set max frame to play for each playthrough
error_states_frame = 1469
fade_frame = 114
error_cal_2_frame = 900
error_cal_3_frame = 870
seq_cal_frame = 3630
playthrough_frame = 4200
prerun_seq_frame = 1950
frame = 0
slide_to_play = 0
sending_Frame = False
door_opened = False
door_closed = False
drawer_opened = False
config = json.load(open("config.json"))

error_states_rgb = []


class Stub:
    def __init__(self, name):
        self.name = name

    def __getattr__(self, item):
        print(f"Called {self.name}.{item}")
        return self.blank

    def blank(self, *args, **kwargs):
        pass


subprocess.Popen(["py", "-m", "http.server", "8081"])

log = logging.getLogger('werkzeug')
log.setLevel(logging.INFO)

print("Initialising Arduino connections...")
print("Initialising LED Arduino (Teensy)")
try:
    ledArduino = Interface("COM8")
    print("Connected LED Arduino")
except Exception as e:
    print(f"Using stub class for LED Arduino: {e}")
    ledArduino = Stub("ledArduino")

print("Initialising stepper Arduino")
try:
    stepperArduino = Interface("COM6")
    print("Connected stepper Arduino.")
except Exception as e:
    print(f"Using stub class for stepper Arduino: {e}")
    stepperArduino = Stub("stepperArduino")

print("Arduinos connected.")

ledArduino.setLEDBrightness(102)

app = Flask(__name__, static_folder="blah")
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, cors_allowed_origins="*")

# this is an arrays that contains timestamps and there corresponding simple_rpc methods to be called for controlling the mechs.
# the array is being used when the "AUTOPLAY DEMO" gets played.
mech_timing = [
    [45.5, lambda: stepperArduino.closeDoor(2, 2), False],
    [47, lambda: stepperArduino.openDraw(1.5, 2), False],
    [55.5, lambda: stepperArduino.bucketDrawSimultaneousMotion(
        0.5, 0.5, 2, 0.7), False],
    [59.5, lambda: stepperArduino.closeBucket(2, 0.7), False],
    [61.5, lambda: stepperArduino.openDoor(1.8, 2), False],
]

timeStamps = [0, 1, 2, 6, 12, 17, 28, 36,
              41, 46, 55, 60, 65, 70, 106.5, 137, 137]


def mech_conf(item, type):
    return config["mechanical"][item][type + "Options"][config["mechanical"][item][type + "Choice"]]


def led_speed(sequence):
    return config["sequences"][sequence]["multiplyOptions"][config["sequences"][sequence]["choice"]]


@socketio.on('message')
def handle_message(data):
    # calls appropriate functions on the microcontrollers based on the current slide.
    global stop_Sending_Frame, sending_Frame, slide_to_play
    sending_Frame = True

    print('received message: ' + data)
    if data == "slide advance to 0":
        sending_Frame = False
    if data == "slide advance to 1":
        thread = threading.Thread(target=play_slide)
        thread.start()
        slide_to_play = 0
    if data == "slide advance to 7":
        slide_to_play = 8
    if data == "slide advance to 8":
        slide_to_play = 9
        stepperArduino.closeDoor(
            mech_conf("door", "speed"), mech_conf("door", "accel"))
        stepperArduino.openDraw(
            mech_conf("drawer", "speed"), mech_conf("drawer", "accel"))
    if data == "slide advance to 9":
        slide_to_play = 10
        time.sleep(0.8)
        stepperArduino.bucketDrawSimultaneousMotion(
            mech_conf("drawer", "speed"), mech_conf(
                "drawer", "accel"), mech_conf("bucket", "speed"),
            mech_conf("bucket", "accel"))
    if data == "slide advance to 10":
        slide_to_play = 11
        stepperArduino.closeBucket(
            mech_conf("bucket", "speed"), mech_conf("bucket", "accel"))
        stepperArduino.openDoor(
            mech_conf("door", "speed"), mech_conf("door", "accel"))
    if data == "slide advance to 11":
        slide_to_play = 12
    if data == "slide advance to 12":
        slide_to_play = 13
    if data == "slide advance to 13":
        slide_to_play = 14


@socketio.on("led_command")
def handle_led_command(data):
    compiled_command = "ledArduino." + data
    print("Running: " + compiled_command)
    if "ledArduino.idle" in compiled_command:
        stop_scene()
    eval(compiled_command)


@socketio.on("jog")
def jog(data):
    command = data.split("|")

    if command[0] == "door":
        if command[1] == "open":
            stepperArduino.jogDoorOpen()
        elif command[1] == "close":
            stepperArduino.jogDoorClosed()
    elif command[0] == "bucket":
        if command[1] == "open":
            stepperArduino.jogBucketOpen()
        elif command[1] == "close":
            stepperArduino.jogBucketClosed()
    elif command[0] == "drawer":
        if command[1] == "open":
            stepperArduino.jogDrawOpen()
        elif command[1] == "close":
            stepperArduino.jogDrawClosed()


@socketio.on('settings')
def handle_settings_message(data):
    print('received settings change: ' + data)
    data = data.split("|")
    if data[0] == "open":
        if data[1] == "door":
            stepperArduino.openDoor(
                mech_conf("door", "speed"), mech_conf("door", "accel"))
        elif data[1] == "bucket":
            stepperArduino.openBucket(
                mech_conf("bucket", "speed"), mech_conf("bucket", "accel"))
        elif data[1] == "drawer":
            stepperArduino.openDraw(
                mech_conf("drawer", "speed"), mech_conf("drawer", "accel"))
    elif data[0] == "close":
        if data[1] == "door":
            stepperArduino.closeDoor(
                mech_conf("door", "speed"), mech_conf("door", "accel"))
        elif data[1] == "bucket":
            stepperArduino.closeBucket(
                mech_conf("bucket", "speed"), mech_conf("bucket", "accel"))
        elif data[1] == "drawer":
            stepperArduino.closeDraw(
                mech_conf("drawer", "speed"), mech_conf("drawer", "accel"))
    elif data[0] == "home":
        if data[1] == "door":
            stepperArduino.homeDoor()
        elif data[1] == "bucket":
            stepperArduino.homeBucket()
        elif data[1] == "drawer":
            stepperArduino.homeDraw()
        elif data[1] == "all":
            stepperArduino.homeDoor()
            stepperArduino.homeDraw()
            stepperArduino.homeBucket()
            stepperArduino.openDoor(0.5, 0.5)


@socketio.on('update_config')
def handle_update_config(data):
    global config
    config = data
    json.dump(data, open("config.json", "w"), indent=4)


@socketio.on('request_config')
def handle_config_request():
    print("Config requested, sending.")
    flask_socketio.emit("config_update", json.load(open("config.json")))


@socketio.on('shut_ui')
def handle_shut_ui():
    # close UI window
    def win_enum_handler(hwnd, ctx):
        if win32gui.IsWindowVisible(hwnd) and win32gui.GetWindowText(hwnd) == "Ionic App - Google Chrome":
            print("Closed window", hex(hwnd), win32gui.GetWindowText(hwnd))
            win32gui.PostMessage(hwnd, win32con.WM_CLOSE, 0, 0)

    win32gui.EnumWindows(win_enum_handler, None)


@app.route('/static/<path:path>', methods=['GET'])
def static_proxy(path):
    if exists("static/" + path):
        return send_from_directory('./static', path)
    else:
        return send_from_directory('./templates', 'index.html')


@app.route('/static/')
def root():
    return send_from_directory('./templates', 'index.html')


@socketio.on('stop-scene')
def stop_scene():
    print("stopping scene")
    global sending_Frame
    sending_Frame = False
    time.sleep(0.5)
    try:
        ledArduino.idle()
    except:
        try:
            time.sleep(1)
            ledArduino = Interface("COM8")
            print("Connected LED Arduino")
            ledArduino.idle()
        except Exception as e:
            print(f"Arduino disconnected")
    stepperArduino.setMotorEnable(False)


def run_non_blocking(func):
    t = threading.Thread(target=func)
    t.start()


def play_states(rgb_arr, max_frame, state_name, start_frame=0):
    # send an array of rgb value correspondent to the frame that needs to be played to teensy at the corresponding time
    global sending_Frame, door_opened, door_closed, drawer_opened, frame, mech_timing
    sending_Frame = True
    frame = 0
    print("play state:", state_name)
    startTime = round(time.perf_counter(), 3)
    startTime_for_mechs = round(time.perf_counter(), 3)
    skip_frame = 0

    if state_name == 'playthrough':
        for mechArr in mech_timing:
            mechArr[2] = False

    while True:
        #print("sending frame", sending_Frame)
        if sending_Frame == False:
            return
        timenow = round(time.perf_counter(), 3)
        startTime = round(time.perf_counter(), 3)
        frame = int((timenow - startTime_for_mechs)/0.033) + start_frame
        if frame >= max_frame or frame >= len(rgb_arr):
            print("stopping")
            if "slide" not in state_name:
                stop_scene()
            return
        skip_frame = skip_frame + 1
        try:
            if skip_frame > 30:
                ledArduino.showVectorFrame(rgb_arr[frame])
        except Exception:
            traceback.print_exc()

        if state_name == 'playthrough':
            timenow = round(time.perf_counter(), 3)
            i = 0
            for mechArr in mech_timing:
                if timenow >= (startTime_for_mechs + mech_timing[i][0]) and mech_timing[i][2] == False:
                    print("running mech")
                    mech_timing[i][2] = True
                    run_non_blocking(mechArr[1])
                i += 1


def play_slide():
    global slide_to_play
    frame = 0
    startTime = round(time.perf_counter(), 3)
    slide_playing = 0
    start_frame = 0
    fade_position = 0

    print("playing slide")
    while True:
        if sending_Frame == False:
            return
        if slide_to_play != slide_playing:
            fade_position = 0
            slide_playing = slide_to_play
            startTime = round(time.perf_counter(), 3)
            start_frame = int(timeStamps[slide_to_play] * 30)
        frame = int((round(time.perf_counter(), 3) -
                    startTime)/0.033) + start_frame
        if frame >= timeStamps[slide_to_play+1]*30 and slide_playing != 0:
            frame = timeStamps[slide_to_play+1]*30
        if slide_playing == 0 and frame > 284:
            start_frame = start_frame - 225
        if slide_playing == 8 or slide_playing == 0:
            fade_position = fade_position + 3
            try:
                ledArduino.showFadeinFrame(
                    hex_playthrough[frame], fade_position)
            except Exception:
                traceback.print_exc()
        else:
            try:
                ledArduino.showVectorFrame(hex_playthrough[frame])
            except Exception:
                traceback.print_exc()


@socketio.on('play-scene')
def handle_play_scene_message(data):
    # calls play_states functon to play specific animation. It passes the 2D array of the animation(frame x RGB values of the individual LEDs), the amount of frames to play and the animation name
    if data == 'play-error-states-calibration-1':
        time.sleep(0.1)
        thread = threading.Thread(target=lambda: play_states(
            hex_full_error, error_states_frame, 'error1'))
        thread.start()
    if data == 'play-sequencing-calibration-2':
        thread = threading.Thread(target=lambda: play_states(
            hex_seq_cal_2, seq_cal_frame, 'seq2'))
        thread.start()
    if data == 'play-sequencing-calibration-1':
        thread = threading.Thread(target=lambda: play_states(
            hex_seq_cal_3, seq_cal_frame, 'seq1'))
        thread.start()
    if data == 'play-playthrough':
        time.sleep(0.5)
        thread = threading.Thread(target=lambda: play_states(
            hex_playthrough, playthrough_frame, 'playthrough'))
        thread.start()
        stepperArduino.setMotorEnable(True)
    if data == 'play-prerun-sequencing':
        thread = threading.Thread(target=lambda: play_states(
            hex_prerun_seq, prerun_seq_frame, 'prerun'))
        thread.start()


if __name__ == '__main__':
    socketio.run(app, port=5678, log_output=True)
