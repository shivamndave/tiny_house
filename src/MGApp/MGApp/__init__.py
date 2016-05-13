#!/usr/bin/env python3

# import http.server
# import socketserver
# import threading
# import handler
import json
from flask import Flask, jsonify, render_template
from flask.ext.mysqldb import MySQL
import MySQLdb
from MySQLdb.cursors import DictCursor
from db_credentials import *

mysql = MySQL()
app = Flask(__name__)
app.config['MYSQL_HOST'] = DB_HOST
app.config['MYSQL_USER'] = DB_USER
app.config['MYSQL_PASSWORD'] = DB_PASS
app.config['MYSQL_DB'] = DB_NAME
mysql.init_app(app)

@app.route("/", methods=['GET'])
def index():
    return render_template('index.html')

@app.route("/api/sensors", methods=['GET'])
def all():
    json = []

    cursor = mysql.connection.cursor()
    dict_cursor = mysql.connection.cursor(cursorclass=DictCursor)

    dict_cursor.execute('''SELECT * FROM `t_sensor_info`''')
    si = dict_cursor.fetchall()

    values_si = []
    for s_item in si:
        json.append(sensor_parser(s_item, dict_cursor))

    dict_cursor.close()
    return jsonify(all=json)

@app.route("/api/sensors/<int:s_id>", methods=['GET'])
def sensor_spec(s_id):
    #app.logger.info(s_id)
    json = []

    cursor = mysql.connection.cursor()
    dict_cursor = mysql.connection.cursor(cursorclass=DictCursor)

    dict_cursor.execute('''SELECT * FROM `t_sensor_info` WHERE id=''' + str(s_id))
    s_item = dict_cursor.fetchone()
    if s_item:
        sensor_object = sensor_parser(s_item, dict_cursor)
    else:
        sensor_object = None

    return jsonify(sensor=sensor_object)

def sensor_parser(s_item, dict_cursor):
    equipment_id = s_item['equipment_id']
    sensor_id = s_item['id']
    # app.logger.info(s_item)
    # app.logger.info(equipment_id)
    query_eq = '''SELECT * FROM `t_equipment` WHERE id=''' + str(equipment_id)
    dict_cursor.execute(query_eq)
    eq = dict_cursor.fetchone()
    # app.logger.info(query_eq)
    # app.logger.info(eq)
    query_ai = '''SELECT * FROM `t_actuator_info` WHERE sensor_id=''' + str(sensor_id)
    dict_cursor.execute(query_ai)
    ai = dict_cursor.fetchone()
    # app.logger.info(query_ai)
    # app.logger.info(ai)

    values_ai = []
    if ai:
        query_data_ai = '''SELECT * FROM `t_data` WHERE actuator_id=''' + str(ai['id']) + ''' ORDER BY timestamp ASC'''
        dict_cursor.execute(query_data_ai)
        # app.logger.info(query_data_ai)

        for val in dict_cursor.fetchall():
            values_ai.append([int(val['timestamp'].strftime("%s"))*1000, val['value']])

    values_si = []
    query_data_si = '''SELECT * FROM `t_data` WHERE sensor_id=''' + str(sensor_id) + ''' ORDER BY timestamp ASC'''
    dict_cursor.execute(query_data_si)
    # app.logger.info(query_data_si)

    for val in dict_cursor.fetchall():
        values_si.append([int(val['timestamp'].strftime("%s"))*1000, val['value']])

    # app.logger.info(values_si)
    values_dict = {"sensor": values_si, "actuator": values_ai}

    query_data_status = '''SELECT * FROM `t_data` WHERE sensor_id=''' + str(sensor_id) + ''' ORDER BY timestamp DESC LIMIT 0, 1'''
    dict_cursor.execute(query_data_status)
    status = dict_cursor.fetchone()
    app.logger.info(status)
    status_dict = {}
    if status is None or status['value'] == 9999:
        status_dict = {"status": 0}
    else:
        status_dict = {"status": 1}

    s_item.update(status_dict)

    _dict={"equipment": eq, "sensor_info": s_item, "actuator_info": ai, "values": values_dict}
    return _dict

@app.route("/api/rooms", methods=['GET'])
def rooms():
    cursor = mysql.connection.cursor()
    dict_cursor = mysql.connection.cursor(cursorclass=DictCursor)

    dict_cursor.execute('''SELECT * FROM `t_room`''')
    rms = dict_cursor.fetchall()

    for room in rms:
        query_eqrm = '''SELECT * FROM `t_equipment` WHERE room_id=''' + str(room['id'])
        dict_cursor.execute(query_eqrm)
        equipment_rooms = dict_cursor.fetchall()
        # app.logger.info(equipment_rooms)
        sensors = []
        for eqrm in equipment_rooms:
            query_snrm = '''SELECT * FROM `t_sensor_info` WHERE equipment_id=''' + str(eqrm['id'])
            dict_cursor.execute(query_snrm)
            sensors.append(dict_cursor.fetchone())
        app.logger.info(sensors)
        temp = {"sensors": sensors}
        room.update(temp)
    dict_cursor.close()
    return jsonify(rooms=rms)

if __name__ == "__main__":
    app.run("0.0.0.0", debug=True)
