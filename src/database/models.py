#!/usr/bin/env python2

from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, Float, DateTime, ForeignKey
from sqlalchemy.orm import relationship
from sqlalchemy.dialects import mysql


Base = declarative_base()


class Data(Base):
    """This is a database table for single measurement and setpoint values"""
    __tablename__ = "t_data"
    id = Column(Integer, primary_key=True, autoincrement=True)
    info_id = Column(Integer, ForeignKey('t_data_info.id'))
    value = Column(Float)
    timestamp = Column(DateTime)

    def __repr__(self):
        return "<Data(id={0}, info_id={1}, value={2}, timestamp={3})>".format(
            self.id, self.info_id, self.value, self.timestamp)


class DataInfo(Base):
    """This is a database table for information about measurements and the device measuring"""
    __tablename__ = "t_data_info"
    id = Column(Integer, primary_key=True, autoincrement=True)
    equipment_id = Column(Integer, ForeignKey('t_equipment.id'))
    name = Column(String(32), nullable=False)
    unit = Column(String(6), nullable=False)
    longunit = Column(String(16), nullable=False)
    info = Column(String(128), nullable=True)
    mType = Column(mysql.ENUM("setpoint", "sensor"), nullable=False)
    datapoints = relationship("Data", backref='t_data_info')

    def __repr__(self):
        return "<DataInfo(id={0}, equipment_id={1}, name={2}, unit={3}, \
longunit={4}, info={5}, mType={6})>".format(
            self.id, self.equipment_id, self.name, self.unit, self.longunit, self.info, self.mType)


class Equipment(Base):
    """This is a database table for information about pieces of equipment"""
    __tablename__ = "t_equipment"

    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(16), nullable=False)
    location = Column(String(32), nullable=False)
    info = Column(String(128), nullable=True)

    data_info = relationship("DataInfo", backref='t_equipment')

    def __repr__(self):
        return "<Equipment(id={0}, name={1}, location={2}, info={3})>".format(
            self.id, self.name, self.location, self.info)
