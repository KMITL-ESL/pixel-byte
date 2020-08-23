import cv2
import serial
import time

def show_webcam(ser, mirror=False):
    cam = cv2.VideoCapture(0)

    prevTime = time.time()
    
    lastReport = 0
    frame = 0
    fps = 0

    while True:
        ret_val, img = cam.read()
        if mirror:
            img = cv2.flip(img, 1)
        dim = (30, 30)
        img = cv2.resize(img, dim, interpolation=cv2.INTER_CUBIC)
        for x in range(int(img.shape[0] / 10)):
            for y in range(int(img.shape[1] / 10)):
                # if x != 0 or y != 0:
                #     continue
                ser.write([ord('n'), (x * 10) + y + 100, ord('\n')])
                buff = bytearray(300)
                for i in range(10):
                    for j in range(10):
                        color = img[x * 10 + i, y * 10 + j]
                        index = ((i * 10) + j) * 3
                        buff[index + 0] = int(color[2] * 1)
                        buff[index + 1] = int(color[1] * 0.85)
                        buff[index + 2] = int(color[0] * 0.65)
                ser.write(buff)
        ser.write([ord('x'), ord('\n')])
#        time.sleep(0.0015)

        #while ser.inWaiting():
        #    a = ser.read_until()
        #    print("aa", a)

        dim = (300, 300)
        img2 = cv2.resize(img, dim, fx=0, fy=0, interpolation=cv2.INTER_NEAREST)

        curTime = time.time()
        frame = frame + 1
        
        if (int(curTime) != lastReport):
            lastReport = int(curTime)
            
            sec = curTime - prevTime
            prevTime = curTime

            fps = frame / sec
            frame = 0
#            print(fps)

        # if fps < 2:
        #     break

        str = "FPS : %0.2f" % fps
        cv2.putText(img2, str, (0, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0))

        cv2.imshow('my webcam', img2)
        if cv2.waitKey(1) == 27:
            break  # esc to quit
    cv2.destroyAllWindows()


def main():
    ser = serial.Serial()
    ser.port = 'COM10'
    ser.baudrate = 500000
    ser.bytesize = serial.EIGHTBITS
    ser.stopbits = serial.STOPBITS_ONE
    ser.parity = serial.PARITY_NONE
    ser.timeout = 0.4
    ser.open()

    show_webcam(ser=ser, mirror=False)

    ser.close()

if __name__ == '__main__':
    main()
