package http

import (
    "context"
    "fmt"
    "github.com/alexraileanu/weather-station/utl/mail"
    "log"
    "net/http"
    "os"
    "os/signal"
    "syscall"
    "time"

    "github.com/gin-gonic/gin"
    "gorm.io/gorm"

    "github.com/alexraileanu/weather-station/pkg/entry"
    "github.com/alexraileanu/weather-station/utl/db"
)

type SRV struct {
    Eng    *gin.Engine
    db     *gorm.DB
    mws    []gin.HandlerFunc
    Server *http.Server
}

func (s *SRV) graciousShutdown() {
    // Initializing the server in a goroutine so that
    // it won't block the graceful shutdown handling below
    go func() {
        if err := s.Server.ListenAndServe(); err != nil && err != http.ErrServerClosed {
            panic(fmt.Errorf("listen: %s\n", err))
        }
    }()

    // Wait for interrupt signal to gracefully shutdown the server with
    // a timeout of 5 seconds.
    quit := make(chan os.Signal)
    // kill (no param) default send syscall.SIGTERM
    // kill -2 is syscall.SIGINT
    // kill -9 is syscall.SIGKILL but can't be catch, so don't need add it
    signal.Notify(quit, syscall.SIGINT, syscall.SIGTERM)
    <-quit
    fmt.Println("Shutting down server...")

    // The context is used to inform the server it has 5 seconds to finish
    // the request it is currently handling
    ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
    defer cancel()
    if err := s.Server.Shutdown(ctx); err != nil {
        panic(fmt.Errorf("Server forced to shutdown: %v", err))
    }

    fmt.Println("Server exiting")
}

func (s *SRV) run() {
    s.Server = &http.Server{
        Addr:    ":5000",
        Handler: s.Eng,
    }

    s.graciousShutdown()
}

func (s *SRV) setup() {
    eng := gin.New()
    for _, mw := range s.mws {
        eng.Use(mw)
    }

    entry.NewHTTP(entry.Initialize(s.db, mail.New()), eng.Group("entry"))

    s.Eng = eng
}

func (s *SRV) Init(usr, pass, host, dbName string) {
    d, err := db.Initialize(usr, pass, host, dbName)
    if err != nil {
        log.Panic(err)
        return
    }
    s.db = d.Db
    s.mws = []gin.HandlerFunc{
        gin.Recovery(),
    }

    s.setup()
    s.run()
}
