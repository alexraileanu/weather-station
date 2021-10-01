package model

import "time"

type Entry struct {
    ID          int     `gorm:"primary_key" json:"-"`
    Temp        float32 `gorm:"column:temp; type:float(2, 2)" json:"name"`
    Pressure    float32 `gorm:"column:pressure; type:float(2, 2)" json:"ref"`
    Humidity    float32 `gorm:"column:humidity; type:float(2, 2)" json:"url"`
    Gas         float32 `gorm:"column:gas; type:float(2, 2)" json:"-"`
    Battery     float32 `gorm:"column:battery; type:float(2, 2)" json:"-"`

    CreatedAt   time.Time `json:"-"`
    UpdatedAt   time.Time `json:"-"`
}