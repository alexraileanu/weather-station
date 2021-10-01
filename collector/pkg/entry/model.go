package entry

import (
    "gorm.io/gorm"

    "github.com/alexraileanu/weather-station/pkg/common/model"
)

type EDB struct{}

func (e EDB) SaveEntry(db *gorm.DB, entry *model.Entry) (*model.Entry, error) {
    qb := db.Table("entries")

    qb.Save(entry)

    return entry, qb.Error
}