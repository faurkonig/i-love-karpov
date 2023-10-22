const geometryRegistry = new Map();
const timeoutRegistry = new Map();
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
    history.push({
      geoObject: geoObject,
      oldGeometry: oldGeometry,
      time: new Date(),
    });
    timeoutRegistry.delete(geoObject);
    geometryRegistry.set(geoObject, newGeometry);

    _renderHistoryTable();
  }, 1000 * 1);
  timeoutRegistry.set(geoObject, newTimeout);
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
    nameCell.textContent = `Изменение геометрии ${e.geoObject.geometry.getType()}`;
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
    const oldGeometry = lastItem.oldGeometry;

    geometryRegistry.set(geoObject, oldGeometry);
    geoObject.geometry.setCoordinates(oldGeometry);

    _renderHistoryTable();
  }
}
