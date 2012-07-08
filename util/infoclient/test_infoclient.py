__author__ = 'satra'

from time import sleep

from infoclient import InfoClient

ic = InfoClient()

ic.start(15003, 'localhost', 15002)
ic.add('activation-img', 'active')

sleep(3)
# at present this only prints how many elements are in the info vector
ic.check()

'''
ic.send('test message', 'localhost', 15002)
ic.remove('activation-img', 'active')
ic.stop()
'''