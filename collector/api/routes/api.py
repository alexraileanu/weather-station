import time

from flask import request

from api import app
from api.models.entry import Entry


@app.route('/collect', methods=['POST'])
def collect() -> str:
    request_body = request.json
    Entry(
        temp=request_body['temperature'],
        pressure=request_body['pressure'],
        humidity=request_body['humidity'],
        gas=request_body['gas'],
        battery=request_body['battery'],
        time=int(time.time())
    ).save()

    return ''
