import scipy.io.wavfile 
import numpy
tab = scipy.io.wavfile.read("sound.wav")
#print tab
nowy=numpy.array(tab[1],dtype=int)
for i in nowy:
 print i,


