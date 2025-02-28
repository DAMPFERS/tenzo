import sys
from PyQt5.QtWidgets import QApplication
import pyqtgraph as pg
import numpy as np

app = QApplication(sys.argv)

x = np.random.normal(size=1000)
y = np.random.normal(size=1000)

pg.plot(x, y, pen = None, symbol = 'o')
status = app.exec_()
sys.exit(status)