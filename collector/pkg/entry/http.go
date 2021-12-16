package entry

import (
    "github.com/alexraileanu/weather-station/pkg/common/model"
    "github.com/alexraileanu/weather-station/utl/math"
    "github.com/gin-gonic/gin"
    "net/http"
)

type HTTP struct {
    svc Service
}

type insert struct {
    Temp     float64 `json:"temp" binding:"required"`
    Pressure float64 `json:"pressure" binding:"required"`
    Humidity float64 `json:"humidity" binding:"required"`
    Gas      float64 `json:"gas" binding:"required"`
    Battery  float64 `json:"battery" binding:"required"`
}

type log struct {
    Msg string `json:"msg"`
}

func NewHTTP(svc Service, c *gin.RouterGroup) {
    h := HTTP{svc}

    c.POST("/entries", h.createEntry)
    c.POST("/log", h.printLog)
}

func (h HTTP) createEntry(c *gin.Context) {
    r := new(insert)
    if err := c.ShouldBindJSON(r); err != nil {
        c.AbortWithError(http.StatusBadRequest, err)
        return
    }

    battery := math.ToFixed(r.Battery, 2)
    if battery < 20 {
        go h.svc.SendBatteryMail(battery)
    }

    entry := &model.Entry{
        Temp:     math.ToFixed(r.Temp, 2),
        Pressure: math.ToFixed(r.Pressure, 2),
        Humidity: math.ToFixed(r.Humidity, 2),
        Gas:      math.ToFixed(r.Gas, 2),
        Battery:  math.ToFixed(r.Battery, 2),
    }

    h.svc.SaveEntry(entry, true)

    c.JSON(http.StatusOK, gin.H{})
    return
}

func (h HTTP) printLog(c *gin.Context) {
    r := new(log)
    if err := c.ShouldBindJSON(r); err != nil {
        c.Abort()
        return
    }

    //timestamp := time.Now().Format("2006-01-02 15:04:05")
    //fmt.Printf("[%s] - Got message from collector: %s\n", timestamp, r.Msg)
    log := &model.Log{
        Message: r.Msg,
    }
    h.svc.SaveLog(log)
    c.JSON(http.StatusOK, gin.H{})
    return
}
