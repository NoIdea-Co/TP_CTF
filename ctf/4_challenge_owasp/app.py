from flask import Flask, request, jsonify, send_from_directory
import os

app = Flask(__name__)
app.config['SECRET_FLAG'] = 'CTF{owasp_idor_easy}'

USERS = {
    1: {"id": 1, "username": "alice", "token": "alice-123"},
    2: {"id": 2, "username": "bob", "token": "bob-456"},
}

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

@app.route('/img/<path:name>')
def serve_img(name):
    return send_from_directory('img', name)

# Vulnerability: IDOR on /profile?id=... without auth checks
@app.route('/profile')
def profile():
    try:
        user_id = int(request.args.get('id', ''))
    except Exception:
        return jsonify({"error": "invalid id"}), 400
    user = USERS.get(user_id)
    if not user:
        return jsonify({"error": "not found"}), 404
    return jsonify(user)

# Flag gated by knowing Bob's token (extracted via IDOR)
@app.route('/flag')
def flag():
    token = request.args.get('token', '')
    if token == USERS[2]['token']:
        return app.config['SECRET_FLAG']
    return 'nope', 403

if __name__ == '__main__':
    port = int(os.environ.get('PORT', '5006'))
    app.run(host='0.0.0.0', port=port)