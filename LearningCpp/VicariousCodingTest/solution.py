
import Queue as Q
import pdb
import string





class Leaf(object):
	"""docstring for Leaf"""
	def __init__(self, c, freq):
		self.char = c
		self.freq = freq

	def __cmp__(self, other):
		return cmp(self.freq, other.freq)

class Node(object):
	"""docstring for Node"""
	def __init__(self, LeftNode, RightNode):
		self.LeftNode = LeftNode;
		self.RightNode = RightNode;
		self.char = LeftNode.char + RightNode.char
		self.freq = LeftNode.freq + RightNode.freq;

	def __cmp__(self, other):
		return cmp(self.freq, other.freq)
		



def CodingFromTree(TreeTop, coding=None, prefix=""):

	if coding is None:
		coding = dict()


	# reached bottom of tree
	if type(TreeTop) == Leaf:
		coding[TreeTop.char] = prefix
	else:
		# left is a 0
		# right is a 1
		coding = CodingFromTree(TreeTop.LeftNode, coding, prefix+"0")
		coding = CodingFromTree(TreeTop.RightNode, coding, prefix+"1")

	return coding


def countCharacters(s):
	count_dict = dict()
	# chars = string.ascii_letters + string.punctuation + string.whitespace
	chars = string.printable
	for c in chars:
		count = s.count(c)
		if count > 0:
			count_dict[c] = count
	return count_dict

def generateTree(count):
	# generate a list of nodes
	q = Q.PriorityQueue()
	for k,v in count.iteritems():
		q.put(Leaf(k, v));

	# while not tree.empty():
	# 	current_node = tree.get()
	# 	print("{}: {}".format(current_node.char, current_node.freq))
	iter_count = 0
	while not q.empty():
		left_node = q.get()
		try:
			right_node = q.get(False)
		except:
			print("Tree Built")
			break;
		newNode = Node(left_node, right_node)
		q.put(newNode)


		iter_count+=1
		# print("Iteration Count: {}".format(iter_count))
		# print("Node 1: {},{} combined with Node 2: {},{} to form Node 3: {},{}".format(left_node.char, left_node.freq, right_node.char, right_node.freq, newNode.char, newNode.freq))
	return newNode

def compressString(s, coding):
	compressed_string = ""
	for c in s:
		try:
			compressed_string += coding[c]
		except:
			print("Character not found: {}".format(c))

	return compressed_string

def inverseCoding(coding):
	i_coding = dict()
	for k,v in coding.iteritems():
		i_coding[v] = k

	return i_coding

def decodeString(compressed_string, coding):
	i_coding = inverseCoding(coding)
	s  = ""
	code = ""
	for c in compressed_string:
		code += c
		try:
			s += i_coding[code]
			code = ""
		except:
			continue
	return s


class Compress(object):
	"""docstring for Compress"""
	def __init__(self, s):
		self.str = s
		self.count = countCharacters(self.str)
		self.tree = generateTree(self.count)
		self.coding = CodingFromTree(self.tree)
		self.compressed_string = compressString(self.str, self.coding)


# example
original_freq = {'a' : 1, 'b' : 2, 'c' : 3, 'd': 6, 'e' : 12, 'f': 24}

original_freq = {'a' : 1, 'b' : 1, 'c' : 1, 'd': 1, 'e' : 1, 'f': 1}


# generate encoding
# coding = CodingFromTree(newNode)

test_seq = "Hello! My name is Ammar."
C = Compress(test_seq)
print(C.compressed_string)
print(decodeString(C.compressed_string, C.coding))
