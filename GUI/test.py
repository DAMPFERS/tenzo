import sys
import random
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QPushButton, QVBoxLayout, QHBoxLayout,
    QLabel, QCheckBox, QWidget
)
from PyQt5.QtGui import QPalette, QColor, QFont
from PyQt5.QtCore import QTimer
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


class RetroApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Retro Sensor Control")
        self.setGeometry(100, 100, 800, 600)

        # --- Основной стиль ---
        self.setStyleSheet("background-color: #001800; color: #00FF00;")
        font = QFont("Courier", 12, QFont.Bold)
        self.setFont(font)

        # --- Виджеты ---
        self.sensor_value_label = QLabel("Sensor Value: 0.00")
        self.sensor_value_label.setStyleSheet("color: #00FF00;")

        self.start_button = QPushButton("START")
        self.start_button.setStyleSheet(self._button_style())
        self.start_button.clicked.connect(self.start_reading)

        self.stop_button = QPushButton("STOP")
        self.stop_button.setStyleSheet(self._button_style())
        self.stop_button.clicked.connect(self.stop_reading)

        self.graph_checkbox = QCheckBox("Show Graph")
        self.graph_checkbox.setStyleSheet("color: #00FF00;")

        # --- График ---
        self.figure = Figure(facecolor="#001800")
        self.canvas = FigureCanvas(self.figure)
        self.ax = self.figure.add_subplot(111)
        self.ax.set_facecolor("#001800")
        self.ax.tick_params(colors="#00FF00")
        self.ax.spines["top"].set_color("#00FF00")
        self.ax.spines["bottom"].set_color("#00FF00")
        self.ax.spines["left"].set_color("#00FF00")
        self.ax.spines["right"].set_color("#00FF00")
        self.ax.set_title("Sensor Data", color="#00FF00", fontsize=12)
        self.ax.set_xlabel("Time (s)", color="#00FF00")
        self.ax.set_ylabel("Value", color="#00FF00")
        self.line, = self.ax.plot([], [], color="#00FF00")

        # --- Компоновка ---
        layout = QVBoxLayout()
        top_layout = QHBoxLayout()

        top_layout.addWidget(self.sensor_value_label)
        top_layout.addWidget(self.start_button)
        top_layout.addWidget(self.stop_button)
        top_layout.addWidget(self.graph_checkbox)

        layout.addLayout(top_layout)
        layout.addWidget(self.canvas)

        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

        # --- Таймер и данные ---
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_data)
        self.data = []
        self.time_data = []
        self.start_time = None

    def _button_style(self):
        return """
        QPushButton {
            background-color: #001800;
            color: #00FF00;
            border: 2px solid #00FF00;
            padding: 5px;
        }
        QPushButton:pressed {
            background-color: #004400;
        }
        """

    def start_reading(self):
        self.start_time = 0
        self.data = []
        self.time_data = []
        self.timer.start(100)

    def stop_reading(self):
        self.timer.stop()

    def update_data(self):
        value = random.uniform(0, 100)
        self.data.append(value)
        if len(self.time_data) == 0:
            self.time_data.append(0)
        else:
            self.time_data.append(self.time_data[-1] + 0.1)

        self.sensor_value_label.setText(f"Sensor Value: {value:.2f}")

        if self.graph_checkbox.isChecked():
            self.line.set_xdata(self.time_data)
            self.line.set_ydata(self.data)
            self.ax.relim()
            self.ax.autoscale_view()
            self.canvas.draw()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RetroApp()
    window.show()
    sys.exit(app.exec_())
