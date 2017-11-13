from pymongo import MongoClient
import numpy as np
import argparse
import pylab as plt

parser = argparse.ArgumentParser(description='')
parser.add_argument('-k','--key', dest='key',
                    help='Data to plot')
args = parser.parse_args()

client = MongoClient('localhost', 27017)

db = client['test']
coll = db['data']

res = coll.find({args.key: { "$exists": True }})
print res.count()

for data in res:

    plt.plot(data[args.key],
                label = "B "+str(data['Beta'])+" F "+str(data['Freq']))
plt.legend(loc='upper right')
plt.show()
