package model

import (
    "time"
)

type Log struct {
    ID      int    `gorm:"primary_key"`
    Message string `gorm:"column:message"`

    CreatedAt time.Time `json:"-"`
    UpdatedAt time.Time `json:"-"`
}
