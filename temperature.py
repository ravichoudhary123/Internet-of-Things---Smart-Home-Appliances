#!/usr/bin/python

import Adafruit_BBIO.ADC as ADC
import time

temperature_pin = 'P9_39'
#First pin = VCC = P9_03
#Middle pin = TEMP = P9_39
#Last pin = GND = P9_34

ADC.setup()
temp_file = open("temp.txt", "w")
print "file open: ",temp_file.name
try:
	while True:	 
		reading = ADC.read(temperature_pin)
		millivolts = reading * 1800		#1.8 V = 1800 mV
		temp_c = (millivolts - 500)/10
		temp_f = (temp_c * 9/5) + 32
		print('mV = %d C = %d F = %d' % (millivolts,temp_c,temp_f))
		#temp_file.seek(0)
		temp_file.write(str(temp_f))
		temp_file.write("\n")
		time.sleep(1)
except KeyboardInterrupt:
	temp_file.close()
	print "Program Ended"

