function saveGeoJson() {
  let object = {
    type: 'FeatureCollection',
    features: [],
  };

  myMap.geoObjects.each((geoObject) => {
    object.features.push(_createGeoJsonFor(geoObject));
  });

  var blob = new Blob([JSON.stringify(convertGeojsonElement(object))], {
    type: 'application/json;charset=utf-8',
  });
  saveAs(blob, 'download.json');
}

function _createGeoJsonFor(geoObject) {
  return {
    type: 'Feature',
    geometry: {
      type: geoObject.geometry.getType(),
      coordinates: geoObject.geometry.getCoordinates(),
    }
  };
}
