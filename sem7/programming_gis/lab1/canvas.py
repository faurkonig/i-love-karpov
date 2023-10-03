from qgis.gui import *
from qgis.PyQt.QtWidgets import QAction, QMainWindow
from qgis.PyQt.QtCore import Qt

class MyWnd(QMainWindow):
    def __init__(self, layers):
        QMainWindow.__init__(self)

        self.canvas = QgsMapCanvas()
        self.canvas.setCanvasColor(Qt.white)

        self.canvas.setExtent(iface.activeLayer().extent())
        self.canvas.setLayers(layers)

        self.setCentralWidget(self.canvas)

        self.actionZoomIn = QAction("Zoom in", self)
        self.actionZoomOut = QAction("Zoom out", self)
        self.actionPan = QAction("Pan", self)

        self.actionZoomIn.setCheckable(True)
        self.actionZoomOut.setCheckable(True)
        self.actionPan.setCheckable(True)

        self.actionZoomIn.triggered.connect(self.zoomIn)
        self.actionZoomOut.triggered.connect(self.zoomOut)
        self.actionPan.triggered.connect(self.pan)

        self.toolbar = self.addToolBar("Canvas actions")
        self.toolbar.addAction(self.actionZoomIn)
        self.toolbar.addAction(self.actionZoomOut)
        self.toolbar.addAction(self.actionPan)

        # create the map tools
        self.toolPan = QgsMapToolPan(self.canvas)
        self.toolPan.setAction(self.actionPan)
        self.toolZoomIn = QgsMapToolZoom(self.canvas, False) # false = in
        self.toolZoomIn.setAction(self.actionZoomIn)
        self.toolZoomOut = QgsMapToolZoom(self.canvas, True) # true = out
        self.toolZoomOut.setAction(self.actionZoomOut)

        self.pan()

    def zoomIn(self):
        self.canvas.setMapTool(self.toolZoomIn)

    def zoomOut(self):
        self.canvas.setMapTool(self.toolZoomOut)

    def pan(self):
        self.canvas.setMapTool(self.toolPan)


layer = iface.activeLayer()

niceRend = QgsCategorizedSymbolRenderer()
niceRend.setClassAttribute('fid')

for feature in layer.getFeatures():
    delta = feature.id() / (layer.featureCount() - 1)
    color = QColor.fromRgb(int(255 * delta), int(255 * (1 - delta)), 0)
    symb = QgsMarkerSymbol.createSimple({'name': 'diamond', 'color': color, 'size': 6})
    
    layerFid = feature.attribute('fid')
    niceRend.addCategory(QgsRendererCategory(layerFid, symb, f'category {delta}'))

layer.setRenderer(niceRend)

layer.setSubsetString('"fclass"=\'bus_stop\'')
layer.setSubsetString('')

layer.triggerRepaint()
iface.layerTreeView().refreshLayerSymbology(layer.id())

allLayers = QgsProject.instance().mapLayers().values()
w = MyWnd(allLayers)
w.show()
