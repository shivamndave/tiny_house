with open('DummyData.dat') as f:
   content = f.readlines()

file = open('dummydata.txt', 'w')
time = 1448254800

for line in content:
   file.write(str(time) + " " + str(line))
   time = time + 5
