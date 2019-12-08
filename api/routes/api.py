from api import app
from flask import request


@app.route('/test', method=['POST'])
def test():
  print(request.json)