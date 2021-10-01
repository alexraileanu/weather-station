package migrations

import (
    "log"

    "github.com/alexraileanu/weather-station/pkg/common/model"
)

func (m *mig) AddEntriesTable() {
    db := m.db

    ID := "20211001111800"
    migrationName := "AddEntriesTable"
    var count int64 = 0

    tableExists := db.Migrator().HasTable(&model.Entry{})
    if tableExists {
        db.Table("migrations").Where("id = ?", ID).Count(&count)
    }

    if count == 0 {
        log.Printf("[%s] Starting migration", migrationName)
        err := db.AutoMigrate(&model.Entry{})

        if err != nil {
            log.Fatalf("[%s] Migration failed: %v", migrationName, err)
        }

        migration := model.Migration{
            ID:    ID,
            Title: "add_entries_table",
        }
        db.Create(migration)

        log.Printf("[%s] Migration ran successfully", migrationName)
    }
}