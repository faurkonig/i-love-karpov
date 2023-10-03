function saveGeoJson() {
  let object = {
    type: 'FeatureCollection',
    features: [],
  };

  // TODO: Add geoobject creation

  var blob = new Blob([JSON.stringify(object)], {
    type: 'application/json;charset=utf-8',
  });
  saveAs(blob, 'download.json');
}

function _createLineGeoJson() {
  let originalCoordinates = myPolyline.geometry.getCoordinates();
  let swappedCoordinates = [];

  originalCoordinates.forEach((coord) => {
    swappedCoordinates.push([coord[1], coord[0]]);
  });

  return {
    type: 'Feature',
    geometry: {
      coordinates: swappedCoordinates,
      type: 'LineString',
    },
  };
}

function _createPointGeoJson() {
  let originalCoordinates = myPoint.geometry.getCoordinates();
  let swappedCoordinates = [originalCoordinates[1], originalCoordinates[0]];

  return {
    type: 'Feature',
    geometry: {
      coordinates: swappedCoordinates,
      type: 'Point',
    },
  };
}
