import pygame
import time
import random

pygame.init()
pygame.font.init()
FONT = pygame.font.SysFont( "comicsansms", 20 )
WIDTH = 1500
HEIGHT = 1000
screen = pygame.display.set_mode( (WIDTH, HEIGHT) )
MAX_POINTS = 9999
CONNECT_RADIUS = 100 ** 2

# #########################################################################################################################################################################

class NODE:
	x = 0
	y = 0
	radius = 10
	points = 0
	done = False

	

	
	def __init__( self, x, y, r ):
		self.x = x
		self.y = y
		self.radius = r
		self.connections = []
		self.connections_checked = []
		self.visited = False
		self.points = 99999999
		self.is_end = False
		self.best_path = None
		self.is_path = False

	def draw( self ):
		#draw the circle
		if( self.points != 99999999 and self.is_path == False ):
			pygame.draw.circle( screen, ((float(self.points)/MAX_POINTS) * 255, 0, 0), (self.x, self.y), self.radius )
		
		elif self.is_path == True:
			pygame.draw.circle( screen, (0, 0, 255), (self.x, self.y), self.radius )	

		else:
			pygame.draw.circle( screen, (0, 255, 0), (self.x, self.y), self.radius )
		screen.blit( FONT.render( (str(self.points) ), False, (0,0,0)), (self.x, self.y) )
			

	def draw_connections( self ):
		# draw the connections
		for i in range( 0, len( self.connections ) ):
			self.connections[i].draw()	


	def draw_visited( self ):
		pygame.draw.circle( screen, (255, 255, 0), (self.x, self.y), self.radius )

	def draw_path( self ):
		pygame.draw.circle( screen, (0, 255, 255), (self.x, self.y), self.radius )


	def connect( self, n1, n2):
		self.connections.append( CONNECTION( n1, n2  ) )		
		self.connections_checked.append( 0 )
		

	def all_checked( self ):
		for i in range ( 0, len(self.connections_checked) ):
			if self.connections_checked[i] == 0:
				return False

		return True

	def to_string( self ):
		print "I am a node"

# #########################################################################################################################################################################

class CONNECTION:

	x1 = 0
	x2 = 0
	y1 = 0
	y2 = 0

	node1 = None
	node2 = None

	def __init__( self, n1, n2 ):
		self.x1 = n1.x
		self.y1 = n1.y
		self.x2 = n2.x
		self.y2 = n2.y
		self.node1 = n1
		self.node2 = n2
		self.checked = False

	def draw( self ):
		pygame.draw.line( screen, ( 0,255,0 ), (self.x1, self.y1), (self.x2, self.y2) )


# #########################################################################################################################################################################

def get_dist( n1, n2 ):
	x = (n1.x - n2.x) ** 2
	y = (n1.y - n2.y) ** 2
	return x + y

def update( _nodes ):

	pygame.draw.rect( screen, (255,255,255), (0, 0, WIDTH, HEIGHT) )
	for i in range ( 0, len(_nodes) ):
		_nodes[i].draw_connections()


	for i in range ( 0, len(_nodes) ):
		_nodes[i].draw()
	
	pygame.display.flip()


def sort( _nodes ):
	x = len(_nodes)
	for i in range( 0, x ):
		for j in range( i, x ):
			if _nodes[i].points > _nodes[j].points:
				temp = _nodes[i]
				_nodes[i] = _nodes[j]
				_nodes[j] = temp


def done_with( n ):
	for i in range ( 0, len( n.connections_checked ) ):
		if ( n.connections_checked[i] == 0):
			return False

	return True

# #########################################################################################################################################################################



# nodes array
nodes = []



# creating the nodes
for i in range ( 0, 100 ):
	nodes.append( NODE( random.randint( 100, WIDTH/2 ), random.randint( 100, HEIGHT/2 ), 10 ) )


#for r in range ( 0, 15 ):
#	for c in range ( 0, 15 ):
#		nodes.append( NODE( c*WIDTH/20 + 30, r*HEIGHT/20 + 30, 10 ) )

# we will connect the nodes
for i in range( 0, len(nodes) ):
	for j in range ( i+1, len(nodes) ):

		if get_dist( nodes[i], nodes[j] ) < CONNECT_RADIUS:
			nodes[i].connect( nodes[i], nodes[j] )
			nodes[j].connect( nodes[j], nodes[i] )
			print "connection " + str( len( nodes[i].connections ) ) + " for " + str(i) + " -- " + str( len( nodes[j].connections ) ) + " for " + str(j)

# #########################################################################################################################################################################



"""
start = nodes[0]
done = []
use_arr = []
current = start

MAX_POINTS = 1

for i in range( 0, len( nodes ) ):
	nodes[i].points = MAX_POINTS

start.points = 0
update( nodes )

use_arr.append( start )
"""
"""
while len( use_arr ) > 0:
	current = use_arr.pop()
	current.visited = True
	for i in range( 0, len( current.connections ) ):
		if( current.connections[i].node2.visited == False ):
			use_arr.append(current.connections[i].node2)
			current.connections[i].node2.points = current.points + get_dist( current, current.connections[i].node2 )
			if( current.connections[i].node2.points > MAX_POINTS ):
					MAX_POINTS = current.connections[i].node2.points
			update( nodes )
			time.sleep( 0.005 )
"""





# initialize the array with the start
arr = []
arr.append( nodes[0] )
update( nodes )
arr[0].points = 0
nodes[20].is_end = True

while len( arr ) != 0:
	current = arr.pop()
	for i in range ( 0, len( current.connections ) ):

		if( current.connections[i].node2.points > get_dist( current, current.connections[i].node2 ) + current.points ):
		
			current.connections[i].node2.points = get_dist( current, current.connections[i].node2 ) + current.points
			if( current.connections[i].node2.points > MAX_POINTS ):
					MAX_POINTS = current.connections[i].node2.points
		
			current.connections[i].node2.best_path = current	
			arr.append( current.connections[i].node2 )

	current.connections_checked[i] = 1
	if( current.all_checked() == True and current.is_end == True ):
		break
	sort(arr)
	update(nodes)


n = nodes[20]
while n != None:
	n.is_path = True
	n = n.best_path

update( nodes )

# #########################################################################################################################################################################

while True:
	# check input
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			quit()

	
	pygame.draw.rect( screen, (255,255,255), (0, 0, WIDTH, HEIGHT) )

	"""
	for i in range ( 0, len(nodes) ):
		nodes[i].draw_connections()


	for i in range ( 0, len(nodes) ):
		nodes[i].draw()
	

	pygame.display.flip()
	"""
	time.sleep(0.001)