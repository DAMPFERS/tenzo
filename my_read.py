import statistics as stat
import time

import RPi.GPIO as GPIO

def init(dt, sck):
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(sck, GPIO.OUT)  # pin _pd_sck is output only
    GPIO.setup(dt, GPIO.IN)  # pin _dout is input only

    time.sleep(0.5)


def ready(dt):
        """
        _ready method check if data is prepared for reading from HX711

        Returns: bool True if ready else False when not ready        
        """
        # if DOUT pin is low data is ready for reading
        if GPIO.input(dt) == 0:
            return True
        else:
            return False


def read(dt, sck):
        """
        _read method reads bits from hx711, converts to INT
        and validate the data.
        
        Returns: (bool || int) if it returns False then it is false reading.
            if it returns int then the reading was correct
        """
        GPIO.output(sck, False)  # start by setting the pd_sck to 0
        ready_counter = 0
        while (not ready(dt) and ready_counter <= 50):
            time.sleep(0.01)  # sleep for 10 ms because data is not ready
            ready_counter += 1
            if ready_counter == 50:  # if counter reached max value then return False
                return False

        # read first 24 bits of data
        data_in = 0  # 2's complement data from hx 711
        for _ in range(24):
            start_counter = time.perf_counter()
            # request next bit from hx 711
            GPIO.output(sck, True)
            GPIO.output(sck, False)
            end_counter = time.perf_counter()
            if end_counter - start_counter >= 0.00006:  # check if the hx 711 did not turn off...
                # if pd_sck pin is HIGH for 60 us and more than the HX 711 enters power down mode.
                print("TIME_ERROR")
                return False
            # Shift the bits as they come to data_in variable.
            # Left shift by one bit then bitwise OR with the new bit.
            data_in = (data_in << 1) | GPIO.input(dt)
        GPIO.output(sck, True)
        GPIO.output(sck, False)
        
        data_in = data_in ^ 0x800000
        
        return data_in
        '''if (data_in == 0x7fffff
                or  # 0x7fffff is the highest possible value from hx711
                data_in == 0x800000
           ):  # 0x800000 is the lowest possible value from hx711
            print('Invalid data detected')
            return False  # rturn false because the data is invalid

        # calculate int from 2's complement
        signed_data = 0
        # 0b1000 0000 0000 0000 0000 0000 check if the sign bit is 1. Negative number.
        if (data_in & 0x800000):
            signed_data = -(
                (data_in ^ 0xffffff) + 1)  # convert from 2's complement to int
        else:  # else do not do anything the value is positive number
            signed_data = data_in
        return signed_data'''




def main():
	GPIO.setwarnings(False)
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(21, GPIO.IN)
	GPIO.setup(20, GPIO.OUT)
	time.sleep(0.3)
	while(1):
		res = read(21,20)
		if res != -1:
			print(res)
	GPIO.cleanup()







if __name__ == "__main__":
    main()
