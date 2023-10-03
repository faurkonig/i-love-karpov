ymaps.ready(init);

var myMap;
var myObjectManager;

function init() {
  myMap = new ymaps.Map('map', {
    center: [55.74556122347454, 37.61131066761479],
    zoom: 8,
  });
  myObjectManager = new ymaps.ObjectManager({});
  myMap.geoObjects.add(myObjectManager);
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
