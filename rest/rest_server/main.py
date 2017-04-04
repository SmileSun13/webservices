from flask import Flask, jsonify, request
import funcs

app = Flask(__name__)

@app.route('/user', methods=['GET'])
def get_users():
	return jsonify(funcs.get_users())

@app.route('/user/<user_id>/appointment', methods=['GET'])
def get_user_appointments(user_id):
	return jsonify(funcs.get_user_appointments(user_id))

@app.route('/user/<id>', methods=['POST'])
def update_user(id):
	data = request.json
	if len(data) == 0:
		return jsonify(funcs.delete_user(id))
	return jsonify(funcs.update_user(id, data['name'], data['info'], data['fav_number'], data['gender']))

@app.route('/user', methods=['POST'])
def add_user():
	data = request.json
	return jsonify(funcs.add_user(data['name'], data['info'], data['fav_number'], data['gender']))

@app.route('/user/<user_id>/appointment', methods=['POST'])
def add_appointment(user_id):
	data = request.json
	return jsonify(funcs.add_appointment(user_id, data['place'], data['info'], data['date'], data['priority']))

@app.route('/appointment/<id>', methods=['POST'])
def update_appointment(id):
	data = request.json
	if len(data) == 0:
		return jsonify(funcs.delete_appointment(id))
	return jsonify(funcs.update_appointment(id, data['place'], data['info'], data['date'], data['priority']))

if __name__ == '__main__':
	app.run(debug=True)