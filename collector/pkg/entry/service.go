package entry

import (
    "gorm.io/gorm"

    "github.com/alexraileanu/weather-station/pkg/common/model"
)

type Service interface {
    SaveEntry(*model.Entry, bool) (*model.Entry, error)
}

type EntryDB interface {
    SaveEntry(*gorm.DB, *model.Entry) (*model.Entry, error)
}

type Entry struct {
    db  *gorm.DB
    edb EntryDB
}

func New(db *gorm.DB, edb EntryDB) Entry {
    return Entry{
        db:  db,
        edb: edb,
    }
}

func Initialize(db *gorm.DB) Entry {
    return New(db, EDB{})
}
