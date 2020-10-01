from api import db
from sqlalchemy.exc import SQLAlchemyError
from api.utl.mail import Mailer


class Entry(db.Model):
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
            Mailer(battery).send()

    def save(self):
        try:
            db.session.add(self)
            db.session.commit()
        except SQLAlchemyError:
            pass
