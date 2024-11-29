import sys
from PyQt5.QtWidgets import QApplication
import pyqtgraph as pg


app = QApplication(sys.argv)


plt = pg.plot(title = "Legend Example")
plt.addLegend()

c1 = plt.plot([1,2,3,4], pen = 'r', name = "Red Plot")
c2 = plt.plot([2,1,4,3], pen = 'g', fillLevel = 0, fillBrush = (255,255,255,30), name = "Green Plot")

c3 = plt.addLine(y=4, pen = 'y')

status = app.exec_()
sys.exit(status)