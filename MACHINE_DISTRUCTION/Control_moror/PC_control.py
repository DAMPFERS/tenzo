# Импортируем модуль sys для работы с системными функциями
import sys
import serial                                   # Импортируем модуль serial для работы с последовательными портами
import serial.tools.list_ports                  # Импортируем инструмент для перечисления доступных портов

# Импортируем необходимые классы из PyQt5 для создания графического интерфейса
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QDoubleSpinBox,
    QCheckBox, QComboBox, QPushButton, QFormLayout
)

# Импортируем модуль Qt из PyQt5 для работы с ядром Qt
from PyQt5.QtCore import Qt

# Определяем класс StyledApp, который наследуется от QWidget
class StyledApp(QWidget):
    def __init__(self):
        # Вызываем конструктор родительского класса
        super().__init__()
        # Инициализируем пользовательский интерфейс
        self.initUI()
        # Инициализируем переменную для хранения объекта serial
        self.serial_connection = None

    def initUI(self):
        # Создаем вертикальный макет для основного окна
        main_layout = QVBoxLayout()

        # Создаем метку для выбора устройства
        uart_label = QLabel('Устройство:')
        # Создаем выпадающий список для выбора UART
        self.uart_combo = QComboBox()
        # Добавляем элементы в выпадающий список
        self.uart_combo.addItems(self.USART_connection())
        # Создаем макет формы для параметров
        form_layout = QFormLayout()

        # Создаем параметры с помощью метода create_parameter
        self.angle_param = self.create_parameter("Градусы", "checkbox_1", 360.0 / 4096 * 114, 0, 360)
        self.rotate_param = self.create_parameter("Град/сек", "checkbox_2", 5.0, -95, 95)
        self.oscillation_param = self.create_parameter("±Угол", "checkbox_3", 360.0 / 4096 * 114, 0, 360)

        # Добавляем параметры в макет формы
        form_layout.addRow(QLabel("Angle:"), self.angle_param)
        form_layout.addRow(QLabel("Rotate:"), self.rotate_param)
        form_layout.addRow(QLabel("Oscill:"), self.oscillation_param)

        # Создаем горизонтальный макет для кнопок
        button_layout = QHBoxLayout()
        # Создаем кнопки "Start" и "Stop"
        self.start_button = QPushButton("Start")
        self.stop_button = QPushButton("Stop")
        self.start_button.setObjectName("start_button") 
        self.stop_button.setObjectName("stop_button") 

        # Подключаем обработчики событий для кнопок
        self.start_button.clicked.connect(self.start_action)
        self.stop_button.clicked.connect(self.stop_action)

        # Добавляем кнопки в макет
        button_layout.addWidget(self.start_button)
        button_layout.addWidget(self.stop_button)

        # Добавляем элементы в основной макет
        main_layout.addWidget(uart_label)
        main_layout.addWidget(self.uart_combo)
        main_layout.addLayout(form_layout)
        main_layout.addLayout(button_layout)

        # Устанавливаем основной макет для окна
        self.setLayout(main_layout)

        # Устанавливаем заголовок окна
        self.setWindowTitle('Control')
        # Устанавливаем фиксированный размер окна
        self.setFixedSize(400, 300)

        # Единый стиль для всех элементов
        self.setStyleSheet("""
            QWidget {
                background-color: #c6c6c6;
                font-size: 12pt;
            }
            QCheckBox::indicator {
                width: 24px;
                height: 24px;
                border: 2px solid #888;
                border-radius: 4px;
                spacing: 8px;
            }
            QCheckBox::indicator:unchecked {
                background: white;
            }
            QCheckBox::indicator:checked {
                background: #4CAF50;
                border-color: #388E3C;
            }
            QPushButton#start_button {
                background-color: #4CAF50;
                color: white;
                border: none;
                padding: 10px;
                border-radius: 4px;
            }
            QPushButton#start_button:hover {
                background-color: #45a049;
            }
            QPushButton#start_button:pressed {
                background-color: #388e3c;
            }
            QPushButton#stop_button {
                background-color: #f44336;
                color: white;
                border: none;
                padding: 10px;
                border-radius: 4px;
            }
            QPushButton#stop_button:hover {
                background-color: #e57373;
            }
            QPushButton#stop_button:pressed {
                background-color: #c62828;
            }
            QDoubleSpinBox {
                padding: 5px;
                min-width: 100px;
                background-color: white;
                color: black;
                border: 1px solid #888;
                border-radius: 3px;
            }
            QDoubleSpinBox:hover {
                background-color: #f5f5f5;
            }

            QDoubleSpinBox:focus {
                border: 2px solid #4CAF50;
            }   
        """)
        

        # Отображаем окно
        self.show()

    def create_parameter(self, unit, checkbox_name, step, range_min, range_max):
        # Создаем горизонтальный макет для параметра
        param_layout = QHBoxLayout()
        # Создаем виджет для ввода числовых значений
        value_spinbox = QDoubleSpinBox()
        # Устанавливаем количество десятичных знаков
        value_spinbox.setDecimals(3)
        # Устанавливаем диапазон допустимых значений
        value_spinbox.setRange(range_min, range_max)
        # Устанавливаем шаг изменения значения
        value_spinbox.setSingleStep(step)

        # Создаем метку для единиц измерения
        unit_label = QLabel(unit)
        # Создаем флажок
        checkbox = QCheckBox()
        # Устанавливаем имя объекта для флажка
        checkbox.setObjectName(checkbox_name)
        # Подключаем обработчик изменения состояния флажка
        checkbox.stateChanged.connect(self.checkbox_changed)

        # Добавляем элементы в макет параметра
        param_layout.addWidget(value_spinbox)
        param_layout.addWidget(unit_label)
        param_layout.addStretch()  # Прижимает чекбокс к правому краю
        param_layout.addWidget(checkbox)
        

        # Сохраняем ссылку на value_spinbox в атрибуте класса
        setattr(self, f'{checkbox_name}_spinbox', value_spinbox)

        # Возвращаем макет параметра
        return param_layout

    def checkbox_changed(self):
        # Получаем объект, вызвавший событие
        sender = self.sender()
        # Если флажок установлен
        if sender.isChecked():
            # Проходим по всем флажкам и снимаем выделение с остальных
            for i in range(1, 4):
                checkbox = self.findChild(QCheckBox, f'checkbox_{i}')
                if checkbox and checkbox != sender:
                    checkbox.setChecked(False)

    def USART_connection(self):
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

    def start_action(self):
        # Получаем выбранный порт
        selected_port = self.uart_combo.currentText()
        if not selected_port:
            return

        try:
            self.serial_connection = serial.Serial(selected_port, 9600, timeout=1)
        except:
            pass

        # Проверяем, какой флажок установлен
        for i in range(1, 4):
            checkbox = self.findChild(QCheckBox, f'checkbox_{i}')
            if checkbox and checkbox.isChecked():
                # Получаем значение параметра
                value_spinbox = getattr(self, f'checkbox_{i}_spinbox')
                value = value_spinbox.value()
                # print(value)
                # Отправляем значение в порт
                # print(value)
                command = 1 << 7 # start bit
                if i == 1:
                    command |= int(value / (360.0 / 4096 * 114))
                    # print (bin(command))
                    
                elif i == 2:
                    if value > 0: command |= 1 << 5
                    else: command |= 1 << 6
                    command |=  abs(int(value)) // 5
                    # print (bin(command))
                    
                elif i == 3: 
                    command |= (1 << 6) | (1 << 5)
                    command |= int(value / (360.0 / 4096 * 114))
                    # print (bin(command))
                
                # Отправляем команду "start"
                self.serial_connection.write(command.to_bytes(1, byteorder='big'))
                # print(command.to_bytes(1, byteorder='big'))
                break

        # Закрываем соединение
        # self.serial_connection.close()

    def stop_action(self):
        # Получаем выбранный порт
        selected_port = self.uart_combo.currentText()
        if not selected_port:
            return

        # Устанавливаем соединение с выбранным портом
        try:
            self.serial_connection = serial.Serial(selected_port, 9600, timeout=1)
        except:
            pass

        # Отправляем команду "stop"
        self.serial_connection.write((0x7f).to_bytes(1, byteorder='big'))

        # Закрываем соединение
        # self.serial_connection.close()

# Основная часть программы
if __name__ == "__main__":
    # Создаем экземпляр приложения
    app = QApplication(sys.argv)
    # Создаем экземпляр нашего виджета
    ex = StyledApp()
    # Запускаем основной цикл приложения
    sys.exit(app.exec_())
