dump:
	# Create postgres SQL dump
	docker-compose exec db pg_dump --username=postgres supergames -f /home/dump.sql
	docker cp postgres:/home/dump.sql dump-supergames.sql
	# Create postgres custom format dump
	docker-compose exec db pg_dump --username=postgres supergames -Fc -f /home/dump.backup
	docker cp postgres:/home/dump.sql dump-supergames.sql
