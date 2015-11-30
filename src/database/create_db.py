#! venv/bin/python

from sqlalchemy import create_engine
from sqlalchemy.engine.url import URL
import models, settings

def create_tables(engine):
    models.Base.metadata.create_all(engine)

def connect_to_db():
    url = URL(**settings.DATABASE)
    return create_engine(url, echo=True)


if __name__ == "__main__":
    engine = connect_to_db()
    create_tables(engine)

