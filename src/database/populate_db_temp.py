from models import Data, DataInfo, Equipment
import time
import datetime
import sqlalchemy
import sqlalchemy.orm
from sqlalchemy import *
from sqlalchemy.orm import *
from create_db import connect_to_db

engine = connect_to_db()
Session = sessionmaker(bind=engine)
session = Session()

timeNominal1 = 1448254800
timeDeferred = 1448255460
timeNominal2 = 1448256545

e = Equipment (name ='Temperature Controller', location='Power Plant', info='This piece of equipment controlls temperature set points')

di = DataInfo(equipment_id=2, name='Temperature', unit='C', longunit='Celsius', mType='setpoint')

d1 = Data(info_id=2, value=float(45), timestamp = datetime.datetime.fromtimestamp(float(timeNominal1)))
d2 = Data(info_id=2, value=float(40), timestamp = datetime.datetime.fromtimestamp(float(timeDeferred)))
d3 = Data(info_id=2, value=float(45), timestamp = datetime.datetime.fromtimestamp(float(timeNominal2)))

session.add_all([e, di, d1, d2, d3])
session.commit()

