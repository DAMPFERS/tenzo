import statistics as stat
import time
import RPi.GPIO as GPIO



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


def read3(dt1, dt2, dt3, sck):
        """
        _read method reads bits from hx711, converts to INT
        and validate the data.
        
        Returns: (bool || int) if it returns False then it is false reading.
            if it returns int then the reading was correct
        """
        GPIO.output(sck, False)  # start by setting the pd_sck to 0
        ready_counter = 0
        while (((not ready(dt1)) or (not ready(dt2)) or (not ready(dt3))) and ready_counter <= 50):
            time.sleep(0.01)  # sleep for 10 ms because data is not ready
            ready_counter += 1
            if ready_counter == 50:  # if counter reached max value then return False
                return False

        # read first 24 bits of data
        data_in1 = 0  # 2's complement data from hx 711
        data_in2 = 0
        data_in3 = 0
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
            data_in1 = (data_in1 << 1) | GPIO.input(dt1)
            data_in2 = (data_in2 << 1) | GPIO.input(dt2)
            data_in3 = (data_in3 << 1) | GPIO.input(dt3)
        GPIO.output(sck, True)
        GPIO.output(sck, False)
        
        data_in1 = data_in1 ^ 0x800000
        data_in2 = data_in2 ^ 0x800000
        data_in3 = data_in3 ^ 0x800000
        
        return (data_in1, data_in2, data_in3) 
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
	GPIO.setup(19, GPIO.IN)
	GPIO.setup(6, GPIO.IN)
	
	GPIO.setup(20, GPIO.OUT)
	time.sleep(0.3)
	while(1):
		a1, a2, a3 = read3(21, 19, 6, 20)
		print(a1, a2, a3)
	GPIO.cleanup()







if __name__ == "__main__":
    main()
