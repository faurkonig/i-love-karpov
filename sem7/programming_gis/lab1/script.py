def printLayerFields(layer: QgsVectorLayer):
    layer_name = layer.name()
    print(f'Поля слоя "{layer_name}":')
    for field in layer.getFeatures():
        print(field.name(), field.typeName())

def printNewFeatures(layer: QgsVectorLayer):
    layer_name = layer.name()
    print(f'Новые фичи слоя "{layer_name}":')
    for feature in layer.features():
        feature_class = feature.attribute('fclass')
        if feature_class.endsWith('generated'):
            print(feature.attribute('fid'), feature.geometry())

def printSeparator():
    print('\n------------------------\n')

poly_layer = QgsVectorLayer("layers/polygons_residential.shp", "polygons_residential", "ogr")

print('# Выводим все изначальные поля')
printLayerFields(poly_layer)
original_fields_count = poly_layer.fields().count()

printSeparator()

print('# Добавляем 2 новых поля')
poly_layer.dataProvider().addAttributes([
        QgsField('f1', QVariant.Int),
        QgsField('f2', QVariant.Double)
    ])
poly_layer.updateFields()
printLayerFields(poly_layer)

printSeparator()

print('# Удаляем все новые поля')
new_fields_count = poly_layer.fields().count()
layers_to_delete = [*range(original_fields_count, new_fields_count)]
poly_layer.dataProvider().deleteAttributes(layers_to_delete)
poly_layer.updateFields()
printLayerFields(poly_layer)

printSeparator()

point_layer = QgsVectorLayer("layers/points_stops.shp", "points_stops", "ogr")

print('# Добавляем новые объекты')
new_features = []
for i in range(3):
    new_feature = QgsFeature(point_layer.fields())
    new_feature.setAttribute('fid', random.randrange(0, 1000000))
    new_feature.setAttribute('osm_id', random.randrange(100000, 100000000))
    new_feature.setAttribute('code', 5621)
    new_feature.setAttribute('fclass', 'bus_stop_generated')
    new_feature.setAttribute('name', 'Новый крутой знак')
    base_x, base_y = (4113462, 7518833)
    new_position = QgsPointXY(
        base_x + random.randrange(100, 1000),
        base_y + random.randrange(100, 1000))
    new_feature.setGeometry(QgsGeometry.fromPointXY(new_position))
    point_layer.dataProvider().addFeatures([new_feature])
    new_features.append(new_feature)

new_features = random.randrange(0, len(new_features))
point_layer.dataProvider().removeFeatures([new_features])
