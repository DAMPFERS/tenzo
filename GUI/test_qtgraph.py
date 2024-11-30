import sys
import random
import csv
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QCheckBox,
    QGroupBox, QWidget, QGridLayout, QLineEdit
)
from PyQt5.QtCore import QTimer
import pyqtgraph as pg

class RetroApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Sensor Control Panel")
        self.setGeometry(100, 100, 1200, 800)
        self.setStyleSheet("background-color: #001800; color: #00FF00;")

        # Основной виджет и макет
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)

        # Верхняя панель с кнопками и полем для ввода
        self.control_layout = self.create_controls()
        main_layout.addLayout(self.control_layout)

        # Панель отображения данных
        self.data_display = self.create_data_display()
        main_layout.addLayout(self.data_display)

        # Панель графиков
        self.graphs_groupbox = QGroupBox("Graphs")
        self.graphs_groupbox.setStyleSheet("color: #00FF00;")
        self.graphs_layout = QGridLayout()
        self.graphs_groupbox.setLayout(self.graphs_layout)
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

        # Инициализация графиков
        self.create_graphs()

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
            checkbox.stateChanged.connect(self.update_graphs_visibility)
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

    def create_graphs(self):
        """Создает панель для графиков с использованием PyQtGraph."""
        self.graphs = {
            "normal_force": pg.PlotWidget(title="Normal Force"),
            "tangential_forces": pg.PlotWidget(title="Tangential Forces"),
            "friction_coefficient": pg.PlotWidget(title="Friction Coefficient"),
            "penetration_depth": pg.PlotWidget(title="Penetration Depth"),
        }

        for name, graph in self.graphs.items():
            graph.setBackground("#001800")  # Устанавливаем фон графика
            graph.getAxis("left").setPen("#00FF00")
            graph.getAxis("bottom").setPen("#00FF00")
            graph.getAxis("left").setTextPen("#00FF00")
            graph.getAxis("bottom").setTextPen("#00FF00")
            graph.setTitle(name.replace("_", " ").title(), color="#00FF00")  # Устанавливаем заголовок
            graph.setLabel('left', 'Value')
            graph.setLabel('bottom', 'Time')
            graph.addLegend(offset=(-10, 10))  # Перемещаем легенду в правый верхний угол

    def start_reading(self):
        """Начинает чтение данных и обновление графиков."""
        self.clear_graphs()
        self.timer.start(100)  # Обновление каждые 100 мс
        self.update_graphs_visibility()  # Обновляем видимость графиков при старте

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
        for graph in self.graphs.values():
            graph.clear()

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
        if self.graph_checkboxes["normal_force"].isChecked():
            self.graphs["normal_force"].plot(self.data["time"], self.data["normal_force"], pen=pg.mkPen(color="#00FF00", width=2), clear=True, name="Normal Force")
        if self.graph_checkboxes["tangential_forces"].isChecked():
            self.graphs["tangential_forces"].plot(self.data["time"], self.data["tangential_force1"], pen=pg.mkPen(color="#FFAA00", width=2), clear=True, name="Tangential Force 1")
            self.graphs["tangential_forces"].plot(self.data["time"], self.data["tangential_force2"], pen=pg.mkPen(color="#AAFF00", width=2), name="Tangential Force 2")
        if self.graph_checkboxes["friction_coefficient"].isChecked():
            self.graphs["friction_coefficient"].plot(self.data["time"], self.data["friction_coefficient"], pen=pg.mkPen(color="#FF00FF", width=2), clear=True, name="Friction Coefficient")
        if self.graph_checkboxes["penetration_depth"].isChecked():
            self.graphs["penetration_depth"].plot(self.data["time"], self.data["penetration_depth"], pen=pg.mkPen(color="#00FFFF", width=2), clear=True, name="Penetration Depth")

    def update_graphs_visibility(self):
        """Обновляет видимость графиков в зависимости от состояния чекбоксов."""
        # Удаляем все виджеты из текущего макета
        while self.graphs_layout.count():
            item = self.graphs_layout.takeAt(0)
            widget = item.widget()
            if widget:
                widget.setParent(None)

        # Сброс строк и колонок в макете
        for row in range(self.graphs_layout.rowCount()):
            self.graphs_layout.setRowStretch(row, 0)  # Сбрасываем растяжение строк
        for col in range(self.graphs_layout.columnCount()):
            self.graphs_layout.setColumnStretch(col, 0)  # Сбрасываем растяжение колонок

        # Список активных графиков
        active_graphs = [name for name, checkbox in self.graph_checkboxes.items() if checkbox.isChecked()]
        num_active_graphs = len(active_graphs)

        if num_active_graphs > 0:
            # Рассчитываем количество строк и столбцов
            cols = 2 if num_active_graphs > 2 else 1
            rows = (num_active_graphs + cols - 1) // cols

            for i, name in enumerate(active_graphs):
                row = i // cols
                col = i % cols
                self.graphs_layout.addWidget(self.graphs[name], row, col)

            # Устанавливаем равномерное распределение пространства между графиками
            for row in range(rows):
                self.graphs_layout.setRowStretch(row, 1)
            for col in range(cols):
                self.graphs_layout.setColumnStretch(col, 1)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    retro_app = RetroApp()
    retro_app.show()
    sys.exit(app.exec_())
