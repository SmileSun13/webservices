from flask import Flask, Response, request
import sqlite3 as lite
from lxml import etree

con = None
cur = None

app = Flask(__name__)

@app.route('/', methods=['POST'])
def get_tasks():
	con = lite.connect('test')
	cur = con.cursor()
	xml = request.get_data()
	root = etree.fromstring(xml)
	info = dict()
	def getn(node, info):
		if node.text:
			info[node.tag] = node.text
		for n in node:
			getn(n, info)
	getn(root.getroottree().getroot(), info)
	xml = etree.Element('reply')
	action = etree.SubElement(xml, 'action')
	message = etree.SubElement(xml, 'message')
	if info['action'] == 't1': #insert row in users
		cur.execute('SELECT name FROM users WHERE name = {!r}'.format(info['name']))
		name = cur.fetchone()
		if not name:
			cur.execute('INSERT INTO users(name, info, fav_number, gender) VALUES ({!r}, {!r}, {!r}, {!r})'.format(info['name'], info['info'], info['fav_num'], info['gender']))
			con.commit()
			cur.execute('SELECT id FROM users WHERE name = {!r}'.format(info['name']))
			message.text = 'good'
		else:
			message.text = 'bad'
		action.text = 't1'
	elif info['action'] == 't2': #update row in users
		cur.execute('SELECT name, info, fav_number, gender FROM users WHERE id = {!r}'.format(info['id']))
		data = cur.fetchone()
		name = data[0]
		cur.execute('SELECT id FROM users WHERE name = {!r}'.format(info['name']))
		id = cur.fetchone()
		if id and str(id[0]) != info['id']:
			message.text = 'bad'
		else:
			new_data = {'name': data[0], 'info': data[1]}
			new_data['name'] = info['name']
			new_data['info'] = info['info']
			new_data['fav_num'] = info['fav_num']
			new_data['gender'] = info['gender']
			cur.execute('UPDATE users SET name = {!r}, info = {!r}, fav_number = {!r}, gender = {!r} WHERE id = {!r}'.format(new_data['name'], new_data['info'], new_data['fav_num'], new_data['gender'], info['id']))
			con.commit()
			message.text = 'good'
		action.text = 't2'
	elif info['action'] == 't3': #get list of users
		table_type = etree.SubElement(xml, 'table_type')
		table_type.text = 'users'
		table = etree.SubElement(xml, 'table')
		cur.execute('SELECT * FROM users')
		data = cur.fetchall()
		for item in data:
			row = etree.Element('row')
			id = etree.SubElement(row, 'id')
			id.text = str(item[0])
			name = etree.SubElement(row, 'name')
			name.text = item[1]
			info = etree.SubElement(row, 'info')
			info.text = item[2]
			fav_num = etree.SubElement(row, 'fav_num')
			fav_num.text = str(item[3])
			gender = etree.SubElement(row, 'gender')
			gender.text = item[4]
			table.append(row)
		action.text = 't3'
		message.text = 'good'
	elif info['action'] == 't4': #insert row in appointments
		if 'priority' not in info:
			info['priority'] = ''
		cur.execute('INSERT INTO appointments(place, info, date, priority, user_id) VALUES ({!r}, {!r}, {!r}, {!r}, {!r})'.format(info['place'], info['info'], info['date'], info['priority'], info['id']))
		con.commit()
		action.text = 't4'
		message.text = 'good'
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
		action.text = 't5'
		message.text = 'good'
	elif info['action'] == 't6': #delete row from appointments
		cur.execute('DELETE FROM appointments WHERE id = {!r}'.format(info['id']))
		con.commit()
		action.text = 't6'
		message.text = 'good'
	elif info['action'] == 't7': #get list of appointments of given fk
		table_type = etree.SubElement(xml, 'table_type')
		table_type.text = 'appointments'
		table = etree.SubElement(xml, 'table')
		cur.execute('SELECT id, place, info, date, priority FROM appointments WHERE user_id = {!r}'.format(info['user_id']))
		data = cur.fetchall()
		for item in data:
			row = etree.Element('row')
			id = etree.SubElement(row, 'id')
			id.text = str(item[0])
			place = etree.SubElement(row, 'place')
			place.text = item[1]
			info = etree.SubElement(row, 'info')
			info.text = item[2]
			date = etree.SubElement(row, 'date')
			date.text = item[3]
			priority = etree.SubElement(row, 'priority')
			priority.text = item[4]
			table.append(row)
		action.text = 't7'
		message.text = 'good'
	elif info['action'] == 't8': #delete row from users if no dependencies
		cur.execute('SELECT * FROM appointments WHERE user_id = {!r}'.format(info['id']))
		if not cur.fetchone():
			cur.execute('DELETE FROM users WHERE id = {!r}'.format(info['id']))
			con.commit()
			message.text = 'good'
		else:
			message.text = 'bad'
		action.text = 't8'
	else:
		message.text = 'KWA'
		con.commit()
	con.close()
	return Response(etree.tostring(xml), mimetype='text/xml')

if __name__ == '__main__':
	app.run(debug=True)