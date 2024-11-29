import sys
import random
import csv
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QCheckBox,
    QGroupBox, QWidget, QGridLayout, QLineEdit, QFormLayout
)
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5.QtCore import QTimer

class RetroApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Sensor Control Panel")
        self.setGeometry(100, 100, 1200, 800)
        self.setStyleSheet("background-color: #001800; color: #00FF00;")

        # Основной виджет и макет
        central_widget = QWidget()
        central_widget.setStyleSheet("background-color: #001800;")
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)

        # Верхняя панель с кнопками и полем для ввода
        self.control_layout = self.create_controls()
        main_layout.addLayout(self.control_layout)

        # Панель отображения данных
        self.data_display = self.create_data_display()
        main_layout.addLayout(self.data_display)

        # Панель графиков
        self.graphs_groupbox = self.create_graph()
        main_layout.addWidget(self.graphs_groupbox)

        # Инициализация данных
        self.data = {
            "time": [],
            "normal_force": [],
            "tangential_force1": [],
            "tangential_force2": [],
            "friction_coefficient": [],
            "penetration_depth": [],
        }

        # Таймер для обновления данных
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_data)

    def create_controls(self):
        """Создает панель управления с кнопками START, STOP, чекбоксами и текстовым полем."""
        control_layout = QHBoxLayout()

        # Кнопка START
        self.start_button = QPushButton("START")
        self.start_button.setStyleSheet("background-color: #006000; color: #00FF00;")
        self.start_button.clicked.connect(self.start_reading)
        control_layout.addWidget(self.start_button)

        # Кнопка STOP
        self.stop_button = QPushButton("STOP")
        self.stop_button.setStyleSheet("background-color: #006000; color: #00FF00;")
        self.stop_button.clicked.connect(self.stop_reading)
        control_layout.addWidget(self.stop_button)

        # Поле для ввода имени файла
        self.filename_input = QLineEdit()
        self.filename_input.setStyleSheet("background-color: #001800; color: #00FF00;")
        self.filename_input.setPlaceholderText("Enter CSV filename")
        control_layout.addWidget(self.filename_input)

        # Чекбоксы для выбора графиков
        self.graph_checkboxes = {
            "normal_force": QCheckBox("Normal Force"),
            "tangential_forces": QCheckBox("Tangential Forces"),
            "friction_coefficient": QCheckBox("Friction Coefficient"),
            "penetration_depth": QCheckBox("Penetration Depth"),
        }

        for checkbox in self.graph_checkboxes.values():
            checkbox.setStyleSheet("color: #00FF00;")
            control_layout.addWidget(checkbox)

        return control_layout

    def create_data_display(self):
        """Создает панель отображения данных."""
        grid_layout = QGridLayout()

        # Метки и значения данных
        self.data_labels = {
            "normal_force": QLabel("Normal Force: 0.00"),
            "tangential_force1": QLabel("Tangential Force 1: 0.00"),
            "tangential_force2": QLabel("Tangential Force 2: 0.00"),
            "angular_velocity": QLabel("Angular Velocity: 0.00"),
            "test_time": QLabel("Test Time (Current/Remaining): 0.00 / 0.00"),
            "friction_path": QLabel("Friction Path (Current/Remaining): 0.00 / 0.00"),
            "z_coordinate": QLabel("Z Coordinate: 0.00"),
        }

        row = 0
        for label_name, label_widget in self.data_labels.items():
            label_widget.setStyleSheet("color: #00FF00;")
            grid_layout.addWidget(label_widget, row, 0)
            row += 1

        return grid_layout

    def create_graph(self):
        """Создает полотно и оси графиков."""
        graph_groupbox = QGroupBox("Graphs")
        graph_groupbox.setStyleSheet("color: #00FF00; background-color: #001800;")
        layout = QVBoxLayout()

        self.canvas = FigureCanvas(Figure(facecolor="#001800"))  # Цвет фона графика
        layout.addWidget(self.canvas)

        # Создаем ось и сохраняем ее как self.ax
        self.ax = self.canvas.figure.add_subplot(111)
        self.ax.set_facecolor("#001800")
        self.ax.tick_params(colors="#00FF00")
        self.ax.set_title("Sensor Data", color="#00FF00", fontsize=12)
        self.ax.set_xlabel("Time (s)", color="#00FF00")
        self.ax.set_ylabel("Value", color="#00FF00")

        graph_groupbox.setLayout(layout)
        return graph_groupbox

    def start_reading(self):
        """Начинает чтение данных и обновление графиков."""
        self.clear_graphs()
        self.timer.start(100)  # Обновление каждые 100 мс

    def stop_reading(self):
        """Останавливает чтение данных и сохраняет их в CSV."""
        self.timer.stop()
        filename = self.filename_input.text().strip()
        if filename:
            self.save_data_to_csv(filename)

    def save_data_to_csv(self, filename):
        """Сохраняет данные в CSV файл."""
        if not filename.endswith(".csv"):
            filename += ".csv"
        with open(filename, mode="w", newline="") as file:
            writer = csv.writer(file)
            writer.writerow(["Time", "Normal Force", "Tangential Force 1", "Tangential Force 2", "Friction Coefficient", "Penetration Depth"])
            for i in range(len(self.data["time"])):
                writer.writerow([
                    self.data["time"][i],
                    self.data["normal_force"][i],
                    self.data["tangential_force1"][i],
                    self.data["tangential_force2"][i],
                    self.data["friction_coefficient"][i],
                    self.data["penetration_depth"][i],
                ])

    def clear_graphs(self):
        """Очищает данные и графики."""
        self.data = {key: [] for key in self.data.keys()}
        if hasattr(self, "ax"):
            self.ax.clear()
            self.ax.set_facecolor("#001800")
            self.canvas.draw()

    def update_data(self):
        """Обновляет данные и графики."""
        # Добавляем случайные данные для теста
        time = len(self.data["time"]) * 0.1
        self.data["time"].append(time)
        self.data["normal_force"].append(random.uniform(0, 10))
        self.data["tangential_force1"].append(random.uniform(0, 5))
        self.data["tangential_force2"].append(random.uniform(0, 5))
        self.data["friction_coefficient"].append(random.uniform(0, 1))
        self.data["penetration_depth"].append(random.uniform(0, 1))

        # Обновление отображения данных
        self.data_labels["normal_force"].setText(f"Normal Force: {self.data['normal_force'][-1]:.2f}")
        self.data_labels["tangential_force1"].setText(f"Tangential Force 1: {self.data['tangential_force1'][-1]:.2f}")
        self.data_labels["tangential_force2"].setText(f"Tangential Force 2: {self.data['tangential_force2'][-1]:.2f}")

        # Обновление графиков
        self.update_graphs()

    def update_graphs(self):
        """Обновляет графики на основе данных."""
        self.ax.clear()
        self.ax.set_facecolor("#001800")
        self.ax.tick_params(colors="#00FF00")
        self.ax.set_title("Sensor Data", color="#00FF00", fontsize=12)
        self.ax.set_xlabel("Time (s)", color="#00FF00")
        self.ax.set_ylabel("Value", color="#00FF00")

        # Проверка активных графиков
        if self.graph_checkboxes["normal_force"].isChecked():
            self.ax.plot(self.data["time"], self.data["normal_force"], label="Normal Force", color="#00FF00")
        if self.graph_checkboxes["tangential_forces"].isChecked():
            self.ax.plot(self.data["time"], self.data["tangential_force1"], label="Tangential Force 1", color="#FFAA00")
            self.ax.plot(self.data["time"], self.data["tangential_force2"], label="Tangential Force 2", color="#AAFF00")
        if self.graph_checkboxes["friction_coefficient"].isChecked():
            self.ax.plot(self.data["time"], self.data["friction_coefficient"], label="Friction Coefficient", color="#00FFFF")
        if self.graph_checkboxes["penetration_depth"].isChecked():
            self.ax.plot(self.data["time"], self.data["penetration_depth"], label="Penetration Depth", color="#FF00FF")

        # Установка легенды
        self.ax.legend(loc="upper right", fontsize=10, facecolor="#001800", edgecolor="#00FF00", labelcolor="#00FF00")
        self.canvas.draw()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    retro_app = RetroApp()
    retro_app.show()
    sys.exit(app.exec_())
