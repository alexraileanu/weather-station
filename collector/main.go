package main

import (
    "github.com/alexraileanu/weather-station/queue"
    "github.com/joho/godotenv"
    "os"

    "github.com/alexraileanu/weather-station/cmd"
    "github.com/alexraileanu/weather-station/http"

    "github.com/spf13/cobra"
)

func initDbEnv(toRun func(usr, pass, host, db string)) {
    toRun(os.Getenv("DB_USER"), os.Getenv("DB_PASS"), os.Getenv("DB_HOST"), os.Getenv("DB_NAME"))
}

var envFile string

func main() {
    cobra.OnInitialize(func() {
        err := godotenv.Load(envFile)
        if err != nil {
            panic(err)
        }
    })

    var rootCmd = &cobra.Command{Use: "weather"}
    var apiCmd = &cobra.Command{
        Use:   "api",
        Short: "Runs the backend",
        Run: func(cmd *cobra.Command, args []string) {
            s := http.SRV{}
            go initDbEnv(s.Init)

            q := queue.Queue{}
            q.Init()
        },
    }
    var migrateCmd = &cobra.Command{
        Use:   "migrate",
        Short: "Runs all the migrations",
        Run: func(c *cobra.Command, args []string) {
            initDbEnv(cmd.RunMigrations)
        },
    }

    apiCmd.Flags().StringVarP(&envFile, "e", "e", "", "Env file")
    migrateCmd.Flags().StringVarP(&envFile, "e", "e", "", "Env file")

    rootCmd.AddCommand(apiCmd)
    rootCmd.AddCommand(migrateCmd)

    rootCmd.Execute()
}
