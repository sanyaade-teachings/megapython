import avr

def wait():
	i = 0
	while i < 1000:
		i = i + 1

while true:
	portA(0xFF)
	wait()
	portA(0x00)
	wait()
