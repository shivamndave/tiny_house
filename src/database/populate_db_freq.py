#! venv/bin/python

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
session=Session()

e = Equipment(name='Hertz Meter', location='Power Plant', info='This is a sensor that performs frequency readings')

di = DataInfo(equipment_id=1, name='frequency', unit='Hz', longunit='Hertz', mType='sensor')

session.add_all([e, di])

with open('dummydata.txt') as f:
   content = f.readlines()

for line in content:
   tup = line.split()
   d = Data(info_id=1, value=float(tup[1]), timestamp=datetime.datetime.fromtimestamp(float(tup[0])))
   session.add(d)

session.commit()  
