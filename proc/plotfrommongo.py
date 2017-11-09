from pymongo import MongoClient
import numpy as np
import argparse
import pylab as plt

client = MongoClient('localhost', 27017)

db = client['test']
coll = db['data']

res = coll.find({'W': 12})
color = {'0.0': "red", '5.0': "green", '10.0': "blue"}
for data in res:
    plt.plot(data['EnergyAC'],
                label = "B "+str(data['Beta'])+" F "+str(data['Freq']),
                color = color[str(data['Freq'])])
plt.legend(loc='upper right')
plt.show()
