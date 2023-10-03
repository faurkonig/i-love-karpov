function openGeoJson() {
  const file = document.querySelector('input[type=file]').files[0];
  if (!file) {
    return;
  }

  const reader = new FileReader();
  reader.onload = function (e) {
    const contents = e.target.result;
    const objects = _convertElement(JSON.parse(contents));
    myObjectManager.add(objects);
    _addPopups();
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

function _addPopups() {
  const geoObjects = myObjectManager.objects.getAll();

  geoObjects.forEach((geoObject) => {
    geoObject.options.balloonContentLayout =
      _createLayoutForGeoObject(geoObject);
  });
}

function _createLayoutForGeoObject(geoObject) {
  return ymaps.templateLayoutFactory.createClass(
    '<h4>Меню объекта</h4>' +
      'Редактирование: <i id="editing-status">выкл</i>' +
      '<hr>' +
      '<button id="edit-geoobject-button">Вкл редактирование</button>' +
      '<br>' +
      '<button id="remove-geoobject-button">Удалить оъект</button>',
    {
      build: () => {
        BalloonContentLayout.superclass.build.call(this);

        if (this.isEditing) {
          $('#editing-status').html('вкл');
          $('#edit-geoobject-button').html('Выкл редактирование');
          $('#edit-geoobject-button').bind('click', this.startEdit);
        } else {
          $('#editing-status').html('выкл');
          $('#edit-geoobject-button').bind('click', this.endEdit);
        }
      },
      clear: () => {
        $('#edit-geoobject-button').unbind('click', this.startEdit);
        $('#edit-geoobject-button').unbind('click', this.endEdit);
        BalloonContentLayout.superclass.clear.call(this);
      },
      startEdit: () => {
        this.isEditing = true;
        geoObject.editor.startEditing();
        geoObject.balloon.close();
      },
      endEdit: () => {
        this.isEditing = false;
        geoObject.editor.endEditing();
        geoObject.balloon.close();
      },
      isEditing: false,
    },
  );
}
