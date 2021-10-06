package entry

import (
    "gorm.io/gorm"

    "github.com/alexraileanu/weather-station/pkg/common/model"
)

type Service interface {
    SaveEntry(*model.Entry, bool) (*model.Entry, error)

    SendBatteryMail(float64) error
}

type EntryDB interface {
    SaveEntry(*gorm.DB, *model.Entry) (*model.Entry, error)
}

type Mail interface {
    SendBatteryMail(float64) error
}

type Entry struct {
    db   *gorm.DB
    edb  EntryDB
    mail Mail
}

func New(db *gorm.DB, edb EntryDB, mail Mail) Entry {
    return Entry{
        db:   db,
        edb:  edb,
        mail: mail,
    }
}

func Initialize(db *gorm.DB, mail Mail) Entry {
    return New(db, EDB{}, mail)
}
