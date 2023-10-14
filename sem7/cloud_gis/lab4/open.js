function openGeoJson() {
  const file = document.querySelector('input[type=file]').files[0];
  if (!file) {
    return;
  }

  const reader = new FileReader();
  reader.onload = function (e) {
    const contents = e.target.result;
    const geoJson = _convertElement(JSON.parse(contents));
    _addObjectToMap(geoJson);
    console.log(myMap.geoObjects.each((e) => console.log('Object', e)));
  };
  reader.readAsText(file);
}

function _convertElement(element) {
  if (element instanceof Array) {
    return _convertArray(element);
  }

  if (typeof element === 'object') {
    return _convertObject(element);
  }

  return element;
}

function _convertArray(array) {
  if (
    array.length === 2 &&
    typeof array[0] === 'number' &&
    typeof array[1] === 'number'
  ) {
    return [array[1], array[0]];
  }

  const result = [];
  array.forEach((element) => result.push(_convertElement(element)));
  return result;
}

var id = 0;

function _convertObject(object) {
  if (object.type === 'FeatureCollection') {
    return {
      ...object,
      features: _convertArray(object.features),
    };
  }

  if (object.type === 'Feature') {
    return {
      ...object,
      id: id++,
      geometry: _convertObject(object.geometry),
    };
  }

  if (object.type !== undefined) {
    return {
      ...object,
      coordinates: _convertArray(object.coordinates),
    };
  }

  return object;
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

function _addFeatureToMap(object) {
  console.log('Add feature', object);
  const type = object.geometry.type;
  const geoObject = new ymaps.GeoObject(
    {
      geometry: object.geometry,
      properties: {
        hintContent: 'Click to start editing',
      },
    },
    _createOptionsForObject(type, false),
  );
  _customizeEditorMenu(geoObject, type);

  let isEditing = false;
  myMap.geoObjects.add(geoObject);
  geoObject.events.add('click', (_) => {
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
      // console.log(`${key}: ${value}`);
    }
  });
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

  if (type === 'Point') {
    geoObject.properties.set(
      'hintContent',
      'Click to stop editing. Right-click to delete',
    );
    geoObject.events.add('contextmenu', _pointContextMenuCallback);
  } else {
    geoObject.properties.set('hintContent', 'Click to stop editing');
  }
}

function _stopEditing(geoObject, type) {
  console.log('Stop editing');
  geoObject.editor.stopEditing();
  geoObject.properties.set('hintContent', 'Click to start editing');

  if (type === 'Point') {
    geoObject.events.remove('contextmenu', _pointContextMenuCallback);
  }
}

function _pointContextMenuCallback(event) {
  myMap.geoObjects.remove(event.get('target'));
}
