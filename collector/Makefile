dev-build:
		go build -tags dev -o weather .

dev: dev-build
		./weather api -e configs/.env

migrate: dev-build
		./weather migrate -e configs/.env

watch:
		air -c .air.toml

release:
		GOOS=linux GOARCH=arm64 go build -ldflags="-s -w" -tags prod -o weather . && upx -9 weather

deploy: release
		./deploy