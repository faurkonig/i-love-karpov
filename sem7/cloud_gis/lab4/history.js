var geometryRegistry = new Map();
var timeoutRegistry = new Map();
const history = [];

function geoObjectGeometryChangedCallback(e) {
  const geoObject = e.get('target');
  const oldGeometry = geometryRegistry.get(geoObject);
  const newGeometry = geoObject.geometry.getCoordinates();
  if (oldGeometry === newGeometry) return;

  const oldTimeout = timeoutRegistry.get(geoObject);
  if (oldTimeout !== undefined) {
    clearTimeout(oldTimeout);
  }
  const newTimeout = setTimeout(() => {
    console.log('Update tick');
    addEditToHistory({
      type: 'edit',
      geoObject: geoObject,
      oldGeometry: oldGeometry,
      time: new Date(),
    });
    timeoutRegistry.delete(geoObject);
    geometryRegistry.set(geoObject, newGeometry);
  }, 1000 * 1);
  timeoutRegistry.set(geoObject, newTimeout);
}

function addEditToHistory(historyElement) {
  history.push(historyElement);
  _renderHistoryTable();
}

function _renderHistoryTable() {
  const undoButton = document.getElementById('undo-button');

  if (history.length > 0) {
    undoButton.disabled = false;
  } else {
    undoButton.disabled = true;
  }

  const myTable = document
    .getElementById('history-table')
    .getElementsByTagName('tbody')[0];

  myTable.innerHTML = '';
  history.toReversed().forEach((e) => {
    const newRow = myTable.insertRow();

    const nameCell = newRow.insertCell();
    if (e.type === 'edit') {
      nameCell.textContent = `Изменение геометрии ${e.geoObject.geometry.getType()}`;
    } else if (e.type === 'delete') {
      nameCell.textContent = `Удаление ${e.geoObject.geometry.getType()}`;
    } else if (e.type === 'add') {
      nameCell.textContent = `Добавление ${e.geoObject.geometry.getType()}`;
    }
    nameCell.style = 'border: 1px solid black;';

    const idCell = newRow.insertCell();
    idCell.textContent = e.geoObject.options.get('id');
    idCell.style = 'border: 1px solid black;';

    const timeCell = newRow.insertCell();
    timeCell.textContent = e.time.toTimeString();
    timeCell.style = 'border: 1px solid black;';
  });
}

function undoLastEdit() {
  if (history.length > 0) {
    const lastItem = history.pop();
    const geoObject = lastItem.geoObject;

    if (lastItem.type === 'edit') {
      const oldGeometry = lastItem.oldGeometry;
      geometryRegistry.set(geoObject, oldGeometry);
      geoObject.geometry.setCoordinates(oldGeometry);
    } else if (lastItem.type === 'delete') {
      const oldGeometry = lastItem.oldGeometry;
      addFeatureToMap({
        type: 'Feature',
        geometry: {
          type: geoObject.geometry.getType(),
          coordinates: oldGeometry,
        },
      });
    } else if (lastItem.type === 'add') {
      myMap.geoObjects.remove(geoObject);
    }

    _renderHistoryTable();
  }
}

function clearHistory() {
  geometryRegistry.clear();
  timeoutRegistry.clear();

  while (history.length > 0) {
    history.pop();
  }
  _renderHistoryTable();
}
