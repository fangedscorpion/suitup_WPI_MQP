all: serverForBands alarmTest

serverForBands: bandServer.c
	gcc -o serverForBands bandServer.c

alarmTest: testAlarm.c
	gcc -o alarmTest testAlarm.c 

clean:
	rm serverForBands
