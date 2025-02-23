import os                                       # Импортируем модуль os для взаимодействия с операционной системой
import serial                                   # Импортируем модуль serial для работы с последовательными портами
import serial.tools.list_ports                  # Импортируем инструмент для перечисления доступных портов
import matplotlib.pyplot as plt                 # Импортируем модуль matplotlib.pyplot для создания графиков
import matplotlib.animation as animation        # Импортируем модуль matplotlib.animation для анимации графиков
from collections import deque                   # Импортируем deque из модуля collections для эффективного управления очередями
import csv                                      # Импортируем модуль csv для работы с CSV-файлами
from colorama import init, Fore, Back, Style    # Импортируем модуль colorama для цветного вывода в консоль


# Функция инициализация портов для подключения
def get_available_serial_ports()->list:
    # Получаем список всех доступных портов
    ports = serial.tools.list_ports.comports()
    available_ports = []

    # Проверяем каждый порт
    for port in ports:
        try:
            # Открываем порт
            ser = serial.Serial(port.device)
            # Если порт успешно открыт, добавляем его имя в список
            ser.close()
            available_ports.append(port.device)
        except (OSError, serial.SerialException):
            # Если не удалось открыть порт, продолжаем проверку
            pass

    # Возвращаем список доступных портов
    return available_ports


# Функция установки нулевого положения нагрузки
def setNull(ser)-> float:
    print("Set Null")
    line = ser.readline().decode('utf-8').strip()
    try:
        bf = [None] * 100
        for i in range(100):
            bf[i] = int(line)
        return sum(bf) / len(bf)
    except:
        return None



# Функция обновления графика
def update_plot(frame):
    line = ser.readline().decode('utf-8').strip()
    try:
        value = int(line) - int(null)
        value_Newton = value * res
        data.append(value_Newton)  # Добавляем новое значение
        line_plot.set_ydata(data)  # Обновляем данные графика

        # Автоматическое масштабирование и обновление меток оси Y
        ax.relim()               # Перерасчет лимитов данных
        ax.autoscale_view()      # Применение автоматического масштабирования
        ax.yaxis.get_major_locator().set_params(integer=True)  # Настройка делений оси Y

        # Запись данных в CSV-файл
        with open(csv_file, mode='a', newline='') as file:
            writer = csv.writer(file, delimiter = ';')
            writer.writerow([value, (value - 8388607)/8388607 * 4.38 / 128* 1000, value_Newton])
            # writer.writerow([value])

    except ValueError:
        print(f"Data conversion error: {line}" + Back.YELLOW)
    return line_plot,



def main():
    init()  # Инициализация colorama
    mas_k = (700 / 2353524.125, 4000 /2759070.524 , 8000 / 2645214.148, 40000 / 5374636)
    global ser  # Глобальная переменная для последовательного порта
    global data  # Глобальная переменная для данных
    global line_plot  # Глобальная переменная для линии графика
    global ax  # Глобальная переменная для осей графика
    global csv_file  # Глобальная переменная для имени CSV-файла
    global res  # Глобальная переменная для коэффициента преобразования
    global null  # Глобальная переменная для нулевого значения
    print(Back.GREEN + "START!\n" , end='')
    print(Back.BLACK)
    print("Sensors?")
    print("1. 100kG")
    print("2. 500kG")
    print("3. 1000kG")
    print("4. 4700kG")
    k = input('''Number: ''')
    if (k == '1') or (k == '2') or (k == '3') or (k == '4'):
        res = mas_k[int(k) - 1]
    else:
        print("Error_Sensor_Selection")
        return None

    os.system('cls')  # Очистка консоли
    csv_file = input("Name file save data: ") + ".csv"

    usb_port = 0
    available_ports = get_available_serial_ports()
    if len(available_ports) == 0:
        print(Back.RED + "ERROR: No ports available :(\n", end='')
        input("Do it again?")
        return None
    elif len(available_ports) > 1:
        print("Multiple ports available. Please select one:\n" + Back.BLUE, end='')
        for i, port in enumerate(available_ports):
            print(f"{i}: {port}\n" + Back.BLACK, end='')
        usb_port = available_ports[int(input("Enter the port number: "))]
    else:
        usb_port = available_ports[0]

    baud_rate = 9600
    ser = serial.Serial(usb_port, baud_rate)

    # Инициализация буфера данных и CSV-файла
    data = deque([0] * 100, maxlen=100)  # Хранит последние 100 значений

    # Открытие CSV-файла для записи данных
    with open(csv_file, mode='w', newline='') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(["Value", "Voltage", "Newton"])  # Запись заголовка
        #writer.writerow(["Value"])

    # Настройка графика
    fig, ax = plt.subplots()
    line_plot, = ax.plot(data)

    # Параметры отображения графика
    ax.set_xlabel("Samples")  # Подпись оси X
    ax.set_ylabel("Newton Value")  # Подпись оси Y
    ax.set_title("Real-time USB Data Plot")  # Название графика
    ax.grid(True)  # Включение сетки

    null = setNull(ser)
    print(null)
    if null == None:
        return None

    # Запуск анимации с отключением blit для полной перерисовки
    ani = animation.FuncAnimation(fig, update_plot, interval=1, blit=False)

    plt.show()

    # Закрытие порта при завершении
    ser.close()
    return True


if __name__ == "__main__":
    os.system('cls')  # Очистка консоли
    while(1):
        if main() == None: continue
        os.system('cls')
        print( Back.GREEN + "End iteratin\n", end='')
        print(Back.BLACK)
        if input("Do it again? [Y, n]: ") != 'Y':
            os.system('cls')
            break
