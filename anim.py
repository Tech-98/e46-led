from simple_rpc import Interface
import time
from seq_cal_3 import hex_seq_cal_3

sending_Frame = False
led = Interface('COM5')

def red():
    led.showColor(255, 0, 0,300)

def fade(r,g,b, duration, increment):
    for i in range(increment):
            led.showColor(int(r*i/increment), int(g*i/increment), int(b*i/increment), 30)
            print(int(r*i/increment),",",int(g*i/increment),",",int(b*i/increment))
            time.sleep(duration/increment)
# fade(1,1,255, 5, 100)
            
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
    # stepperArduino.setMotorEnable(False)

def play_seq(rgb_arr=hex_seq_cal_3, max_frame=3630, state_name='seq1', start_frame=0):
    # (target=lambda: play_states(
    #         hex_seq_cal_3, seq_cal_frame, 'seq1'))
    # send an array of rgb value correspondent to the frame that needs to be played to teensy at the corresponding time
    global sending_Frame, door_opened, door_closed, drawer_opened, frame, mech_timing
    sending_Frame = True
    frame = 0
    print("play state:", state_name)
    startTime = round(time.perf_counter(), 3)
    startTime_for_mechs = round(time.perf_counter(), 3)
    skip_frame = 0

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
                led.showVectorFrame(rgb_arr[frame])
        except Exception:
            # traceback.print_exc()
            pass

        
# r = 40
# g = 80
# b = 200
l = 117  
r = 223
g= 255
b= 0
# led.fadeIn(r,g,b,1,100,l)
led.blank()
time.sleep(1)
led.fillIn(r,g,b,300,5,0.15)
# led.fadeOut(r,g,b,1,100,l)