ymaps.ready(init);

const catImages = [
  'images/cat1.png',
  'images/cat2.png',
  'images/cat3.png',
]

var myMap;
var myTable;
var points = []

function init() {
  myMap = new ymaps.Map("map", {
    center: [
      55.74556122347454,
      37.61131066761479,
    ],
    zoom: 8
  });

  myTable = document.getElementById('points-table').getElementsByTagName('tbody')[0];

  myMap.events.add('click', function (e) {
    let coords = e.get('coords');
    console.log('Clicked in', coords[0], coords[1]);

    let number = points.length + 1;
    let imageFile = catImages[number % catImages.length];
    let newPoint = new ymaps.Placemark(coords, {
      balloonContent: `Метка #${number}`
    }, {
      iconLayout: 'default#image',
      iconImageHref: imageFile,
      // iconImageSize: [30, 42],
    });
    points.push({
      number: number,
      coords: coords,
      point: newPoint,
    });

    myMap.geoObjects.add(newPoint);

    let newRow = myTable.insertRow();
    let numberCell = newRow.insertCell()
    numberCell.textContent = number;
    numberCell.style = 'border: 1px solid black;';
    let xCell = newRow.insertCell()
    xCell.textContent = coords[0].toFixed(3);
    xCell.style = 'border: 1px solid black;';
    let yCell = newRow.insertCell()
    yCell.textContent = coords[1].toFixed(3);
    yCell.style = 'border: 1px solid black;';
  });
}

function resetPoints() {
  points.forEach(point => {
    myMap.geoObjects.remove(point.point);
  });
  points = [];
  myTable.innerHTML = '';
}
