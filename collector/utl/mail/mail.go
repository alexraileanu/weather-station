package mail

import (
    "bytes"
    "errors"
    "fmt"
    "html/template"
    "os"

    "github.com/sendgrid/sendgrid-go"
    "github.com/sendgrid/sendgrid-go/helpers/mail"
)

var TemplatesFolder = "utl/tpl"

type tpl struct {
    Battery float64
}

type Service struct {
}

func New() *Service {
    return &Service{}
}

func (s *Service) SendBatteryMail(battery float64) error {
    tplData := tpl{
        Battery: battery,
    }

    templateFile := fmt.Sprintf("%s/email.gohtml", TemplatesFolder)
    return s.send(tplData, templateFile)
}

func (s *Service) send(tplData tpl, tplPath string) error {
    to := mail.NewEmail(os.Getenv("MAIL_TO_NAME"), os.Getenv("MAIL_TO_EMAIL"))
    t := template.Must(template.ParseFiles(tplPath))

    buf := bytes.Buffer{}
    err := t.Execute(&buf, tplData)
    if err != nil {
        return err
    }

    from := mail.NewEmail(os.Getenv("MAIL_FROM_NAME"), os.Getenv("MAIL_FROM_EMAIL"))
    subject := "Weather battery is running low!"
    message := mail.NewV3MailInit(from, subject, to, mail.NewContent("text/html", buf.String()))

    // disable click tracking in email
    trackingSettings := mail.NewTrackingSettings()
    clickTrackingSettings := mail.NewClickTrackingSetting()
    clickTrackingSettings.SetEnable(false)
    clickTrackingSettings.SetEnableText(false)
    trackingSettings.SetClickTracking(clickTrackingSettings)
    message.SetTrackingSettings(trackingSettings)

    client := sendgrid.NewSendClient(os.Getenv("SENDGRID_KEY"))
    r, err := client.Send(message)
    if err != nil {
        return err
    }

    if r.StatusCode != 200 {
        return errors.New(r.Body)
    }

    return nil
}
