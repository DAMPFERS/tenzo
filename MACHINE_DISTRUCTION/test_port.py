import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import csv
from colorama import init, Fore, Back, Style

def get_available_serial_ports():
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

# Функция обновления графика
def update_plot(frame):
    

    line = ser.readline().decode('utf-8').strip()
    try:
        value = int(line)
        data.append(value)  # Добавляем новое значение
        line_plot.set_ydata(data)  # Обновляем данные графика

        # Автоматическое масштабирование и обновление меток оси Y
        ax.relim()               # Перерасчет лимитов данных
        ax.autoscale_view()      # Применение автоматического масштабирования
        ax.yaxis.get_major_locator().set_params(integer=True)  # Настройка делений оси Y

        # Запись данных в CSV-файл
        with open(csv_file, mode='a', newline='') as file:
            writer = csv.writer(file, delimiter = ';')
            writer.writerow([value, (value - 8388607)/8388607 * 4.38 / 128* 1000])
            # writer.writerow([value])

    except ValueError:
        print(f"Data conversion error: {line}" + Back.YELLOW)
    return line_plot,

def main():
    init()
    
    global ser
    global data
    global line_plot
    global ax
    global csv_file
    # Настройки USB-порта
    print(Back.GREEN + "START!\n" , end='')
    print(Back.BLACK)
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
        writer.writerow(["Value", "Voltage"])  # Запись заголовка
        #writer.writerow(["Value"])  

    # Настройка графика
    fig, ax = plt.subplots()
    line_plot, = ax.plot(data)

    # Параметры отображения графика
    ax.set_xlabel("Samples")  # Подпись оси X
    ax.set_ylabel("Data Value")  # Подпись оси Y
    ax.set_title("Real-time USB Data Plot")  # Название графика
    ax.grid(True)  # Включение сетки

    # Запуск анимации с отключением blit для полной перерисовки
    ani = animation.FuncAnimation(fig, update_plot, interval=1, blit=False)

    plt.show()

    # Закрытие порта при завершении
    ser.close()
    return True

if __name__ == "__main__":
    while(1):
        if main() == None: continue
        print( Back.GREEN + "End iteratin\n", end='')
        print(Back.CYAN)
        if input("Do it again? [Y, n]: ") != 'Y': break
