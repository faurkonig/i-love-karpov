ymaps.ready(init);

var myMap;
var myPolyline;
var myPoint;
var myPolygon;

function init() {
  myMap = new ymaps.Map("map", {
    center: [
      55.74556122347454,
      37.61131066761479,
    ],
    zoom: 8
  });

  myPolyline = new ymaps.Polyline(
    [
      [
        55.74556122347454,
        37.61131066761479,
      ],
      [
        55.84556122347454,
        37.71131066761479,
      ],
    ],
    {},
    {
      strokeWidth: 6,
      strokeColor: '#FF00FF',
    }
  );

  myPoint = new ymaps.Placemark(
    [
      55.74556122347454,
      37.61131066761479,
    ],
    {},
    {
      draggable: true
    }
  );

  myMap.events.add('click', function (e) {
    let coords = e.get('coords');
    console.log('Clicked in', coords[0], coords[1]);

    myPolyline.geometry.insert(myPolyline.geometry.getLength(), coords)

    console.log(myPolyline.geometry)

  });

  myMap.geoObjects.add(myPolyline);
  myMap.geoObjects.add(myPoint);
  myPolyline.editor.startEditing();
}

function saveGeoJson() {
  let object = {
    'type': 'FeatureCollection',
    'features': [
      _createLineGeoJson(),
      _createPointGeoJson(),
    ]
  }

  var blob = new Blob([JSON.stringify(object)], {
    type: "application/json;charset=utf-8",
  });
  saveAs(blob, 'download.json');
}

function _createLineGeoJson() {
  let originalCoordinates = myPolyline.geometry.getCoordinates();
  let swappedCoordinates = []

  originalCoordinates.forEach((coord) => {
    swappedCoordinates.push([coord[1], coord[0]])
  })

  return {
    'type': 'Feature',
    'geometry': {
      'coordinates': swappedCoordinates,
      'type': 'LineString',
    },
  }
}

function _createPointGeoJson() {
  let originalCoordinates = myPoint.geometry.getCoordinates();
  let swappedCoordinates = [originalCoordinates[1], originalCoordinates[0]];

  return {
    'type': 'Feature',
    'geometry': {
      coordinates: swappedCoordinates,
      type: 'Point',
    }
  }
}

/*
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "coordinates": [
          [
            25.801110652861382,
            -0.8717269248489004
          ],
          [
            26.03692400311843,
            13.84875491400571
          ],
          [
            41.23267529307978,
            6.777811204550687
          ]
        ],
        "type": "LineString"
      }
    }
  ]
}
*/