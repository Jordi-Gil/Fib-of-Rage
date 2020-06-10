import sys
if(len(sys.argv) != 2):
    print("Usage: Script map_to_conver")
    exit(0)
map = open(sys.argv[1],'r')
if(map.readline()!= "TILEMAP\n"):
    print("Not properly formated the map")
    exit(0)
line = map.readline()
sizex=line[:line.find(' ')]
sizey=line[line.find(' ')+1:line.find('\t')]
line = map.readline()
line = map.readline()
line = map.readline()
line = map.readline()
raw=open(sys.argv[1]+'.raw','wb')
for a in range(0,int(sizey)):
    line = map.readline()
    for b in range(0,len(line)):
        if(b%2==0):
            if(line[b]!='\n'):
                raw.write(bytes([int(line[b])]))

raw.close()
map.close()



