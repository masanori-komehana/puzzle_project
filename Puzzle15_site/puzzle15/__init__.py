from flask import Flask
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config.from_object('puzzle15.config')

db = SQLAlchemy(app)

import puzzle15.views
