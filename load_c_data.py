#!.venv/bin/python3
import numpy as np
from sys import argv
from matplotlib import pyplot as plt
import tifffile
#from tqdm.std import tqdm

FILE_NAME=None
if len(argv) > 1:
    FILE_NAME = argv[1]
DTYPE=np.uint16
WIDTH,HEIGHT=1920,2560
data = np.fromfile(FILE_NAME, dtype=DTYPE)
_im = plt.imread(FILE_NAME[:-len("bin")] + "png")
w,h,c = _im.shape
if w <= 4:
    c,w,h = w,h,c
WIDTH,HEIGHT = w,h
labels = data.reshape(WIDTH,HEIGHT)

BASE="../Datasets/"
NAME = FILE_NAME[-len("HT-XX.bin"):-len(".bin")]
image = plt.imread(BASE + NAME + "/image.png")
print(FILE_NAME[:-len("bin")] + "tif", "\n", image.transpose(2,0,1).shape, len(np.unique(labels)))
tifffile.imwrite(FILE_NAME[:-len("bin")] + "tif", image.transpose(2,0,1)[:3,...])
#np.savez(FILE_NAME[:-len("bin")] + ".npz", image=[], masks=labels)
np.save(FILE_NAME[:-len(".bin")] + "_seg.npy", dict(image=image, masks=labels))
