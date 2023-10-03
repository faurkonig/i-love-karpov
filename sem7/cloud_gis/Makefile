%/yandex.js:
	curl "https://api-maps.yandex.ru/2.1/?apikey=$(shell cat api_key.txt)&lang=en_US" -o $@

download-yandex-sdk: lab1/yandex.js lab2/yandex.js lab3/yandex.js lab4/yandex.js
