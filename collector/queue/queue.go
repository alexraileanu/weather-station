package queue

import (
    "encoding/json"
    "fmt"
    "github.com/alexraileanu/weather-station/pkg/common/model"
    "github.com/alexraileanu/weather-station/pkg/entry"
    "github.com/alexraileanu/weather-station/utl/db"
    mqtt "github.com/eclipse/paho.mqtt.golang"
    "log"
    "os"
    "os/signal"
    "syscall"
    "time"
)

const TAG = "QUEUE-debug"

type Queue struct {
    cfg *mqtt.ClientOptions

    pubHandler func(mqtt.Client, mqtt.Message)
}

func (q *Queue) initCfg() {
    cfg := mqtt.NewClientOptions()
    cfg.SetUsername(os.Getenv("MQTT_USER"))
    cfg.SetPassword(os.Getenv("MQTT_PASS"))
    cfg.SetClientID("collector")
    cfg.SetKeepAlive(120 * time.Second)

    cfg.AddBroker(fmt.Sprintf("tcp://%s:%s", os.Getenv("MQTT_HOST"), os.Getenv("MQTT_PORT")))

    cfg.OnConnect = func(client mqtt.Client) {
        fmt.Printf("[%s] Connected to client\n", TAG)
        if token := client.Subscribe("weather", 0, q.subHandler); token.Wait() && token.Error() != nil {
            panic(token.Error())
            return
        }
    }

    q.cfg = cfg
}

func (q *Queue) subHandler(client mqtt.Client, message mqtt.Message) {
    e := model.Entry{}
    _ = json.Unmarshal(message.Payload(), &e)
    edb := entry.EDB{}
    d, err := db.Initialize(os.Getenv("DB_USER"), os.Getenv("DB_PASS"), os.Getenv("DB_HOST"), os.Getenv("DB_NAME"))
    if err != nil {
        log.Panic(err)
        return
    }

    edb.SaveEntry(d.Db, &e)
}

func (q *Queue) Init() {
    c := make(chan os.Signal, 1)
    signal.Notify(c, os.Interrupt, syscall.SIGTERM)

    q.initCfg()

    client := mqtt.NewClient(q.cfg)
    if token := client.Connect(); token.Wait() && token.Error() != nil {
        panic(token.Error())
        return
    }

    <-c
}
