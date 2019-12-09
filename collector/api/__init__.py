from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate

app = Flask(__name__)
app.config.from_pyfile('../config/config.cfg')

db = SQLAlchemy(app)
migrate = Migrate(app, db)

from .routes import *
from .models import *
