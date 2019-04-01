#!/anaconda3/bin/python

import random
import pandas as pd
l=-65230
e=65230

pdata=[]
with open('test.xyz','w') as file :
  for i in range(0,1000) :
    pdata.append([random.uniform(l,e),random.uniform(l,e),random.uniform(l,e)])
    file.write("{0} {1} {2}\n".format(pdata[i][0],pdata[i][1],pdata[i][2]))

#pd.options.display.float_format = '{:,.2f}'.format

df=pd.DataFrame(data=pdata, columns=['x','y','z'])
#print( df)
print (df.mean())
print (df.max())
print (df.min())

