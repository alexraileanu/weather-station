package cmd

import (
    "github.com/alexraileanu/weather-station/migrations"
)

func RunMigrations(usr, pass, host, name string) {
    m := migrations.Initialize(usr, pass, host, name)

    m.AddMigrationsTable()
}