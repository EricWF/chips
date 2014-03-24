with open('extract.sh', 'w') as f:
    f.write('#!/bin/bash\n\n')

    for i in range(0, 10):
        for j in range(0, 15):
            cmd = 'convert -extract 32x32+%s+%s tileset.gif tile-%s-%s.png' % (str(i*32), str(j*32), str(i), str(j))
            f.write(cmd + '\n')
        
    
