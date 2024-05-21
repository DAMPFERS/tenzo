import RPi.GPIO as GPIO
import threading
import numpy as np
import datetime
import time
import sys
import os


ROOT_DIR = "/home/vladr/Downloads"
START_MOTOR_PIN = 12
SCK = 20
DT_LEFT_SENSOR = 21
DT_RIGHT_SENSOR = 19
DT_CENTER_SENSOR = 6

WEIGHT = 3470
VELOCITY = 10


def cleanAndExit():
    GPIO.cleanup()
    sys.exit()
    return



def inputParameters(weight : int, velocity : int) -> tuple:
    '''
    INPUT PARAMETERS
    param weight: grams on scales, need for CENTRAL sensor
    param velocity: angular velosity, cycles per minute
    return NameObject, numberOfCycles, timeCycles, addresDirectory
    '''

    f_object = input("Enter object/material name: ")
    cycles = int(input("Enter number of revolutions: "))
    cycles_time = cycles * 60 / velocity

    print ('Excellent\nLoading...')

    date = time.strftime("%Y-%m-%d", time.localtime())
    objdir = date + '_' + f_object
    filedir = os.path.join(ROOT_DIR,objdir)
    if not os.path.exists(filedir): os.makedirs(filedir)
    return (f_object, cycles, cycles_time, filedir)





	

def contactGpioSettingsV3():
	pass
	GPIO.setwarnings(False)
	GPIO.cleanup()
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(DT_LEFT_SENSOR, GPIO.IN)
	GPIO.setup(DT_RIGHT_SENSOR, GPIO.IN)
	GPIO.setup(DT_CENTER_SENSOR, GPIO.IN)
	GPIO.setup(SCK, GPIO.OUT)
	GPIO.setup(START_MOTOR_PIN, GPIO.OUT, initial=GPIO.LOW)
	time.sleep(0.3)
	
	
  
  
def ready(dt: int) -> bool:
        """
        func check if data is prepared for reading from HX711
		parametr dt: number data pin raspberry pi4
        return: bool True if ready else False when not ready        
        """
        # if DOUT pin is low data is ready for reading
        if GPIO.input(dt) == 0:
            return True
        else:
            return False




def read3(dt1: int, dt2: int, dt3: int, sck: int) -> tuple[int]:
        """
        _read method reads bits from hx711, converts to INT
        and validate the data.
        parametrs dt1, dt2, dt3: number data pins raspberry pi4
        parametr sck: number clocked pin raspberry pi4
        return: (bool || int) if it returns False then it is false reading.
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
    

def frictionProgramV3(weight: int, velocity: int, f_object: str, cycles: int, cycles_time: float, filedir: str) -> bool:
    '''FRICTION PROGRAM'''
    save_time = []
    sensor_left = []
    sensor_center = []
    sensor_right = []

    if input("Start the program? (y/n): ").lower() != 'y' : return False
    
    print (f'''PROGRAM START 
    {f_object}
    {cycles} cycles
    {weight} g 

    TIME       L      C      R ''')

    start_time = time.time()
    GPIO.output(START_MOTOR_PIN, GPIO.HIGH)

    measure_time = 0
    m_time = 0
    while (measure_time <= cycles_time) :
        try:
            val_left, val_center, val_right = read3(DT_LEFT_SENSOR, DT_RIGHT_SENSOR, DT_CENTER_SENSOR, SCK)
            measure_time = time.time() - start_time

            save_time.append(measure_time)
            sensor_left.append(val_left)
            sensor_center.append(val_center)
            sensor_right.append(val_right)
            print(f"{measure_time}      {val_left}  {val_center}  {val_right}")                    
            
        except (KeyboardInterrupt, SystemExit):
            print ('End of the program')
            GPIO.output(START_MOTOR_PIN, GPIO.LOW)
            file_txt = filedir + f"/{cycles}rev.txt"
            file_csv = filedir + f"/{cycles}rev.csv"

            saveResult_TXT(file_txt, save_time, sensor_left, sensor_center, sensor_right)
            saveResult_CSV(file_csv, save_time, sensor_left, sensor_center, sensor_right)
            cleanAndExit()
            return False
    
                
    GPIO.output(START_MOTOR_PIN, GPIO.LOW)
    
    file_txt = filedir + f'/{cycles}rev.txt'
    file_csv = filedir + f'/{cycles}rev.csv'
    
    saveResult_TXT(file_txt, save_time, sensor_left, sensor_center, sensor_right)
    saveResult_CSV(file_csv, save_time, sensor_left, sensor_center, sensor_right)

    print (f'''
    End of the program
    Datafile: {file_csv}''')
    cleanAndExit()
    return True








def saveResult_TXT(file_addr: str, save_time: float, sensor_left: int, sensor_center: int, sensor_right: int) -> bool:
    '''
    Saving result to a txt file
	parametr file_addr: filename addres 
	parametr save_time: time
	parametr sensor_left: left ADC underestimation
	parametr sensor_center: center ADC underestimation
	parametr sensor_right: right ADC underestimation
	return: True if successful
	'''
    with open(file_addr, 'w', encoding='utf-8') as f:
        data = "time  leftSens  centerSens  rightSens "
        f.write(data)
        for i in range(len(save_time)):
            data = f"{save_time[i]}  {sensor_left[i]}  {sensor_center[i]}  {sensor_right[i]} \n"
            f.write(data)
    return True



def saveResult_CSV(file_addr: str, save_time: float, sensor_left: int, sensor_center: int, sensor_right: int) -> bool:
    '''
    Saving result to a csv file
	parametr file_addr: filename addres 
	parametr save_time: time
	parametr sensor_left: left ADC underestimation
	parametr sensor_center: center ADC underestimation
	parametr sensor_right: right ADC underestimation
	return: True if successful '''
    import csv
    with open(file_addr, 'w', encoding='utf-8', newline='') as f:
        writer = csv.writer(f, delimiter=',')
        writer.writerow(["save_time", "sensor_left", "sensor_center", "sensor_right"])
        for i in range(len(save_time)):
            writer.writerow([save_time[i], sensor_left[i], sensor_center[i], sensor_right[i]])
    return True






	


def main():
    f_object, cycles, cycles_time, filedir = inputParameters(weight= WEIGHT, velocity= VELOCITY)
    contactGpioSettingsV3()
    while True:
        res = frictionProgramV3(WEIGHT, VELOCITY, f_object, cycles, cycles_time, filedir)
        if res == False: break
    return    
    
    
    
   
   



    

if __name__ == "__main__":
    main()
        
