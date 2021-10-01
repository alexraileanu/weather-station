package entry

import (
    "net/http"
    "github.com/gin-gonic/gin"

    "github.com/alexraileanu/weather-station/pkg/common/model"
)

type HTTP struct {
    svc Service
}

type insert struct {
    Temp       float32 `json:"temp" binding:"required"`
    Pressure   float32 `json:"pressure" binding:"required"`
    Humidity   float32 `json:"humidity" binding:"required"`
    Gas        float32 `json:"gas" binding:"required"`
    Battery    float32 `json:"battery" binding:"required"`
}

func NewHTTP(svc Service, c *gin.RouterGroup) {
    h := HTTP{svc}

    c.POST("/entries", h.createEntry)
}

func (h HTTP) createEntry(c *gin.Context) {
    r := new(insert)
    if err := c.ShouldBindJSON(r); err != nil {
        c.AbortWithError(http.StatusBadRequest, err)
        return
    }

    entry := &model.Entry{
        Temp: r.Temp,
        Pressure: r.Pressure,
        Humidity: r.Humidity,
        Gas: r.Gas,
        Battery: r.Battery,
    }

    h.svc.SaveEntry(entry, true)

    c.JSON(http.StatusOK, gin.H{})
    return
}