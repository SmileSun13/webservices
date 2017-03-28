from flask import Flask, jsonify, request
import sqlite3 as lite
import json

con = None
cur = None

app = Flask(__name__)

@app.route('/', methods=['POST'])
def get_tasks():
	con = lite.connect('test')
	cur = con.cursor()
	info = request.json
	reply = dict()
	if info['action'] == 't1': #insert row in users
		cur.execute('SELECT name FROM users WHERE name = {!r}'.format(info['name']))
		name = cur.fetchone()
		if not name:
			cur.execute('INSERT INTO users(name, info, fav_number, gender) VALUES ({!r}, {!r}, {!r}, {!r})'.format(info['name'], info['info'], info['fav_num'], info['gender']))
			con.commit()
			cur.execute('SELECT id FROM users WHERE name = {!r}'.format(info['name']))
			reply['message'] = 'good'
		else:
			reply['message'] = 'bad'
		reply['action'] = 't1'
	elif info['action'] == 't2': #update row in users
		cur.execute('SELECT name, info, fav_number, gender FROM users WHERE id = {!r}'.format(info['id']))
		data = cur.fetchone()
		name = data[0]
		cur.execute('SELECT id FROM users WHERE name = {!r}'.format(info['name']))
		id = cur.fetchone()
		if id and str(id[0]) != info['id']:
			reply['message'] = 'bad'
		else:
			new_data = {'name': data[0], 'info': data[1], 'fav_num': data[2], 'gender': data[3]}
			new_data['name'] = info['name']
			new_data['info'] = info['info']
			new_data['fav_num'] = info['fav_num']
			new_data['gender'] = info['gender']
			cur.execute('UPDATE users SET name = {!r}, info = {!r}, fav_number = {!r}, gender = {!r} WHERE id = {!r}'.format(new_data['name'], new_data['info'], new_data['fav_num'], new_data['gender'], info['id']))
			con.commit()
			reply['message'] = 'good'
		reply['action'] = 't2'
	elif info['action'] == 't3': #get list of users
		reply['table_type'] = 'users'
		table = list()
		cur.execute('SELECT * FROM users')
		data = cur.fetchall()
		for item in data:
			row = dict()
			row['id'] = item[0]
			row['name'] = item[1]
			row['info'] = item[2]
			row['fav_num'] = item[3]
			row['gender'] = item[4]
			table.append(row)
		reply['table'] = table
		reply['action'] = 't3'
		reply['message'] = 'good'
	elif info['action'] == 't4': #insert row in appointments
		if 'priority' not in info:
			info['priority'] = ''
		cur.execute('INSERT INTO appointments(place, info, date, priority, user_id) VALUES ({!r}, {!r}, {!r}, {!r}, {!r})'.format(info['place'], info['info'], info['date'], info['priority'], info['id']))
		con.commit()
		reply['action'] = 't4'
		reply['message'] = 'good'
	elif info['action'] == 't5': #update row in appointments
		cur.execute('SELECT place, info, date, priority FROM appointments WHERE id = {!r}'.format(info['id']))
		data = cur.fetchone()
		new_data = {'place': data[0], 'info': data[1], 'date': data[2], 'priority': data[3]}
		new_data['place'] = info['place']
		new_data['info'] = info['info']
		new_data['date'] = info['date']
		if 'priority' in info:
			new_data['priority'] = info['priority']
		else:
			new_data['priority'] = ''
		cur.execute('UPDATE appointments SET place = {!r}, info = {!r}, date = {!r}, priority = {!r} WHERE id = {!r}'.format(new_data['place'], new_data['info'], new_data['date'], new_data['priority'], info['id']))
		con.commit()
		reply['action'] = 't5'
		reply['message'] = 'good'
	elif info['action'] == 't6': #delete row from appointments
		cur.execute('DELETE FROM appointments WHERE id = {!r}'.format(info['id']))
		con.commit()
		reply['action'] = 't6'
		reply['message'] = 'good'
	elif info['action'] == 't7': #get list of appointments of given fk
		reply['table_type'] = 'appointments'
		table = list()
		cur.execute('SELECT id, place, info, date, priority FROM appointments WHERE user_id = {!r}'.format(info['user_id']))
		data = cur.fetchall()
		for item in data:
			row = dict()
			row['id'] = item[0]
			row['place'] = item[1]
			row['info'] = item[2]
			row['date'] = item[3]
			row['priority'] = item[4]
			table.append(row)
		reply['table'] = table
		reply['action'] = 't7'
		reply['message'] = 'good'
	elif info['action'] == 't8': #delete row from users if no dependencies
		cur.execute('SELECT * FROM appointments WHERE user_id = {!r}'.format(info['id']))
		if not cur.fetchone():
			cur.execute('DELETE FROM users WHERE id = {!r}'.format(info['id']))
			con.commit()
			reply['message'] = 'good'
		else:
			reply['message'] = 'bad'
		reply['action'] = 't8'
	else:
		reply['message'] = 'KWA'
		con.commit()
	con.close()
	print(json.dumps(reply))
	return jsonify(reply)

if __name__ == '__main__':
	app.run(debug=True)