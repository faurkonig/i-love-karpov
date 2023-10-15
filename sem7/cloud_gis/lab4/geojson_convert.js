function convertGeojsonElement(element) {
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
  array.forEach((element) => result.push(convertGeojsonElement(element)));
  return result;
}

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
