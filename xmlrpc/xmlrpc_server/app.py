from xmlrpc.server import SimpleXMLRPCServer
from xmlrpc.server import SimpleXMLRPCRequestHandler
import sqlite3 as lite

class RequestHandler(SimpleXMLRPCRequestHandler):
	rpc_paths = ('/RPC2',)

with SimpleXMLRPCServer(("localhost", 8000), requestHandler=RequestHandler, allow_none=True) as server:
	con = lite.connect('test')
	cur = con.cursor()

	def add_user(name, info, fav_number, gender):
		reply = ["add_user"]
		cur.execute('SELECT name FROM users WHERE name = ?', (name,))
		name_found = cur.fetchone()
		if not name_found:
			cur.execute('INSERT INTO users(name, info, fav_number, gender) VALUES (?, ?, ?, ?)', (name, info, fav_number, gender))
			con.commit()
			reply.append('good')
		else:
			reply.append('bad')
		return reply

	def update_user(user_id, name, info, fav_number, gender):
		reply = ["update_user"]
		cur.execute('SELECT id FROM users WHERE name = ?', (name,))
		id_found = cur.fetchone()
		if id_found and str(id_found[0]) != user_id:
			reply.append('bad')
		else:
			cur.execute(
				'UPDATE users SET name = ?, info = ?, fav_number = ?, gender = ? WHERE id = ?', (name, info, fav_number, gender, user_id))
			con.commit()
			reply.append('good')
		return reply

	def delete_user(user_id):
		reply = ["delete_user"]
		cur.execute('SELECT * FROM appointments WHERE user_id = ?', (user_id,))
		if not cur.fetchone():
			cur.execute('DELETE FROM users WHERE id = ?', (user_id,))
			con.commit()
			reply.append('good')
		else:
			reply.append('bad')
		return reply

	def get_users():
		reply = ["get_users"]
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
		reply.append(table)
		return reply

	def add_appointment(user_id, place, info, date, priority):
		reply = ["add_appointment"]
		cur.execute('INSERT INTO appointments(place, info, date, priority, user_id) VALUES (?, ?, ?, ?, ?)', (place, info, date, priority, user_id))
		con.commit()
		reply.append('good')
		return reply

	def update_appointment(appointment_id, place, info, date, priority):
		reply = ["update_appointment"]
		cur.execute(
			'UPDATE appointments SET place = ?, info = ?, date = ?, priority = ? WHERE id = ?', (place, info, date, priority, appointment_id))
		con.commit()
		reply.append('good')
		return reply

	def delete_appointment(appointment_id):
		reply = ["delete_appointment"]
		cur.execute('DELETE FROM appointments WHERE id = ?', (appointment_id,))
		con.commit()
		reply.append('good')
		return reply

	def get_user_appointments(user_id):
		reply = ["get_user_appointments"]
		table = list()
		cur.execute('SELECT id, place, info, date, priority FROM appointments WHERE user_id = ?', (user_id,))
		data = cur.fetchall()
		for item in data:
			row = dict()
			row['id'] = item[0]
			row['place'] = item[1]
			row['info'] = item[2]
			row['date'] = item[3]
			row['priority'] = item[4]
			table.append(row)
		reply.append(table)
		return reply

	server.register_function(add_user)
	server.register_function(update_user)
	server.register_function(delete_user)
	server.register_function(get_users)
	server.register_function(add_appointment)
	server.register_function(update_appointment)
	server.register_function(delete_appointment)
	server.register_function(get_user_appointments)

	server.serve_forever()