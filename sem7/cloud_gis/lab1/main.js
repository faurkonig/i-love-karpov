ymaps.ready(init);

function init() {
  var myMap = new ymaps.Map("map", {
    center: [
      55.74556122347454,
      37.61131066761479,
    ],
    zoom: 8
  });

  // Creating a polyline using the GeoObject class.
  var myGeoObject = new ymaps.GeoObject({
    geometry: {
      type: "LineString",
      coordinates: [
        [
          55.67854426873242,
          37.418593769996846,
        ],
        [
          55.68927470785792,
          37.825440553856566,
        ],
        [
          55.886590098261905,
          37.46855741012067,
        ],
        [
          55.57914806671255,
          37.59465612090776,
        ],
        [
          55.86857225853572,
          37.749305483193325,
        ],
        [
          55.67988573463742,
          37.41740415951867,
        ]
      ]
    },
    properties: {
      balloonContent: "МКАД - ад"
    }
  }, {
    strokeColor: "#FF0000",
    strokeWidth: 5
  });

  const myPoint = new ymaps.GeoObject({
    geometry: {
      type: "Point",
      coordinates: [
        55.74556122347454,
        37.61131066761479,
      ]
    },
    properties: {
      balloonContent: 'Классная точка',
    }
  }, {
    preset: 'islands#blackStretchyIcon',
    draggable: true
  })

  myMap.geoObjects.add(myGeoObject).add(myPoint);
}
