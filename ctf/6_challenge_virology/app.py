from flask import Flask, send_from_directory, jsonify
import os, base64

app = Flask(__name__)

@app.route('/')
def index():
    return send_from_directory('.', 'app.html')

@app.route('/app.css')
def app_css():
    return send_from_directory('.', 'app.css')

@app.route('/style.css')
def style_css():
    return send_from_directory('.', 'style.css')

@app.route('/app.js')
def app_js():
    return send_from_directory('.', 'app.js')

@app.route('/files/<path:name>')
def files(name):
    return send_from_directory('.', name)

@app.route('/img/<path:name>')
def serve_img(name):
    return send_from_directory('img', name)

if __name__ == '__main__':
    port = int(os.environ.get('PORT', '5008'))
    app.run(host='0.0.0.0', port=port)