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
  const geoObject = new ymaps.GeoObject(
    {
      geometry: object.geometry,
      properties: {
        hintContent: 'Click to toggle editing',
      },
    },
    _createOptionsForObject(object, false),
  );

  let isEditing = false;
  myMap.geoObjects.add(geoObject);
  geoObject.events.add('click', (_) => {
    if (isEditing) {
      console.log('Stop editing');
      isEditing = false;
      geoObject.editor.stopEditing();
    } else {
      console.log('Start editing');
      isEditing = true;
      geoObject.editor.startEditing();
    }

    const newOptions = _createOptionsForObject(object, isEditing);
    console.log('Setting new options for object', newOptions);
    for (const [key, value] of Object.entries(newOptions)) {
      geoObject.options.set(key, value);
      // console.log(`${key}: ${value}`);
    }
  });
}

function _createOptionsForObject(object, isEditing) {
  const foregroundColor = isEditing ? '#E32636' : '#7FFFD4';
  const backgroundColor = isEditing ? '#E32636AA' : '#7FFFD4AA';
  switch (object.geometry.type) {
    case 'Point':
      return {
        iconColor: foregroundColor,
      };
    case 'LineString':
      return {
        strokeColor: foregroundColor,
        strokeWidth: 6,
      };
    case 'Polygon':
      return {
        fillColor: backgroundColor,
        strokeColor: foregroundColor,
        strokeWidth: 3,
      };
  }
}
