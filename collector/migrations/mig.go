package migrations

import (
    "fmt"
    "log"

    "gorm.io/driver/mysql"
    "gorm.io/gorm"
)

type mig struct {
    db *gorm.DB
}

func Initialize(usr, pwd, host, db string) *mig {
    dsn := fmt.Sprintf("%s:%s@(%s:3306)/%s?charset=utf8&parseTime=True&loc=Local", usr, pwd, host, db)
    d, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
    if err != nil {
        log.Panic(err)
        return nil
    }

    return &mig{
        db: d,
    }
}
