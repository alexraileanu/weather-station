from api import app


if __name__ == '__main__':
    app.run(host=app.config['OWN_IP'], debug=True)
