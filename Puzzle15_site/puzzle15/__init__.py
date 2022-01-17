from flask import Flask
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config.from_object('pazzle15.config')

db = SQLAlchemy(app)

import pazzle15.views
