#!/usr/bin/python
import sys
import time 
import pyspark

def makeKey(line):
    temp = line.split(" ")
    return (temp[0], temp[1])

start_time = time.time()
input_file = sys.argv[1]
out_dir = sys.argv[2]

sc = pyspark.SparkContext(appName='local')

transformed = sc.textFile(input_file).map(makeKey)
inverted = transformed.map(lambda ele: (ele[1], ele[0]))
num_elements = transformed.count()
roots = transformed.filter(lambda val: val[0] == val[1])
while (num_elements != inverted.join(roots).count()):
    transformed = inverted.join(transformed).map(lambda pair: (pair[1][0], pair[1][1]))    
    inverted = transformed.map(lambda ele: (ele[1], ele[0]))

transformed.saveAsTextFile(out_dir)
print("{:.2f}".format(time.time() - start_time))
sc.stop()