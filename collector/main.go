package main

import (
    "os"

    "github.com/alexraileanu/weather-station/cmd"
    "github.com/alexraileanu/weather-station/http"

    "github.com/joho/godotenv"
    "github.com/spf13/cobra"
)

func initEnv(toRun func(usr, pass, host, db string), env string) {
    err := godotenv.Load(env)
    if err != nil {
        panic(err)
    }

    toRun(os.Getenv("DB_USER"), os.Getenv("DB_PASS"), os.Getenv("DB_HOST"), os.Getenv("DB_NAME"))
}

func main() {
    var env string

    var rootCmd = &cobra.Command{Use: "serveradmin"}
    var apiCmd = &cobra.Command{
        Use:   "api",
        Short: "Runs the backend API",
        Run: func(cmd *cobra.Command, args []string) {
            s := http.SRV{}
            initEnv(s.Init, env)
        },
    }
    var migrateCmd = &cobra.Command{
        Use:   "migrate",
        Short: "Runs all the migrations",
        Run: func(c *cobra.Command, args []string) {
            initEnv(cmd.RunMigrations, env)
        },
    }

    apiCmd.Flags().StringVarP(&env, "e", "e", "", "Env file")
    migrateCmd.Flags().StringVarP(&env, "e", "e", "", "Env file")

    rootCmd.AddCommand(apiCmd)
    rootCmd.AddCommand(migrateCmd)

    rootCmd.Execute()
}
