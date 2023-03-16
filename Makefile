PYTHON = python
CHECKSTYLE = flake8
TEST_BINARIES = $(wildcard *Test.py)

all: compile test checkstyle run

compile:
	@echo "Nothing to compile for Python :-)"

test: $(TEST_BINARIES)
	#for T in $(TEST_BINARIES); do $(PYTHON) $$T; done
	python -m unittest *.py

checkstyle:
	$(CHECKSTYLE) *.py

clean:
	rm -f *.pyc

run:
	python travianBot.py
