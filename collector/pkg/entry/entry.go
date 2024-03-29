package entry

import (
    "github.com/alexraileanu/weather-station/pkg/common/model"
)

func (e Entry) SaveEntry(entry *model.Entry, new bool) (*model.Entry, error) {
    return e.edb.SaveEntry(e.db, entry)
}

func (e Entry) SaveLog(log *model.Log) (*model.Log, error) {
    return e.edb.SaveLog(e.db, log)
}

func (e Entry) SendBatteryMail(battery float64) error {
    return e.mail.SendBatteryMail(battery)
}
