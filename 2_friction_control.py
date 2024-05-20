import RPi.GPIO as GPIO
import threading
import numpy as np
import datetime
import time
import sys
import os

from hx711_v2 import HX711

ROOT_DIR = "/home/vladr/Downloads"
START_MOTOR_PIN = 12
LEFT_SENSOR = (21, 20) # (DT, SCK)
RIGHT_SENSOR = (19, 26) # (DT, SCK)
CENTER_SENSOR = (6, 13) # (DT, SCK)
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





def contactGpioSettingsV2() -> tuple:
    '''CONTACT GPIO SETTINGS'''
    GPIO.cleanup()
    GPIO.setmode(GPIO.BCM)  # set GPIO pin mode to BCM numbering
    GPIO.setup(START_MOTOR_PIN, GPIO.OUT, initial=GPIO.LOW)

    # Create an object hx which represents your real hx711 chip
    # Required input parameters are only 'dout_pin' and 'pd_sck_pin'
    # If you do not pass any argument 'gain_channel_A' then the default value is 128
    # If you do not pass any argument 'set_channel' then the default value is 'A'
    # you can set a gain for channel A even though you want to currently select channel B
    hx_left = HX711(
        dout_pin=LEFT_SENSOR[0], pd_sck_pin=LEFT_SENSOR[1], gain_channel_A=128, select_channel='A')
    hx_center = HX711(
        dout_pin=CENTER_SENSOR[0], pd_sck_pin=CENTER_SENSOR[1], gain_channel_A=128, select_channel='A')
    hx_right = HX711(
        dout_pin=RIGHT_SENSOR[0], pd_sck_pin=RIGHT_SENSOR[1], gain_channel_A=128, select_channel='A')
    return (hx_left, hx_center, hx_right)
    # Before we start, reset the hx711 ( not necessary)
    # you can check if the reset was successful
    '''if (hx_left.reset() or hx_center.reset() or hx_right.reset):  
        print('not ready')
        return None
    else: print('Ready to use')
    
    data = (hx_left.get_raw_data_mean(readings=2), hx_center.get_raw_data_mean(readings=2), hx_right.get_raw_data_mean(readings=2))
    
    if data[0] and data[1] and data[2]:  # always check if you get correct value or only False
        print('Value data correct! :)')
        return (hx_left, hx_center, hx_right)
    else:
        print('invalid data :(')
        return None'''
   

    # Read data several, or only one, time and return mean value
    # argument "readings" is not required default value is 30
    

def frictionProgramV2(weight, velocity, f_object, cycles, cycles_time, filedir, hx_left, hx_center, hx_right) -> bool:
    '''FRICTION PROGRAM'''
    save_time = []
    sensor_left = []
    sensor_center = []
    sensor_right = []
    friction_coefficient = []

    if input("Start the program? (y/n): ").lower() != 'y' : return None
    
    print (f'''PROGRAM START 
    {f_object}
    {cycles} cycles
    {weight} g 

    TIME       L      C      R    FC''')

    start_time = time.time()
    GPIO.output(START_MOTOR_PIN, GPIO.HIGH)

    measure_time = 0
    m_time = 0
    while measure_time <= cycles_time:
        try:
            val_left = hx_left.get_raw_data_mean(readings=1)     #Read not filter
            val_center = hx_center.get_raw_data_mean(readings=1) #Read not filter
            val_right = hx_right.get_raw_data_mean(readings=1)   #Read not filter
            fc = 0
            measure_time = time.time() - start_time

            save_time.append(measure_time)
            sensor_left.append(val_left)
            #sensor_center.append(val_center+weight*5)
            sensor_center.append(val_center)
            sensor_right.append(val_right)
            friction_coefficient.append(fc)

            #print('%.6f      %d  %d  %d      %.3f'%(measure_time, val_left, val_center+weight*5, val_right, fc))
            print(f"{measure_time}      {val_left}  {val_center}  {val_right}      {fc}")                    
            
        except (KeyboardInterrupt, SystemExit):
            print ('End of the program')
            GPIO.output(START_MOTOR_PIN, GPIO.LOW)
            '''
            file = filedir + '/%srev.txt'%(cycles)
            data = np.column_stack((save_time, sensor_left, sensor_center, sensor_right, friction_coefficient))
            np.savetxt(file, data, fmt = '%.3f\t%d\t%d\t%d\t%.3f')
            '''
            cleanAndExit()
            return None
                
    GPIO.output(START_MOTOR_PIN, GPIO.LOW)

    file_txt = filedir + f'/{cycles}rev.txt'
    file_csv = filedir + f'/{cycles}rev.csv'
    
    saveResult_TXT(file_txt, save_time, sensor_left, sensor_center, sensor_right, friction_coefficient)
    saveResult_CSV(file_csv, save_time, sensor_left, sensor_center, sensor_right, friction_coefficient)

    print (f'''
    End of the program
    Datafile: {file_csv}''')
    cleanAndExit()
    return True








def saveResult_TXT(file_addr, save_time, sensor_left, sensor_center, sensor_right, friction_coefficient):
    with open(file_addr, 'w', encoding='utf-8') as f:
        data = "time  leftSens  centerSens  rightSens  fricCoef"
        f.write(data)
        for i in range(len(save_time)):
            data = f"{save_time[i]}  {sensor_left[i]}  {sensor_center[i]}  {sensor_right[i]}  {friction_coefficient[i]}"
            f.write(data)
    return True



def saveResult_CSV(file_addr, save_time, sensor_left, sensor_center, sensor_right, friction_coefficient):
    import csv
    with open(file_addr, 'w', encoding='utf-8') as f:
        writer = csv.writer(f, delimiter=',')
        writer.writerow(["save_time", "sensor_left", "sensor_center", "sensor_right", "friction_coefficient"])
    #with open(file_addr, 'a', encoding="utf-8") as f:
        #writer = csv.writer(f, delimiter=';')
        for i in range(len(save_time)):
            writer.writerow([save_time[i], sensor_left[i], sensor_center[i], sensor_right[i], friction_coefficient[i]])
    return






	


def main():
    
    GPIO.setwarnings(False)
    #GPIO.cleanup()
    f_object, cycles, cycles_time, filedir = inputParameters(weight= WEIGHT, velocity= VELOCITY)
    
    res = contactGpioSettingsV2()
    if res != None: hx_left, hx_center, hx_right = res
    else: return None
    del res
    
    frictionProgramV2(WEIGHT, VELOCITY, f_object, cycles, cycles_time, filedir, hx_left, hx_center, hx_right)
    #frictionProgramV3(hx_left, hx_center, hx_right)
    #while(1): frictionProgramV4()
    #GPIO.setwarnings(False)
    #while(1): frictionProgramV4()
    
    
    
    
   
   
   

def simple_test():
    import RPi.GPIO as GPIO  # import GPIO
    from hx711_v2 import HX711  # import the class HX711
    GPIO.setmode(GPIO.BCM)  # set GPIO pin mode to BCM numbering
    hx = HX711(dout_pin=21, pd_sck_pin=20)  # create an object
    while(1): print(hx.get_raw_data_mean(readings=1))  # get raw data reading from hx711
    GPIO.cleanup()
    return


    

if __name__ == "__main__":
    #main()
    simple_test() 
    #frictionProgramV4()  
    
