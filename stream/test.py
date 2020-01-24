"""
Simply display the contents of the webcam with optional mirroring using OpenCV 
via the new Pythonic cv2 interface.  Press <esc> to quit.
"""

import cv2

import serial

import time


def show_webcam(ser, mirror=False):
    cam = cv2.VideoCapture(1)
    # cam.set(cv2.CAP_PROP_FRAME_WIDTH, 1270)
    # cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
    # print("fps", cam.get(cv2.CAP_PROP_FPS))
    # print("width", cam.get(cv2.CAP_PROP_FRAME_WIDTH))
    # print("height", cam.get(cv2.CAP_PROP_FRAME_HEIGHT))

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
                ser.write([ord('n'), x * 10 + y + 100, ord('\n')])
                buff = bytearray(300)
                for i in range(10):
                    for j in range(10):
                        color = img[x * 10 + i, y * 10 + j]
                        index = ((i * 10) + j) * 3
                        buff[index + 0] = color[2]
                        buff[index + 1] = color[1]
                        buff[index + 2] = color[0]
                ser.write(buff)
        ser.write([ord('x'), ord('\n')])

        while ser.inWaiting():
            a = ser.read_until()
            print("aa", a)

        dim = (300, 300)
        img2 = cv2.resize(img, dim, fx=0, fy=0,
                          interpolation=cv2.INTER_NEAREST)
        cv2.imshow('my webcam', img2)
        if cv2.waitKey(1) == 27:
            break  # esc to quit
    cv2.destroyAllWindows()


def main():
    ser = serial.Serial()
    ser.port = 'COM14'
    ser.baudrate = 500000
    ser.bytesize = serial.EIGHTBITS
    ser.stopbits = serial.STOPBITS_ONE
    ser.parity = serial.PARITY_NONE
    ser.timeout = 0.4
    ser.open()

    show_webcam(ser=ser, mirror=False)


if __name__ == '__main__':
    main()
