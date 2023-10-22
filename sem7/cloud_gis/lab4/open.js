function openGeoJson() {
  const file = document.querySelector('input[type=file]').files[0];
  if (!file) {
    return;
  }

  const reader = new FileReader();
  reader.onload = function (e) {
    const contents = e.target.result;
    const geoJson = convertGeojsonElement(JSON.parse(contents));
    _addObjectToMap(geoJson);
    console.log(myMap.geoObjects.each((e) => console.log('Object', e)));
  };
  reader.readAsText(file);
}

function _addObjectToMap(object) {
  switch (object.type) {
    case 'FeatureCollection':
      console.log('Add feature collection', object);
      object.features.forEach(_addObjectToMap);
      break;
    case 'Feature':
      _addFeatureToMap(object);
      break;
  }
}

var objectIndex = 1;

function _addFeatureToMap(object) {
  console.log('Add feature', object);
  const type = object.geometry.type;
  const geoObject = new ymaps.GeoObject(
    {
      geometry: object.geometry,
      properties: {
        hintContent: `(#${objectIndex}) Click to start editing`,
      },
    },
    {
      id: objectIndex++,
      ..._createOptionsForObject(type, false),
    },
  );
  _customizeEditorMenu(geoObject, type);

  let isEditing = false;
  myMap.geoObjects.add(geoObject);
  geoObject.events.add('click', (e) => {
    if (isEditing) {
      _stopEditing(geoObject, type);
      isEditing = false;
    } else {
      _startEditing(geoObject, type);
      isEditing = true;
    }

    const newOptions = _createOptionsForObject(type, isEditing);
    for (const [key, value] of Object.entries(newOptions)) {
      geoObject.options.set(key, value);
    }
  });
  geoObject.events.add('geometrychange', geoObjectGeometryChangedCallback);

  geometryRegistry.set(geoObject, geoObject.geometry.getCoordinates());
}

function _createOptionsForObject(type, isEditing) {
  const foregroundColor = isEditing ? '#FF4040' : '#6495ED';
  const backgroundColor = isEditing ? '#FF4040AA' : '#6495EDAA';
  switch (type) {
    case 'Point':
      return {
        iconColor: foregroundColor,
      };
    case 'LineString':
      return {
        strokeColor: foregroundColor,
        strokeWidth: 6,
        draggable: isEditing,
      };
    case 'Polygon':
      return {
        fillColor: backgroundColor,
        strokeColor: foregroundColor,
        strokeWidth: 3,
        draggable: isEditing,
      };
  }
}

function _customizeEditorMenu(geoObject, type) {
  let buttonText;
  switch (type) {
    case 'Point':
      buttonText = 'Delete point';
      break;
    case 'LineString':
      buttonText = 'Delete line';
      break;
    case 'Polygon':
      buttonText = 'Delete polygon';
      break;
  }

  geoObject.options.set('editorMenuManager', function (items) {
    items.push({
      title: buttonText,
      onClick: function () {
        myMap.geoObjects.remove(geoObject);
      },
    });
    return items;
  });
}

function _startEditing(geoObject, type) {
  console.log('Start editing');
  geoObject.editor.startEditing();

  const objectIndex = geoObject.options.get('id');

  if (type === 'Point') {
    geoObject.properties.set(
      'hintContent',
      `(#${objectIndex}) Click to stop editing. Right-click to delete`,
    );
    geoObject.events.add('contextmenu', _pointContextMenuCallback);
  } else {
    geoObject.properties.set(
      'hintContent',
      `(#${objectIndex}) Click to stop editing`,
    );
  }
}

function _stopEditing(geoObject, type) {
  console.log('Stop editing');
  geoObject.editor.stopEditing();
  const objectIndex = geoObject.options.get('id');
  geoObject.properties.set(
    'hintContent',
    `(#${objectIndex}) Click to start editing`,
  );

  if (type === 'Point') {
    geoObject.events.remove('contextmenu', _pointContextMenuCallback);
  }
}

function _pointContextMenuCallback(event) {
  myMap.geoObjects.remove(event.get('target'));
}
