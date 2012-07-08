__author__ = 'satra'

from time import sleep

from infoclient import InfoClient

ic = InfoClient()

ic.start(15003, 'localhost', 15002)
ic.add('activation-img', 'active')


'''
ic.send('test message', 'localhost', 15002)
ic.remove('activation-img', 'active')
ic.stop()
'''