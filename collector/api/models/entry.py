import arrow

from api import db
from api.models.base import BaseModel
from api.models.mail import DBMail
from api.utl.mail import Mailer


class Entry(db.Model, BaseModel):
    id = db.Column(db.Integer(), primary_key=True, autoincrement=True)
    temp = db.Column(db.Float(precision=2))
    pressure = db.Column(db.Float(precision=2))
    humidity = db.Column(db.Float(precision=2))
    gas = db.Column(db.Float(precision=2))
    battery = db.Column(db.Float(precision=2))
    time = db.Column(db.Integer())

    def __init__(self, temp, pressure, humidity, gas, battery, time):
        self.temp = temp
        self.pressure = pressure
        self.humidity = humidity
        self.gas = gas
        self.battery = battery
        self.time = time

        if battery < 3100:
            yesterday = arrow.now().shift(days=-1).timestamp

            should_mail = DBMail.query.filter(DBMail.sent_at > yesterday).first() is None
            if should_mail:
                mailer = Mailer(battery)

                DBMail(int(arrow.now().timestamp)).save()

                mailer.send()
