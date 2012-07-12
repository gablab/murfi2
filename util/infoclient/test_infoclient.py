
from infoclientLib import InfoClient
ic = InfoClient('localhost', 15002, 'localhost', 15003)
ic.add('roi-weightedave', 'active')
ic.start()

