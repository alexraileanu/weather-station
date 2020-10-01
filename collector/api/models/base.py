from sqlalchemy.exc import SQLAlchemyError

from api import db


class BaseModel:
    def save(self):
        try:
            db.session.add(self)
            db.session.commit()
        except SQLAlchemyError:
            pass
