a = 1
b = 0

i = 0
with open("D:/Programs/C++/Fibonnaci/data/nums.out") as f:
	for line in f.readlines():
		i += 1
		try:
			num = int(line)
		except:
			print('fuck', i)
			break

		c = a
		a += b
		b = c

		if a != num:
			print('fuck', i)
			break