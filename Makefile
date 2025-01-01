SHELL := /usr/bin/env bash
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

BUILD_DIR = "${PWD}/build"
# connect the board to the usb and do $ arduino-cli board list
FQBN = "rp2040:rp2040:generic_rp2350"
LIBS = --libraries "rp2350,lvl,lvgl"
EXTRA_FLAGS =  --build-property "build.extra_flags=-DDEBUG_RP2040_PORT=Serial -w -fcompare-debug-second -DLV_CONF_PATH=$(ROOT_DIR)/lvl/src/lv_conf.h"
DEBUG =
ARDUINO_CLI = arduino-cli
TIO = tio
PORT ?= $(shell $(ARDUINO_CLI) board list | grep -i USB | cut -d ' ' -f 1)

SKETCH = $(wildcard *.ino)

compile:
	$(ARDUINO_CLI) compile $(DEBUG) --fqbn $(FQBN) --build-path $(BUILD_DIR) $(EXTRA_FLAGS) $(LIBS) -u -p $(PORT) $(SKETCH)

debug: DEBUG=-v --log-level debug
debug: compile

props:
	$(ARDUINO_CLI) compile --fqbn $(FQBN) --build-path $(BUILD_DIR) --show-properties

upload:
	cd ..
	$(ARDUINO_CLI) upload --fqbn $(FQBN) --port $(PORT) .

clean:
	rm -Rf $(BUILD_DIR)

serial:
	$(TIO) $(PORT)
