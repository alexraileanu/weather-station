package model

import (
    "time"
)

type Entry struct {
    ID       int     `gorm:"primary_key" json:"-"`
    Temp     float64 `gorm:"column:temp; type:double" json:"temperature"`
    Pressure float64 `gorm:"column:pressure; type:double" json:"pressure"`
    Humidity float64 `gorm:"column:humidity; type:double" json:"humidity"`
    Gas      float64 `gorm:"column:gas; type:double" json:"-"`
    Battery  float64 `gorm:"column:battery; type:double" json:"battery"`

    CreatedAt time.Time `json:"-"`
    UpdatedAt time.Time `json:"-"`
}
