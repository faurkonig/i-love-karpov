dump:
	# Create postgres SQL dump
	docker-compose exec db pg_dump --clean -U postgres -f /home/dump.sql supergames
	docker cp postgres:/home/dump.sql dump-supergames.sql
	# Create postgres custom format dump
	docker-compose exec db pg_dump --clean -U postgres -Fc -f /home/dump.backup supergames
	docker cp postgres:/home/dump.sql dump-supergames.sql

restore:
	# Copy file
	docker cp dump-supergames.sql postgres:/home/dump.sql
	# Clear and upload new data
	docker-compose exec db psql -U postgres -f /home/dump.sql supergames

