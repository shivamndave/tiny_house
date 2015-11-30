.PHONY: install clean clobber

all: install

venv:
	virtualenv venv

install: requirements.txt venv
	venv/bin/pip install --upgrade pip
	venv/bin/pip install -r requirements.txt


clean:
	rm *.pyc

clobber:
	rm -rf venv
