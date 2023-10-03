layer = iface.activeLayer()
#layer.renderer().setSymbol(symb)
#layer.renderer().symbol().setColor(QColor.fromRgb(255,0,0))

niceRend = QgsCategorizedSymbolRenderer()
niceRend.setClassAttribute('fid')


for feature in layer.getFeatures():
    delta = feature.id() / (layer.featureCount() - 1)
    color = QColor.fromRgb(int(255 * delta), int(255 * (1 - delta)), 0)
    symb = QgsMarkerSymbol.createSimple({'name': 'diamond', 'color': color, 'size': 6})
    
    layerFid = feature.attribute('fid')
    niceRend.addCategory(QgsRendererCategory(layerFid, symb, 'category 1'))
    layer.setRenderer(niceRend)

#layer.setSubsetString('"fclass"=\'bus_stop\'')
layer.setSubsetString('')

layer.triggerRepaint()
iface.layerTreeView().refreshLayerSymbology(layer.id())
