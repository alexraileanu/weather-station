from sendgrid import SendGridAPIClient
from sendgrid.helpers.mail import Content, Email, Mail, To

from api import app


class Mailer:
    battery_val: int
    sg: SendGridAPIClient

    def __init__(self, battery_val: int):
        self.battery_val = battery_val
        self.sg = SendGridAPIClient(api_key=app.config['SENDGRID_API_KEY'])

    def send(self):
        from_email: Email = Email(app.config['FROM_EMAIL'])
        to_email: To = To(app.config['TO_EMAIL'])
        subject: str = 'Low battery on weather collector!'
        content: Content = Content('text/plain', f'Battery level currently at {self.battery_val}')

        mail: Mail = Mail(from_email, to_email, subject, content)
        try:
            self.sg.client.mail.send.post(request_body=mail.get())
        except:
            # if the email failed to send somehow, ignore it silently.
            pass
