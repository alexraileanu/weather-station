package db

import (
	"fmt"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

type Service struct {
	Db *gorm.DB
}

func Initialize(usr, pwd, host, db string) (*Service, error) {
	dsn := fmt.Sprintf("%s:%s@(%s)/%s?charset=utf8&parseTime=True&loc=Local", usr, pwd, host, db)
	d, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	return &Service{
		Db: d,
	}, nil

}
