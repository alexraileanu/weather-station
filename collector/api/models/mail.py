from api import db
from api.models.base import BaseModel


class DBMail(db.Model, BaseModel):
    id = db.Column(db.Integer(), primary_key=True, autoincrement=True)
    sent_at = db.Column(db.Integer())

    def __init__(self, sent_at: int):
        self.sent_at = sent_at
